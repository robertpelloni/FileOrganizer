set(FO_CORE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../libs)

# Vendored libraries: TinyEXIF, hash-library (SHA256/etc), xxHash
set(TINYEXIF_SOURCES ${LIBS_DIR}/TinyEXIF/TinyEXIF.cpp)
set(HASHLIBRARY_SOURCES
    ${LIBS_DIR}/hash-library/sha256.cpp
    ${LIBS_DIR}/hash-library/sha1.cpp
    ${LIBS_DIR}/hash-library/md5.cpp
    ${LIBS_DIR}/hash-library/crc32.cpp
    ${LIBS_DIR}/hash-library/keccak.cpp
    ${LIBS_DIR}/hash-library/sha3.cpp
)
# xxHash is header-only if XXH_INLINE_ALL is defined; no separate .c needed
set(XXHASH_INCLUDE ${LIBS_DIR}/xxHash)

file(GLOB_RECURSE FO_CORE_HEADERS ${FO_CORE_INCLUDE_DIR}/*.hpp)
file(GLOB FO_CORE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB PROVIDER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/providers/*.cpp)
list(APPEND FO_CORE_SOURCES ${PROVIDER_SOURCES})

add_library(fo_core STATIC
    ${FO_CORE_HEADERS}
    ${FO_CORE_SOURCES}
    ${TINYEXIF_SOURCES}
    ${HASHLIBRARY_SOURCES}
)

target_include_directories(fo_core
    PUBLIC ${CMAKE_SOURCE_DIR}/core/include
    PUBLIC ${LIBS_DIR}/TinyEXIF
    PUBLIC ${LIBS_DIR}/tinyxml2
    PUBLIC ${LIBS_DIR}/hash-library
    PUBLIC ${XXHASH_INCLUDE}
    PUBLIC ${LIBS_DIR}/dirent
    PUBLIC ${LIBS_DIR}/stb
)

target_compile_features(fo_core PUBLIC cxx_std_20)

if(MSVC)
  target_compile_options(fo_core PRIVATE /W4 /permissive-)
else()
  target_compile_options(fo_core PRIVATE -Wall -Wextra -Wpedantic)
endif()
