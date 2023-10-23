BUILDER_IMAGE = foundationdb-builder
EXTRACT_WORKDIR = workdir
OUTPUT_DIR = $(error OUTPUT_DIR is required)
OUTPUT_DIR_REAL = $(realpath $(OUTPUT_DIR))

.PHONY: all docker-image extract clean

all: extract

docker-image: Dockerfile
	docker build --tag "$(BUILDER_IMAGE)" .

extract: docker-image
	rm -rf "$(OUTPUT_DIR)"
	mkdir -p "$(OUTPUT_DIR)"
	docker run --rm -ti \
		-u $(shell id -u):$(shell id -g) \
		-v "$(CURDIR)/$(EXTRACT_WORKDIR):$(CURDIR)/$(EXTRACT_WORKDIR)" \
		-v "$(OUTPUT_DIR_REAL):$(OUTPUT_DIR_REAL)" \
		-e FOUNDATIONDB_TAG="7.1.41" \
		"$(BUILDER_IMAGE)" \
		$(CURDIR)/$(EXTRACT_WORKDIR)/extract.sh "$(OUTPUT_DIR_REAL)"

clean:
	-rm -r "${EXTRACT_WORKDIR}/foundationdb_build"
	-rm -r "${EXTRACT_WORKDIR}/foundationdb_ep"

