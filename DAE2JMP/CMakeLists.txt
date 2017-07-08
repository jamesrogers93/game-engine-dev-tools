project(DAE2JMP)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Finds all header and source files and places in DEVTOOLS_INC and DEVTOOLS_INC respectively
include(include/CMakeLists.txt)
include(src/CMakeLists.txt)

# PugiXML library
find_package(PugiXML REQUIRED)
include_directories(${PUGIXML_INCLUDE_DIR})
list(APPEND PLATFORM_LIBRARIES
    ${PUGIXML_LIBRARIES}
)

# OpenCOLLADA library
find_package(OPENCOLLADA REQUIRED)
include_directories(${OPENCOLLADA_INCLUDE_DIRS})
list(APPEND PLATFORM_LIBRARIES
    "/usr/local/lib/opencollada/libbuffer.dylib"
    "/usr/local/lib/opencollada/libftoa.dylib"
    "/usr/local/lib/opencollada/libGeneratedSaxParser.dylib"
    "/usr/local/lib/opencollada/libMathMLSolver.dylib"
    "/usr/local/lib/opencollada/libOpenCOLLADABaseUtils.dylib"
    "/usr/local/lib/opencollada/libOpenCOLLADAFramework.dylib"
    "/usr/local/lib/opencollada/libOpenCOLLADASaxFrameworkLoader.dylib"
    "/usr/local/lib/opencollada/libOpenCOLLADAStreamWriter.dylib"
    "/usr/local/lib/opencollada/libUTF.dylib"
    "/usr/local/lib/opencollada/libzlib.dylib"
    #${OPENCOLLADA_LIBRARIES}
)

#find_package(ASSIMP REQUIRED)
#include_directories(${ASSIMP_INCLUDE_DIRS})
#list(APPEND PLATFORM_LIBRARIES
#    "/usr/local/lib/libassimp.3.3.1.dylib"
#)

message(STATUS "OpenCOLLADA includes:\n " ${OPENCOLLADA_INCLUDE_DIRS})
message(STATUS "OpenCOLLADA libs:\n " ${OPENCOLLADA_LIBRARIES})


add_executable(DAE2JMP ${DEVTOOLS_INC} ${DEVTOOLS_SRC})
target_link_libraries(DAE2JMP ${PLATFORM_LIBRARIES})