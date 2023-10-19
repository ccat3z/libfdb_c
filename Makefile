BUILDER_IMAGE = foundationdb-builder
EXTRACT_WORKDIR = extractor/workdir

.PHONY: all
all: extract

.PHONY: docker-image
docker-image: extractor/Dockerfile
	cd extractor && docker build --tag "$(BUILDER_IMAGE)" .

.PHONY:
extract: docker-image | $(WORKDIR) src
	rm -rf src/*
	docker run --rm -ti \
		-u $(shell id -u):$(shell id -g) \
		-v "$(CURDIR)/$(EXTRACT_WORKDIR):/tmp/fdb_c" \
		-v "$(CURDIR)/src:/src" \
		-e FOUNDATIONDB_TAG="7.1.41" \
		"$(BUILDER_IMAGE)" \
		/tmp/fdb_c/extract.sh
