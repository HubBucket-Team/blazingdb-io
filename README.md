# blazingdb-io 

# Dependencies
- General dependencies: https://github.com/BlazingDB/blazingdb-toolchain

# Build

```bash
cd blazingdb-io
mkdir build
CUDACXX=/usr/local/cuda-9.2/bin/nvcc cmake -DCMAKE_BUILD_TYPE=Debug \
      -DBUILD_TESTING=ON \
      -DBLAZINGDB_DEPENDENCIES_INSTALL_DIR=/foo/blazingsql/dependencies/ \
      -DCMAKE_INSTALL_PREFIX:PATH=/foo/blazingdb_io_install_dir/ \
      ..
make -j8
```

**NOTE:**
If you want to build the dependencies using the old C++ ABI, add this cmake argument:

```bash
-DCXX_OLD_ABI=ON
```
