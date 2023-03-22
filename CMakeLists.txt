cmake_minimum_required(VERSION 3.24)
project(MatrixEvaluator)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#add_library(MatrixEvaluator INTERFACE)
#target_compile_features(MatrixEvaluator INTERFACE cxx_std_17)
#

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

find_package(PythonLibs REQUIRED)

add_library(MatrixEvaluator SHARED matrixEvalModule.cpp matrixEvalModule.h)
set(EXE main.cpp)
add_executable(main_exec ${EXE})

set_target_properties(
        MatrixEvaluator
        PROPERTIES
        PREFIX ""
        OUTPUT_NAME "MatrixEvaluator"
        LINKER_LANGUAGE C
        LINK_LIBRARIES "${PYTHON_LIBRARIES}"
        INCLUDE_DIRECTORIES "/usr/include/python3.9;${PYTHON_INCLUDE_DIR};${CMAKE_CURRENT_SOURCE_DIR}/tests"
        # COMPILE_DEFINITIONS "Py_LIMITED_API=0x03090000" # (broke in default installation)
)


set_target_properties(
        main_exec
        PROPERTIES
        PREFIX ""
        OUTPUT_NAME "PyDebug"
        LINKER_LANGUAGE C
        INCLUDE_DIRECTORIES "/usr/include/python3.9;${PYTHON_INCLUDE_DIR};${CMAKE_CURRENT_SOURCE_DIR}/tests"
)


target_link_libraries(main_exec MatrixEvaluator)


#cmake_minimum_required(VERSION 3.14)
#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)
#
#option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
#
#set(SRCS "module_def.cpp" "matrixEvalModule.cpp")
#add_library(MyModule SHARED ${SRCS})
##add_executable(PyDebug "pydbg.cpp")
#
#find_package(PythonLibs REQUIRED)
##target_link_libraries(PyDebug ${PYTHON_LIBRARIES})

