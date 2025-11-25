set(FO_CORE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../libs)

# Vendored libraries: TinyEXIF, hash-library (SHA256/etc), xxHash
set(TINYEXIF_SOURCES ${LIBS_DIR}/TinyEXIF/TinyEXIF.cpp)
set(TINYXML2_SOURCES ${LIBS_DIR}/tinyxml2/tinyxml2.cpp)
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

add_library(fo_core STATIC
    ${FO_CORE_HEADERS}
    ${FO_CORE_SOURCES}
    ${TINYEXIF_SOURCES}
    ${TINYXML2_SOURCES}
    ${HASHLIBRARY_SOURCES}
)

target_include_directories(fo_core
    PUBLIC ${FO_CORE_INCLUDE_DIR}
    PRIVATE ${LIBS_DIR}/TinyEXIF
    PRIVATE ${LIBS_DIR}/tinyxml2
    PRIVATE ${LIBS_DIR}/hash-library
    PRIVATE ${XXHASH_INCLUDE}
  PRIVATE ${LIBS_DIR}/dirent
)

target_compile_features(fo_core PUBLIC cxx_std_20)

if(MSVC)
  target_compile_options(fo_core PRIVATE /W4 /permissive-)
else()
  target_compile_options(fo_core PRIVATE -Wall -Wextra -Wpedantic)
endif()
