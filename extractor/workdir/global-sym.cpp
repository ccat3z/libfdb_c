#include <algorithm>
#include <cstring>
#include <functional>
#include <iostream>
#include <llvm/Demangle/ItaniumDemangle.h>
#include <llvm/Demangle/Utility.h>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

namespace {
class BumpPointerAllocator {
  struct BlockMeta {
    BlockMeta *Next;
    size_t Current;
  };

  static constexpr size_t AllocSize = 4096;
  static constexpr size_t UsableAllocSize = AllocSize - sizeof(BlockMeta);

  alignas(long double) char InitialBuffer[AllocSize];
  BlockMeta *BlockList = nullptr;

  void grow() {
    char *NewMeta = static_cast<char *>(std::malloc(AllocSize));
    if (NewMeta == nullptr)
      std::terminate();
    BlockList = new (NewMeta) BlockMeta{BlockList, 0};
  }

  void *allocateMassive(size_t NBytes) {
    NBytes += sizeof(BlockMeta);
    BlockMeta *NewMeta = reinterpret_cast<BlockMeta *>(std::malloc(NBytes));
    if (NewMeta == nullptr)
      std::terminate();
    BlockList->Next = new (NewMeta) BlockMeta{BlockList->Next, 0};
    return static_cast<void *>(NewMeta + 1);
  }

public:
  BumpPointerAllocator()
      : BlockList(new(InitialBuffer) BlockMeta{nullptr, 0}) {}

  void *allocate(size_t N) {
    N = (N + 15u) & ~15u;
    if (N + BlockList->Current >= UsableAllocSize) {
      if (N > UsableAllocSize)
        return allocateMassive(N);
      grow();
    }
    BlockList->Current += N;
    return static_cast<void *>(reinterpret_cast<char *>(BlockList + 1) +
                               BlockList->Current - N);
  }

  void reset() {
    while (BlockList) {
      BlockMeta *Tmp = BlockList;
      BlockList = BlockList->Next;
      if (reinterpret_cast<char *>(Tmp) != InitialBuffer)
        std::free(Tmp);
    }
    BlockList = new (InitialBuffer) BlockMeta{nullptr, 0};
  }

  ~BumpPointerAllocator() { reset(); }
};

class DefaultAllocator {
  BumpPointerAllocator Alloc;

public:
  void reset() { Alloc.reset(); }

  template <typename T, typename... Args> T *makeNode(Args &&...args) {
    return new (Alloc.allocate(sizeof(T))) T(std::forward<Args>(args)...);
  }

