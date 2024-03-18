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
