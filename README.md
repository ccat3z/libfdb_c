# libfdb_c

This repository extracts libfdb_c from [FoundationDB](https://github.com/apple/foundationdb)
and pre-process the flow code to avoid mono during compilation.

## Build

```
make build
```

This Makefile will use vcpkg to install dependencies and then compile with cmake.
You can also directly use cmake.

```
cmake -Bbuild .
cmake --build build
```

## Extract and convert libfdb_c source from foundationdb repo

```
make extract
```
