call conan_install.sh
cmake -DCMAKE_TOOLCHAIN_FILE="conan-deps-release/conan_toolchain.cmake" -B./build -DCMAKE_BUILD_TYPE=Release .
cd build
make -j 4 .
./server ../config.json
