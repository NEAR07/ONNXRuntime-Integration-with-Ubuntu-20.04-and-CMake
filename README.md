# Instalasi-ONNXRuntime-Pada-Ubuntu-20.04-dan-CMake

Saya akan menjelaskan langkah-langkah untuk instalasi seluruh sistem. Namun, perlu dicatat bahwa Anda juga memiliki opsi untuk menginstalnya untuk pengguna Anda secara eksklusif, menggunakan folder ~/.local/ folder. Harap diingat bahwa jika Anda memilih metode ini, Anda harus melakukan penyesuaian yang diperlukan pada variabel CMAKE_PREFIX_PATH. 

referensi : https://medium.com/@massimilianoriva96/onnxruntime-integration-with-ubuntu-and-cmake-5d7af482136a

### Pertama-tama Anda perlu mengunduh paketnya: 
```
mkdir /tmp/onnxInstall
cd /tmp/onnxInstall
wget -O onnx_archive.nupkg https://www.nuget.org/api/v2/package/Microsoft.ML.OnnxRuntime/1.15.1
unzip onnx_archive.nupkg
```

### Selanjutnya, lanjutkan dengan menyalin perpustakaan dan header ke direktori /usr/local/lib dan /usr/local/include. 
```
cd runtimes/linux-x64/native/
ln -s libonnxruntime.so libonnxruntime.so.1.15.1
sudo cp libonnxruntime.so /usr/local/lib/
sudo cp libonnxruntime.so.1.15.1 /usr/local/lib/

cd /tmp/onnxInstall
sudo mkdir -p /usr/local/include/onnxruntime/
sudo cp -r build/native/include/ /usr/local/include/onnxruntime/
```

### Sekarang untuk menggunakan fungsi find_package dari CMake, kita perlu membuat dua file cmake yang dikembangkan oleh [jcarius](https://stackoverflow.com/users/11315834/jan-ca) :
```
sudo mkdir -p /usr/local/share/cmake/onnxruntime/
sudo nano /usr/local/share/cmake/onnxruntime/onnxruntimeVersion.cmake
```

```
# Custom cmake version file by jcarius

set(PACKAGE_VERSION "1.15.1")

# Check whether the requested PACKAGE_FIND_VERSION is compatible
if("${PACKAGE_VERSION}" VERSION_LESS "${PACKAGE_FIND_VERSION}")
  set(PACKAGE_VERSION_COMPATIBLE FALSE)
else()
  set(PACKAGE_VERSION_COMPATIBLE TRUE)
  if("${PACKAGE_VERSION}" VERSION_EQUAL "${PACKAGE_FIND_VERSION}")
    set(PACKAGE_VERSION_EXACT TRUE)
  endif()
endif()
```
```
sudo nano /usr/local/share/cmake/onnxruntime/onnxruntimeConfig.cmake
```
```
# Custom cmake config file by jcarius to enable find_package(onnxruntime) without modifying LIBRARY_PATH and LD_LIBRARY_PATH
#
# This will define the following variables:
#   onnxruntime_FOUND        -- True if the system has the onnxruntime library
#   onnxruntime_INCLUDE_DIRS -- The include directories for onnxruntime
#   onnxruntime_LIBRARIES    -- Libraries to link against
#   onnxruntime_CXX_FLAGS    -- Additional (required) compiler flags

include(FindPackageHandleStandardArgs)

# Assume we are in <install-prefix>/share/cmake/onnxruntime/onnxruntimeConfig.cmake
get_filename_component(CMAKE_CURRENT_LIST_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component(onnxruntime_INSTALL_PREFIX "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

set(onnxruntime_INCLUDE_DIRS ${onnxruntime_INSTALL_PREFIX}/include/onnxruntime/include)
set(onnxruntime_LIBRARIES onnxruntime)
set(onnxruntime_CXX_FLAGS "") # no flags needed


find_library(onnxruntime_LIBRARY onnxruntime
    PATHS "${onnxruntime_INSTALL_PREFIX}/lib"
)

add_library(onnxruntime SHARED IMPORTED)
set_property(TARGET onnxruntime PROPERTY IMPORTED_LOCATION "${onnxruntime_LIBRARY}")
set_property(TARGET onnxruntime PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${onnxruntime_INCLUDE_DIRS}")
set_property(TARGET onnxruntime PROPERTY INTERFACE_COMPILE_OPTIONS "${onnxruntime_CXX_FLAGS}")

find_package_handle_standard_args(onnxruntime DEFAULT_MSG onnxruntime_LIBRARY onnxruntime_INCLUDE_DIRS)
```

# Testing
Berikut adalah tutorial langkah demi langkah untuk membuat folder kosong baru pada Ubuntu, membuat program main.cpp yang menggunakan library Onnxruntime, serta membuat dan menjalankan file CMakeLists.txt. Seperti pada folder [test_predict](https://github.com/NEAR07/ONNXRuntime-Integration-with-Ubuntu-20.04-and-CMake/tree/main/tes_predict)

### Langkah 1: Membuat Folder Baru
Buka terminal dan jalankan perintah berikut:
```
mkdir folder_test_predict
cd folder_test_predict
```

### Langkah 2: Membuat Program main.cpp

Buka editor teks favorit Anda dan salin kode berikut ke dalam file main.cpp:

```
#include <iostream>
#include </usr/local/include/onnxruntime/include/onnxruntime_cxx_api.h>

int main() {
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "ONNX_Runtime_Example"); // Membuat objek environment
    
    // Mengisi alamat model ONNX
    const char* model_path = "/home/nizhar/Downloads/onnx_model.onnx";
    
    // Membuat objek session options
    Ort::SessionOptions session_options;
    
    // Membuat objek session menggunakan model_path dan session_options
    Ort::Session session(env, model_path, session_options);
    
    std::cout << "Model berhasil dimuat." << std::endl;
    
    return 0;
}
```

### Langkah 3: Membuat File CMakeLists.txt

Buat file baru bernama CMakeLists.txt di dalam folder folder_test_predict dan salin kode berikut:
```
CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
PROJECT(prediction)

find_package(onnxruntime)

include_directories(${onnxruntime_INCLUDE_DIRS}/onnxruntime/include)

set(SOURCE_FILE
    main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILE})

target_link_libraries(${PROJECT_NAME}
    ${onnxruntime_LIBRARY}
)
```

### Langkah 4: Kompilasi dan Jalankan

Kembali ke terminal dan jalankan perintah berikut:

```
mkdir build
cd build
cmake ..
make
```

Setelah kompilasi selesai, jalankan program dengan perintah:

```
./prediction
```

Program akan menampilkan pesan "Model berhasil dimuat" jika berhasil.

Dengan ini, Anda telah membuat folder kosong baru, membuat program main.cpp yang menggunakan library Onnxruntime, serta membuat dan menjalankan file CMakeLists.txt dengan sukses.
