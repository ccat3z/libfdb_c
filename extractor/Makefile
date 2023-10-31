BUILDER_IMAGE = foundationdb-builder
EXTRACT_WORKDIR = workdir
OUTPUT_DIR = $(error OUTPUT_DIR is required)
OUTPUT_DIR_REAL = $(realpath $(OUTPUT_DIR))

BUILDER_DOCKER_FLAGS = \
	-u $(shell id -u):$(shell id -g) \
	-v "$(CURDIR)/$(EXTRACT_WORKDIR):$(CURDIR)/$(EXTRACT_WORKDIR)"

.PHONY: all docker-image extract build-tests clean

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

clean:
	-rm -r "${EXTRACT_WORKDIR}/foundationdb_build"
	-rm -r "${EXTRACT_WORKDIR}/foundationdb_ep"

