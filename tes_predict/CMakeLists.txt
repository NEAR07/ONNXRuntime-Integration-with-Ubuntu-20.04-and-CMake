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