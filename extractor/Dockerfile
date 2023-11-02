FROM ubuntu:22.04

# Install basic utils
RUN apt-get update \
    && apt-get install ca-certificates lsb-release wget gnupg apt-transport-https \
        --yes --no-install-recommends --verbose-versions \
    && rm -rf /var/lib/apt/lists/*

# LLVM repo
ENV LLVM_VERSION=15
RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - \
    && echo "deb [trusted=yes] https://mirrors.tuna.tsinghua.edu.cn/llvm-apt/$(lsb_release -cs)/ llvm-toolchain-$(lsb_release -cs)-${LLVM_VERSION} main" > /etc/apt/sources.list.d/llvm.list
# Mono repo
RUN apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --keyserver-options "${http_proxy+http-proxy=${http_proxy}}" --recv-keys 3FA7E0328081BFF6A14DA29AA6A19B38D3D831EF \
    && echo "deb https://download.mono-project.com/repo/ubuntu stable-focal main" > /etc/apt/sources.list.d/mono.list

# Install build depends
RUN apt-get update \
    && apt-get install \
        llvm-${LLVM_VERSION} \
        clang-${LLVM_VERSION} \
        clang-tidy-${LLVM_VERSION} \
        lld-${LLVM_VERSION} \
        lldb-${LLVM_VERSION} \
        libc++-${LLVM_VERSION}-dev \
        libc++abi-${LLVM_VERSION}-dev \
        patch \
        ccache \
        cmake \
        ninja-build \
        patch \
        --yes --no-install-recommends \
    && rm -rf /var/lib/apt/lists/*

# Install build depends
RUN apt-get update \
    && apt-get install \
        git \
        mono-devel \
        python3 \
        make \
        libssl-dev \
        --yes --no-install-recommends \
    && rm -rf /var/lib/apt/lists/*

# Install build depends
RUN apt-get update \
    && apt-get install \
        llvm-${LLVM_VERSION}-dev \
        --yes --no-install-recommends \
    && rm -rf /var/lib/apt/lists/*

ENV CC=clang-15
ENV CXX=clang++-15