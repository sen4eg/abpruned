cmake_minimum_required(VERSION 3.24)
project(MatrixEval)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
#find_package(Boost REQUIRED COMPONENTS python)
#include_directories(${Boost_INCLUDE_DIRS})
#set(CMAKE_BUILD_TYPE Release)
#add_definitions(-DPy_LIMITED_API)
#set(PYTHON_LIMITED_API_VERSION 0x03050000)

#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections")

SET(PYTHON_LIBBOS /home/arso/Documents/Python-3.11.1 /home/arso/Documents/Python-3.11.1/Include/)
include_directories(${PYTHON_LIBBOS})

#link_directories(${Boost_LIBRARY_DIRS})
#add_definitions(-DBOOST_PYTHON_STATIC_LIB)
##set(EIGEN3_INCLUDE_DIR /usr/include/eigen3/Eigen)
#link_directories("/usr/share/boost/lib")
#set(LIBS ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
#
#set(Boost_PYTHON_LIBRARY "/usr/lib/x86_64-linux-gnu/libboost_python310.a")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static-libgcc -static-libstdc++")
add_library(MatrixEval SHARED MatrixEvaluator.cpp
#        ~/CLionProjects/Python-3.9.16/Include/Python
        )
#add_library(MatrixEval SHARED MatrixEvaluator.cpp )
set_target_properties(MatrixEval PROPERTIES PREFIX "")
set_target_properties(MatrixEval PROPERTIES OUTPUT_NAME "MatrixEval")
set_target_properties(MatrixEval PROPERTIES SUFFIX ".so")
#target_link_libraries(MatrixEvaluator PUBLIC ${LIBS})
#target_link_libraries(MatrixEvaluator ${Boost_LIBRARY_DIRS} -static)
#target_link_libraries(MatrixEval ${PYTHON_LIBRARIES} )
#target_link_libraries(MatrixEval ${PYTHON_LIBRARIES} -static-libgcc -static-libstdc++)
target_compile_options(MatrixEval PRIVATE
        -DPy_BUILD_CORE
        -DPy_NO_ENABLE_SHARED
        )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os")
set(CMAKE_EXE_LINKER_FLAGS "-static")
target_link_libraries(MatrixEval "-Wl,--gc-sections")
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--strip-all")
#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --gc-sections")
#set(Boost_PYTHON_LIBRARY "/path/to/libboost_python.a")
#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${PYTHON_LIBRARIES}")

# Link the library with required libraries
#target_link_libraries(MatrixEvaluator ${LIBS})

#target_link_libraries(main_exec MatrixEvaluator)
#
#set_target_properties(
#        main_exec
#        PROPERTIES
#        PREFIX ""
#        OUTPUT_NAME "PyDebug"
#        LINKER_LANGUAGE C
#        INCLUDE_DIRECTORIES "/usr/include/python3.9;${PYTHON_INCLUDE_DIR};${CMAKE_CURRENT_SOURCE_DIR}/tests"
#)
#
#add_executable(main_exec ${EXE})


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

