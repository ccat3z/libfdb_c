
.PHONY: all extract build clean clean-all

all:

build:
	cmake -Bbuild \
		-DCMAKE_TOOLCHAIN_FILE="$(CURDIR)/vcpkg/scripts/buildsystems/vcpkg.cmake" \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
		.
	taskset -c 0-29 cmake --build build -j

extract:
	$(MAKE) -C extractor \
		OUTPUT_DIR="$(CURDIR)/src"

clean:
	-rm -r build

clean-all: clean
	$(MAKE) -C extractor clean



