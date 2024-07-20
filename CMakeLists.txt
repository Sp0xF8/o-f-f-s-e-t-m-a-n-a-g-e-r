cmake_minimum_required(VERSION 3.15)
project(cs2)

set(CMAKE_CXX_STANDARD 20)


file(GLOB_RECURSE SOURCES 
            "src/*.cpp"
)

file(GLOB_RECURSE INCLUDE 
            "src/inc/*.h"
            "src/inc/*.cpp"

)

file(GLOB_RECURSE FEATURES
            "src/features/*.cpp"        "src/features/*.h"
)


file (GLOB_RECURSE RESOURCES
            "res/*.cpp"             "res/*.h"		"res/*.hpp"
            "res/fnv1a/*.cpp"       "res/fnv1a/*.h"
)

include_directories("src/inc"
                    "src/features"
                    "res/MinHook"
                    "res/fnv1a"
					"res")

add_library(cs2 SHARED ${SOURCES} ${INCLUDE} ${RESOURCES} ${FEATURES} ) 

target_link_libraries(cs2 dwmapi.lib d3d11.lib)

if(MSVC)
    target_compile_options(cs2 PUBLIC)
else()
    target_compile_options(cs2 PUBLIC -fpic -mtune=native)
    target_link_options(cs2 PUBLIC -static-libgcc -static-libstdc++ -static -lpthread)
endif()



