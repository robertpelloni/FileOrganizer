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

# SQLite3
set(SQLITE3_SOURCES ${LIBS_DIR}/sqlite3/sqlite3.c)

# Dirent (Windows shim)
if(WIN32)
    set(DIRENT_SOURCES ${LIBS_DIR}/dirent/src/dirent.c)
endif()

file(GLOB_RECURSE FO_CORE_HEADERS ${FO_CORE_INCLUDE_DIR}/*.hpp)
file(GLOB FO_CORE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
list(APPEND FO_CORE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/duplicate_finders.cpp)

# # Find optional provider dependencies
# find_package(exiv2 CONFIG)
# find_package(BLAKE3 CONFIG)
# find_package(Tesseract CONFIG)

# if(exiv2_FOUND)
#     message(STATUS "Found exiv2, enabling exiv2 metadata provider")
#     list(APPEND FO_CORE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/providers/exiv2/exiv2_metadata_provider.cpp)
# endif()

# if(BLAKE3_FOUND)
#     message(STATUS "Found BLAKE3, enabling blake3 hasher")
#     list(APPEND FO_CORE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/providers/blake3/blake3_hasher.cpp)
# endif()

# if(Tesseract_FOUND)
#     message(STATUS "Found Tesseract, enabling tesseract OCR provider")
#     list(APPEND FO_CORE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/providers/tesseract/tesseract_ocr_provider.cpp)
# endif()

add_library(fo_core STATIC
    ${FO_CORE_HEADERS}
    ${FO_CORE_SOURCES}
    ${TINYEXIF_SOURCES}
    ${TINYXML2_SOURCES}
    ${HASHLIBRARY_SOURCES}
    ${SQLITE3_SOURCES}
    ${DIRENT_SOURCES}
)

target_include_directories(fo_core
    PUBLIC ${FO_CORE_INCLUDE_DIR}
    PRIVATE ${LIBS_DIR}/TinyEXIF
    PRIVATE ${LIBS_DIR}/tinyxml2
    PRIVATE ${LIBS_DIR}/hash-library
    PRIVATE ${XXHASH_INCLUDE}
    PRIVATE ${LIBS_DIR}/dirent/src
    PRIVATE ${LIBS_DIR}/sqlite3
)

# if(exiv2_FOUND)
#     target_compile_definitions(fo_core PRIVATE FO_HAVE_EXIV2)
#     target_link_libraries(fo_core PUBLIC exiv2::exiv2lib)
# endif()

# if(BLAKE3_FOUND)
#     target_compile_definitions(fo_core PRIVATE FO_HAVE_BLAKE3)
#     target_link_libraries(fo_core PUBLIC BLAKE3::blake3)
# endif()

# if(Tesseract_FOUND)
#     target_compile_definitions(fo_core PRIVATE FO_HAVE_TESSERACT)
#     target_link_libraries(fo_core PUBLIC Tesseract::libtesseract)
# endif()

target_compile_features(fo_core PUBLIC cxx_std_20)

if(MSVC)
  target_compile_options(fo_core PRIVATE /W4 /permissive-)
else()
  target_compile_options(fo_core PRIVATE -Wall -Wextra -Wpedantic)
endif()
