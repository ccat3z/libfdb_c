BUILDER_IMAGE = foundationdb-builder
EXTRACT_WORKDIR = workdir
OUTPUT_DIR = $(error OUTPUT_DIR is required)
OUTPUT_DIR_REAL = $(realpath $(OUTPUT_DIR))
OUTPUT_SYMBOL_FILE = $(error OUTPUT_SYMBOL_FILE is required)

BUILDER_DOCKER_FLAGS = \
	-u $(shell id -u):$(shell id -g) \
	-v "$(CURDIR)/$(EXTRACT_WORKDIR):$(CURDIR)/$(EXTRACT_WORKDIR)" \
	-e FOUNDATIONDB_TAG="7.1.41"

.PHONY: all docker-image extract build-tests global-sym clean

all: extract

docker-image: Dockerfile
	docker build --tag "$(BUILDER_IMAGE)" .

extract: docker-image
	rm -rf "$(OUTPUT_DIR)"
	mkdir -p "$(OUTPUT_DIR)"
	docker run --rm -ti  \
		$(BUILDER_DOCKER_FLAGS) \
		-v "$(OUTPUT_DIR_REAL):$(OUTPUT_DIR_REAL)" \
		"$(BUILDER_IMAGE)" \
		$(CURDIR)/$(EXTRACT_WORKDIR)/extract.sh "$(OUTPUT_DIR_REAL)"

build-tests: docker-image
	docker run --rm -ti  \
		$(BUILDER_DOCKER_FLAGS) \
		"$(BUILDER_IMAGE)" \
		$(CURDIR)/$(EXTRACT_WORKDIR)/build-tests.sh

global-sym: docker-image
	docker run --rm -ti  \
		$(BUILDER_DOCKER_FLAGS) \
		"$(BUILDER_IMAGE)" \
		$(CURDIR)/$(EXTRACT_WORKDIR)/global-sym.sh
	cp "$(EXTRACT_WORKDIR)/global-sym.list" "$(OUTPUT_SYMBOL_FILE)"

clean:
	-rm -r "${EXTRACT_WORKDIR}/foundationdb_build"
	-rm -r "${EXTRACT_WORKDIR}/foundationdb_ep"
	-rm global-sym global-sym.log global-sym.list

