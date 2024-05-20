cmake_minimum_required(VERSION 3.4)
project("tp3")
set(CMAKE_CXX_STANDARD 17)
set(MODULE_SOURCE "${PROJECT_SOURCE_DIR}/source/module")
set(EXTERNALS "${PROJECT_SOURCE_DIR}/externals")

#pybind11
add_subdirectory(${EXTERNALS}/pybind11)

#matplotplusplus
add_subdirectory(${EXTERNALS}/matplotplusplus)

#module
pybind11_add_module (
    Pybind11module
    ${MODULE_SOURCE}/module.cpp
)

target_include_directories (
    Pybind11module
    PRIVATE ${MODULE_SOURCE}
)

target_link_libraries (
    Pybind11module
    PUBLIC matplot
)