  void *allocateNodeArray(size_t sz) {
    return Alloc.allocate(sizeof(llvm::itanium_demangle::Node *) * sz);
  }
};

std::string demangle(const llvm::itanium_demangle::Node *node) {
  llvm::itanium_demangle::OutputBuffer ob(new char[1024], 1024);
  node->print(ob);
  ob += '\0';
  std::string name(ob.getBuffer());
  delete[] ob.getBuffer();
  return name;
}

using NodeDispatcherFn =
    std::function<bool(const llvm::itanium_demangle::Node *, int)>;

// Walk the mangling ast
template <class Node>
void walk(const Node *node, NodeDispatcherFn dispatcher, int dep = 0) {
  if (!dispatcher(node, dep))
    return;
}

auto walk_visitor(NodeDispatcherFn dispatcher, int dep = 0) {
  return [=](const auto *node) { walk(node, dispatcher, dep); };
}

template <>
void walk(const llvm::itanium_demangle::FunctionEncoding *node,
          NodeDispatcherFn dispatcher, int dep) {
  if (!dispatcher(node, dep))
    return;
  if (node->getReturnType())
    node->getReturnType()->visit(walk_visitor(dispatcher, dep + 1));
  node->getName()->visit(walk_visitor(dispatcher, dep + 1));
  auto params = node->getParams();
  for (auto param : params) {
    param->visit(walk_visitor(dispatcher, dep + 1));
  }
}
template <>
void walk(const llvm::itanium_demangle::LiteralOperator *node,
          NodeDispatcherFn dispatcher, int dep) {
  if (!dispatcher(node, dep))
    return;
}
template <>
void walk(const llvm::itanium_demangle::NestedName *node,
          NodeDispatcherFn dispatcher, int dep) {
  if (!dispatcher(node, dep))
    return;
  node->Qual->visit(walk_visitor(dispatcher, dep + 1));
  node->Name->visit(walk_visitor(dispatcher, dep + 1));
}
template <>
void walk(const llvm::itanium_demangle::LocalName *node,
          NodeDispatcherFn dispatcher, int dep) {
  if (!dispatcher(node, dep))
    return;
  node->Encoding->visit(walk_visitor(dispatcher, dep + 1));
  node->Entity->visit(walk_visitor(dispatcher, dep + 1));
}
template <>
void walk(const llvm::itanium_demangle::TemplateArgs *node,
          NodeDispatcherFn dispatcher, int dep) {
  if (!dispatcher(node, dep))
    return;
  auto params =
      const_cast<llvm::itanium_demangle::TemplateArgs *>(node)->getParams();
  for (auto param : params) {
    param->visit(walk_visitor(dispatcher, dep + 1));
  }
}
template <>
void walk(const llvm::itanium_demangle::NameWithTemplateArgs *node,
          NodeDispatcherFn dispatcher, int dep) {
  if (!dispatcher(node, dep))
    return;
  node->Name->visit(walk_visitor(dispatcher, dep + 1));
  node->TemplateArgs->visit(walk_visitor(dispatcher, dep + 1));
}

namespace {
using namespace llvm::itanium_demangle;

template <class TNode, const Node *TNode::*PTR> struct StealCVRefNode {
  friend const Node *steal_child_node(const TNode &node) { return node.*PTR; }
};
template struct StealCVRefNode<ReferenceType, &ReferenceType::Pointee>;
const Node *steal_child_node(const ReferenceType &node);
template struct StealCVRefNode<QualType, &QualType::Child>;
const Node *steal_child_node(const QualType &node);
template struct StealCVRefNode<PointerType, &PointerType::Pointee>;
const Node *steal_child_node(const PointerType &node);
} // namespace

template <>
void walk(const llvm::itanium_demangle::ReferenceType *node,
          NodeDispatcherFn dispatcher, int dep) {
  if (!dispatcher(node, dep))
    return;
  const auto *child = steal_child_node(*node);
  child->visit(walk_visitor(dispatcher, dep + 1));
}

template <>
void walk(const llvm::itanium_demangle::QualType *node,
          NodeDispatcherFn dispatcher, int dep) {
  if (!dispatcher(node, dep))
    return;
  const auto *child = steal_child_node(*node);
  child->visit(walk_visitor(dispatcher, dep + 1));
}

template <>
void walk(const llvm::itanium_demangle::PointerType *node,
          NodeDispatcherFn dispatcher, int dep) {
  if (!dispatcher(node, dep))
    return;
  const auto *child = steal_child_node(*node);
  child->visit(walk_visitor(dispatcher, dep + 1));
}

// Get node type name
const char *node_kind_name(llvm::itanium_demangle::Node::Kind k) {
  switch (k) {
#define NODE(X)                                                                \
  case llvm::itanium_demangle::Node::Kind::K##X:                               \
    return #X;
#include <llvm/Demangle/ItaniumNodes.def>
  }
}

bool is_knowned_name(const llvm::itanium_demangle::NameType *node) {
  static std::unordered_set<std::string> knowned_name = {
      // Reserved word in itanium demangler
      "...",
      "(anonymous namespace)",
      "auto",
      "'block-literal'",
      "bool",
      "char",
      "char16_t",
      "char32_t",
      "char8_t",
      "decimal128",
      "decimal32",
      "decimal64",
      "decltype(auto)",
      "double",
      "float",
      "__float128",
      "half",
      "int",
      "__int128",
      "long",
      "long double",
      "long long",
      "noexcept",
      "nullptr",
      "short",
      "signed char",
      "std",
      "std::nullptr_t",
      "string literal",
      "this",
      "throw",
      "unsigned char",
      "unsigned int",
      "unsigned __int128",
      "unsigned long",
      "unsigned long long",
      "unsigned short",
      "void",
      "wchar_t",
      // Symbol in glibc
      "stat",
      "addrinfo",
      "tm",
      "sockaddr",
      "__mbstate_t",
      "_IO_FILE",
  };
  static std::vector<std::string> knowned_prefixs = {
      "fdb_", "ssl_", "msgpack_", "x509_", "boost_",
  };

  if (knowned_name.contains(
          std::string(node->getName().begin(), node->getName().end())))
    return true;

  // FIXME: operator_xxxx
  if (node->getName().startsWith("operator"))
    return true;

  for (auto &prefix : knowned_prefixs) {
    if (node->getName().startsWith(prefix.c_str()))
      return true;
  }

  return false;
}

bool is_fdb_namespace(const std::string &ns) {
  static std::unordered_set<std::string> fdb_ns = {
      "N2",
      "detail",
      "fdb",
  };

  return fdb_ns.contains(ns);
}

bool is_knowned_namespace(const std::string &ns) {
  static std::unordered_set<std::string> knowned_ns = {
      "std",
      "boost",
  };

  if (ns.empty())
    return false;

  if (knowned_ns.contains(ns))
    return true;

  if ('a' <= ns[0] && ns[0] <= 'z')
    return true;

  return false;
}

std::string get_name(const llvm::itanium_demangle::NameType &node) {
  return std::string(node.getName().begin(), node.getName().end());
}

// Try get namespace string from a NestedName node
std::vector<const llvm::itanium_demangle::NameType *>
try_get_namespace(const llvm::itanium_demangle::NestedName &node) {
  using namespace llvm::itanium_demangle;
  using Kind = Node::Kind;
  std::vector<const NameType *> ns;

  auto *qual = node.Qual;
  while (qual->getKind() == Kind::KNestedName) {
    auto *nn = reinterpret_cast<const NestedName *>(qual);
    if (nn->Name->getKind() != Kind::KNameType) {
      return {};
    }

    ns.emplace_back(reinterpret_cast<const NameType *>(nn->Name));
    qual = nn->Qual;
  }

  if (qual->getKind() == Kind::KNameType) {
    ns.emplace_back(reinterpret_cast<const NameType *>(qual));
  } else {
    return {};
  }

  std::reverse(ns.begin(), ns.end());
  return ns;
}

std::unordered_set<const llvm::itanium_demangle::NameType *>
extract_possible_ambiguous_symbols(const llvm::itanium_demangle::Node *node) {
  using namespace llvm::itanium_demangle;
  using Kind = Node::Kind;
  std::unordered_set<const NameType *> out;

  bool has_fdb_namespaced_symbol = false;

  NodeDispatcherFn visitor = [&](const Node *node, int dep) {
    switch (node->getKind()) {
    case Kind::KNameType: {
      auto *nt = reinterpret_cast<const NameType *>(node);
      if (!is_knowned_name(nt))
        out.emplace(nt);
    } break;
    case Kind::KNestedName: { // *::name
      auto *nn = reinterpret_cast<const NestedName *>(node);

      auto ns = try_get_namespace(*nn);
      if (!ns.empty()) {
        // ns_a::ns_b::ClassC::ClassD::name
        if (is_fdb_namespace(get_name(*ns[0]))) {
          has_fdb_namespaced_symbol = true;
          return false;
        }

        if (is_knowned_namespace(get_name(*ns[0]))) {
          return false;
        }

        out.emplace(ns[0]);
      } else {
        // ns_a::ns_b::ClassC<Param1, Param2>::ClassD::name
        nn->Qual->visit(walk_visitor(visitor, dep + 1));
      }

      return false;
    } break;
    default:
      break;
    }
    return true;
  };
  node->visit(walk_visitor(visitor));

  // If a mangling name has any fdb namespace, it is not ambiguous.
  if (has_fdb_namespaced_symbol)
    out.clear();

  return out;
}
} // unnamed namespace

