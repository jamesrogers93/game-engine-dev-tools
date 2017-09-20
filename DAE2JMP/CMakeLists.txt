project(DAE2JMP)

# Finds all header and source files and places in DEVTOOLS_INC and DEVTOOLS_INC respectively
include(include/CMakeLists.txt)
include(src/CMakeLists.txt)

# Includes for it's own headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Includes for the game engine
include_directories(${CMAKE_SOURCE_DIR}/external/game-engine/game-engine/include) 

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

# Include GLM (same for all platforms)
find_package (glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

#find_package(ASSIMP REQUIRED)
#include_directories(${ASSIMP_INCLUDE_DIRS})
#list(APPEND PLATFORM_LIBRARIES
#    "/usr/local/lib/libassimp.3.3.1.dylib"
#)

message(STATUS "OpenCOLLADA includes:\n " ${OPENCOLLADA_INCLUDE_DIRS})
message(STATUS "OpenCOLLADA libs:\n " ${OPENCOLLADA_LIBRARIES})


add_executable(DAE2JMP ${DEVTOOLS_INC} ${DEVTOOLS_SRC})

# Link the Game Engine libraries
target_link_libraries(DAE2JMP GameEngine)

# Link the other libraries
target_link_libraries(DAE2JMP ${PLATFORM_LIBRARIES})