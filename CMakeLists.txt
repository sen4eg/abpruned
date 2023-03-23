cmake_minimum_required(VERSION 3.24)
project(MatrixEvaluator)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
find_package(Boost REQUIRED COMPONENTS python)
include_directories(${Boost_INCLUDE_DIRS})
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
add_definitions(-DBOOST_PYTHON_STATIC_LIB)
set(LIBS ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})

add_library(MatrixEvaluator SHARED MatrixEvaluator.cpp )
set_target_properties(MatrixEvaluator PROPERTIES PREFIX "")
set_target_properties(MatrixEvaluator PROPERTIES OUTPUT_NAME "MatrixEval")
set_target_properties(MatrixEvaluator PROPERTIES SUFFIX ".so")
target_link_libraries(MatrixEvaluator ${LIBS})

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