using Demangler = llvm::itanium_demangle::ManglingParser<DefaultAllocator>;

void usage(const char *progam) {
  std::cerr << "Usage: " << progam << " -vs --verbose --source" << std::endl;
}

int main(int argc, char *argv[]) {
  bool verbose = false;
  bool print_symbol_source = false;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
      verbose = true;
    } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--source") == 0) {
      print_symbol_source = true;
    } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      usage(argv[0]);
      return 0;
    } else {
      usage(argv[0]);
      return 1;
    }
  }

  std::string mangled_name;
  Demangler demangler(nullptr, nullptr);
  std::unordered_set<std::string> ambiguous_symbols;

  while (std::cin >> mangled_name) {
    if (verbose) {
      std::cerr << "Parsing " << mangled_name << std::endl;
    }

    demangler.reset(mangled_name.c_str(),
                    mangled_name.c_str() + mangled_name.size());
    auto *node = demangler.parse();

    if (!node) {
      if (mangled_name != "__clang_call_terminate" &&
          mangled_name != "boost_asio_detail_posix_thread_function")
        std::cerr << "failed: " << mangled_name << std::endl;
      continue;
    }

    auto ambiguous_symbols_in_mangling =
        extract_possible_ambiguous_symbols(node);

    // You can avoid mangling conflicts by modifying any of the ambiguous
    // symbols.
    if (!ambiguous_symbols_in_mangling.empty()) {
      using NameType = llvm::itanium_demangle::NameType;
      std::string selected_name;

      for (auto *node : ambiguous_symbols_in_mangling) {
        auto name = get_name(*node);

        // Prefer selected name
        if (ambiguous_symbols.contains(name)) {
          selected_name = name;
          break;
        }

        // Prefer longer upper camel case name
        if (selected_name.empty()) {
          selected_name = name;
        } else if (selected_name[0] > 'Z') {
          selected_name = name;
        } else if (name[0] <= 'Z') {
          if (name.size() > selected_name.size())
            selected_name = name;
        }
      }
      ambiguous_symbols.emplace(selected_name);

      // Print selected in mangled name
      if ((verbose || print_symbol_source)) {
        std::cerr << mangled_name << ' ' << selected_name << std::endl;
      }
    }

    // Print mangling tree
    if (verbose) {
      node->visit(
          walk_visitor([&](const llvm::itanium_demangle::Node *node, int dep) {
            using Kind = llvm::itanium_demangle::Node::Kind;

            for (int i = 0; i < dep; ++i)
              std::cerr << "\e[30m->\e[0m";

            if (node->getKind() == Kind::KNameType &&
                ambiguous_symbols_in_mangling.contains(
                    reinterpret_cast<const llvm::itanium_demangle::NameType *>(
                        node))) {
              std::cerr << "\e[34m";
            } else {
              std::cerr << "\e[31m";
            }

            std::cerr << node_kind_name(node->getKind()) << "\e[0m";
            std::cerr << ' ' << demangle(node);
            std::cerr << std::endl;
            return true;
          }));
    }
  }

  for (const auto &n : ambiguous_symbols) {
    std::cout << n << std::endl;
  }

  return 0;
}
