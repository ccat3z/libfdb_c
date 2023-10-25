ENABLE_VCPKG 		  = ON
CMAKE_CONFIGURE_DEPS  =
CMAKE_CONFIGURE_FLAGS = -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

check = $(if $(filter $(1), on ON true TRUE yes YES 1),1)

.PHONY: all extract build clean clean-all
all: build

ifeq ($(call check, $(ENABLE_VCPKG)), 1)
vcpkg:
	git clone https://github.com/microsoft/vcpkg.git

vcpkg/vcpkg: vcpkg
	./vcpkg/bootstrap-vcpkg.sh

CMAKE_CONFIGURE_DEPS  += vcpkg/vcpkg
CMAKE_CONFIGURE_FLAGS += -DCMAKE_TOOLCHAIN_FILE="$(CURDIR)/vcpkg/scripts/buildsystems/vcpkg.cmake"
endif

build: $(CMAKE_CONFIGURE_DEPS)
	cmake -Bbuild $(CMAKE_CONFIGURE_FLAGS) .
	cmake --build build -j

extract:
	$(MAKE) -C extractor \
		OUTPUT_DIR="$(CURDIR)/src"

clean:
	-rm -r build

clean-all: clean
	$(MAKE) -C extractor clean
