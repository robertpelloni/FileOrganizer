# Submodule Dashboard

**Last Updated:** 2025-12-29 22:38:06

## Project Structure

The project is organized as follows:

- **core/**: Contains the core logic of the FileOrganizer application (scanning, hashing, database, etc.).
- **cli/**: Contains the Command Line Interface (CLI) application.
- **libs/**: Contains all external dependencies included as git submodules.
- **docs/**: Project documentation.
- **benchmarks/**: Performance benchmarks.
- **tests/**: Unit and integration tests.
- **vcpkg/**: The vcpkg package manager submodule.

## Submodules Status

| Name | Path | Version (Tag) | Commit | Date | Build # | URL |
|---|---|---|---|---|---|---|
| libs/fstlib | `libs/fstlib` | v0.1.1-111-g4d61c19 | `4d61c19` | 2025-02-07 | 180 | [Link](https://github.com/fstpackage/fstlib) |
| libs/hash-library | `libs/hash-library` | hash_library_v8-2-gd389d18 | `d389d18` | 2021-09-29 | 11 | [Link](https://github.com/stbrumme/hash-library) |
| libs/libheif | `libs/libheif` | v1.20.1-406-g81b09baa | `81b09baa` | 2025-12-28 | 4917 | [Link](https://github.com/strukturag/libheif) |
| libs/heif | `libs/heif` | v3.7.1 | `503194e` | 2025-03-11 | 59 | [Link](https://github.com/nokiatech/heif) |
| libs/imageinfo | `libs/imageinfo` | - | `5cab2df` | 2025-12-27 | 34 | [Link](https://github.com/xiaozhuai/imageinfo) |
| libs/image_info | `libs/image_info` | v1.2.2-1-g1b6222d | `1b6222d` | 2025-07-23 | 31 | [Link](https://github.com/gottfrois/image_info) |
| libs/Windows | `libs/Windows` | build-binaries-2025-08-30 | `52fc4d3` | 2025-08-23 | 3 | [Link](https://github.com/ImageMagick/Windows) |
| libs/Dependencies | `libs/Dependencies` | 2025.12.27.1720 | `1de7fbb` | 2025-12-26 | 149 | [Link](https://github.com/ImageMagick/Dependencies) |
| libs/ImageMagick | `libs/ImageMagick` | 7.1.2-12-1-g0db229e52 | `0db229e52` | 2025-12-28 | 23491 | [Link](https://github.com/ImageMagick/ImageMagick) |
| libs/gdk-pixbuf | `libs/gdk-pixbuf` | - | `a01fe1d` | 2025-10-24 | 90 | [Link](https://github.com/ImageMagick/gdk-pixbuf) |
| libs/Imath | `libs/Imath` | v3.2.0-21-g4de9a1d | `4de9a1d` | 2025-11-25 | 694 | [Link](https://github.com/AcademySoftwareFoundation/Imath) |
| libs/openexr | `libs/openexr` | v3.4-alpha-96-g166bc483 | `166bc483` | 2025-12-18 | 2807 | [Link](https://github.com/AcademySoftwareFoundation/openexr) |
| libs/OpenImageIO | `libs/OpenImageIO` | spi-Arn3.6.72.1-2787-gff8965449 | `ff8965449` | 2025-12-22 | 6391 | [Link](https://github.com/AcademySoftwareFoundation/OpenImageIO) |
| libs/OpenColorIO | `libs/OpenColorIO` | v1.0.8-912-g1d77ecda | `1d77ecda` | 2025-12-10 | 1574 | [Link](https://github.com/AcademySoftwareFoundation/OpenColorIO) |
| libs/OpenTimelineIO | `libs/OpenTimelineIO` | last_pure_python-551-g4af7407 | `4af7407` | 2025-12-18 | 820 | [Link](https://github.com/AcademySoftwareFoundation/OpenTimelineIO) |
| libs/OpenCue | `libs/OpenCue` | v1.13.8-49-g9d7fafcd | `9d7fafcd` | 2025-12-28 | 1242 | [Link](https://github.com/AcademySoftwareFoundation/OpenCue) |
| libs/OpenRV | `libs/OpenRV` | v3.0.0-89-g7351285 | `7351285` | 2025-12-18 | 556 | [Link](https://github.com/AcademySoftwareFoundation/OpenRV) |
| libs/openfx | `libs/openfx` | OFX_Release_1.5.1 | `ab77951` | 2025-11-20 | 793 | [Link](https://github.com/AcademySoftwareFoundation/openfx) |
| libs/openapv | `libs/openapv` | v0.2.0.4-8-g1a7845a | `1a7845a` | 2025-12-16 | 208 | [Link](https://github.com/AcademySoftwareFoundation/openapv) |
| libs/libxml2 | `libs/libxml2` | v2.15.0-35-g9827e6e4 | `9827e6e4` | 2025-12-22 | 7730 | [Link](https://github.com/GNOME/libxml2) |
| libs/libxmlplusplus | `libs/libxmlplusplus` | 5.6.0 | `80ce21a` | 2025-10-28 | 553 | [Link](https://github.com/libxmlplusplus/libxmlplusplus) |
| libs/json-c | `libs/json-c` | - | `a1249bf` | 2025-12-01 | 1399 | [Link](https://github.com/json-c/json-c) |
| libs/SysmonForLinux | `libs/SysmonForLinux` | 1.4.0.0-1-g412cb37 | `412cb37` | 2025-07-03 | 130 | [Link](https://github.com/microsoft/SysmonForLinux) |
| libs/fast-lzma2 | `libs/fast-lzma2` | v1.0.1-7-gb44b79b | `b44b79b` | 2025-10-26 | 244 | [Link](https://github.com/conor42/fast-lzma2) |
| libs/pxz | `libs/pxz` | - | `136e5c2` | 2022-05-09 | 68 | [Link](https://github.com/jnovy/pxz) |
| libs/openh264 | `libs/openh264` | v1.1-3041-gcf568c83 | `cf568c83` | 2025-10-28 | 5264 | [Link](https://github.com/cisco/openh264) |
| libs/libffi | `libs/libffi` | v3.2.1-1174-g2263d60 | `2263d60` | 2025-12-26 | 1921 | [Link](https://github.com/libffi/libffi) |
| libs/dragonffi | `libs/dragonffi` | dffi-0.9.4 | `90c66b3` | 2025-02-08 | 166 | [Link](https://github.com/aguinet/dragonffi) |
| libs/WizardsToolkit | `libs/WizardsToolkit` | - | `af6884f` | 2025-12-18 | 996 | [Link](https://github.com/ImageMagick/WizardsToolkit) |
| libs/openjpeg | `libs/openjpeg` | v2.5.4-5-g8ac52623 | `8ac52623` | 2025-12-05 | 3150 | [Link](https://github.com/uclouvain/openjpeg) |
| libs/libzip | `libs/libzip` | v1.11.4-36-ga1364389 | `a1364389` | 2025-12-01 | 3661 | [Link](https://github.com/nih-at/libzip) |
| libs/libiconv | `libs/libiconv` | libiconv-1.17-1 | `880a1fa` | 2024-10-27 | 44 | [Link](https://github.com/winlibs/libiconv) |
| libs/ckmame | `libs/ckmame` | v2.1-154-gad23f21b | `ad23f21b` | 2025-12-18 | 3202 | [Link](https://github.com/nih-at/ckmame) |
| libs/nihtest | `libs/nihtest` | v1.9.1-6-g3adeb60 | `3adeb60` | 2025-12-18 | 136 | [Link](https://github.com/nih-at/nihtest) |
| libs/ziptools | `libs/ziptools` | - | `24ccea5` | 2020-05-03 | 13 | [Link](https://github.com/nih-at/ziptools) |
| libs/nihtest-cpp | `libs/nihtest-cpp` | - | `8eafcb2` | 2023-03-22 | 205 | [Link](https://github.com/nih-at/nihtest-cpp) |
| libs/openssl | `libs/openssl` | openssl-1.1.1t | `58bea78d` | 2023-02-13 | 47 | [Link](https://github.com/winlibs/openssl) |
| libs/sqlite3 | `libs/sqlite3` | sqlite3-3.49.2 | `182a6a4` | 2025-07-07 | 24 | [Link](https://github.com/winlibs/sqlite3) |
| libs/postgresql | `libs/postgresql` | libpq-16.9 | `59a67cba` | 2025-06-25 | 53 | [Link](https://github.com/winlibs/postgresql) |
| libs/cURL | `libs/cURL` | libcurl-8.17.0 | `97a22cc1` | 2025-12-15 | 111 | [Link](https://github.com/winlibs/cURL) |
| libs/libtidy | `libs/libtidy` | libtidy-5.6.0-1-1-gcfff694 | `cfff694` | 2025-01-21 | 14 | [Link](https://github.com/winlibs/libtidy) |
| libs/libjpeg | `libs/libjpeg` | libjpeg-turbo-3.0.3-1-g1177955 | `1177955` | 2025-01-20 | 19 | [Link](https://github.com/winlibs/libjpeg) |
| libs/glib | `libs/glib` | glib-2.53.3-1-1-ge9f0a75 | `e9f0a75` | 2024-11-18 | 21 | [Link](https://github.com/winlibs/glib) |
| libs/enchant | `libs/enchant` | enchant-1.6.0-11-gf8238b4 | `f8238b4` | 2024-11-18 | 17 | [Link](https://github.com/winlibs/enchant) |
| libs/net-snmp | `libs/net-snmp` | net-snmp-5.9.4 | `0c47808` | 2024-09-30 | 18 | [Link](https://github.com/winlibs/net-snmp) |
| libs/openldap | `libs/openldap` | openldap-2.4.47-1 | `b2ffd90` | 2024-10-31 | 44 | [Link](https://github.com/winlibs/openldap) |
| libs/nghttp2 | `libs/nghttp2` | nghttp2-1.64.0 | `2bfeb83` | 2024-11-06 | 22 | [Link](https://github.com/winlibs/nghttp2) |
| libs/libxpm | `libs/libxpm` | libxpm-3.5.12pl1-1-2-g8cbfcb3 | `8cbfcb3` | 2024-11-04 | 18 | [Link](https://github.com/winlibs/libxpm) |
| libs/cyrus-sasl | `libs/cyrus-sasl` | libsasl-2.1.27-3 | `d933c03` | 2022-06-11 | 10 | [Link](https://github.com/winlibs/cyrus-sasl) |
| libs/icu4c | `libs/icu4c` | ICU-75.1-1-gb5cdbf1d | `b5cdbf1d` | 2024-10-31 | 40 | [Link](https://github.com/winlibs/icu4c) |
| libs/mpir | `libs/mpir` | mpir-3.0.0-1 | `62672a0` | 2024-10-30 | 18 | [Link](https://github.com/winlibs/mpir) |
| libs/gettext | `libs/gettext` | libintl-0.18.3-9 | `04c409c` | 2024-10-29 | 17 | [Link](https://github.com/winlibs/gettext) |
| libs/libxslt | `libs/libxslt` | libxslt-1.1.39 | `dc7d30c` | 2024-02-10 | 30 | [Link](https://github.com/winlibs/libxslt) |
| libs/libwebp | `libs/libwebp` | libwebp-1.3.2 | `f6ee4b2` | 2024-01-31 | 13 | [Link](https://github.com/winlibs/libwebp) |
| libs/freetype | `libs/freetype` | freetype-2.13.3 | `70391bf` | 2024-09-21 | 27 | [Link](https://github.com/winlibs/freetype) |
| libs/libavif | `libs/libavif` | libavif-1.1.1 | `ecc52e7` | 2024-10-28 | 7 | [Link](https://github.com/winlibs/libavif) |
| libs/libssh2 | `libs/libssh2` | libssh2-1.11.1 | `a768549` | 2024-10-27 | 32 | [Link](https://github.com/winlibs/libssh2) |
| libs/libpng | `libs/libpng` | libpng-1.6.43 | `44baa87` | 2024-10-27 | 27 | [Link](https://github.com/winlibs/libpng) |
| libs/libsodium | `libs/libsodium` | libsodium-1.0.20 | `822546c` | 2024-10-26 | 10 | [Link](https://github.com/winlibs/libsodium) |
| libs/qdbm | `libs/qdbm` | libqdbm-1.8.78 | `586aa47` | 2016-09-26 | 2 | [Link](https://github.com/winlibs/qdbm) |
| libs/oniguruma | `libs/oniguruma` | libonig-6.9.9 | `ac76cdc` | 2024-10-26 | 11 | [Link](https://github.com/winlibs/oniguruma) |
| libs/argon2 | `libs/argon2` | libargon2-20190702 | `fb53ef3` | 2019-07-02 | 9 | [Link](https://github.com/winlibs/argon2) |
| libs/lmdb | `libs/lmdb` | liblmdb-0.9.33 | `46aef27` | 2024-10-26 | 8 | [Link](https://github.com/winlibs/lmdb) |
| libs/libbzip2 | `libs/libbzip2` | libbzip2-1.0.8-1 | `99d3c48` | 2020-02-11 | 8 | [Link](https://github.com/winlibs/libbzip2) |
| libs/zlib | `libs/zlib` | zlib-1.3.1 | `65dd212` | 2024-07-26 | 19 | [Link](https://github.com/winlibs/zlib) |
| libs/ssdeep | `libs/ssdeep` | ssdeep-2.13 | `c509f99` | 2016-09-20 | 6 | [Link](https://github.com/winlibs/ssdeep) |
| libs/wineditline | `libs/wineditline` | WinEditLine-2.206 | `60eb709` | 2022-05-03 | 18 | [Link](https://github.com/winlibs/wineditline) |
| libs/pthreads | `libs/pthreads` | pthreads-3.0.0 | `648488d` | 2022-03-22 | 2 | [Link](https://github.com/winlibs/pthreads) |
| libs/imap | `libs/imap` | imap-2007f-1 | `4f5969e` | 2022-02-01 | 4 | [Link](https://github.com/winlibs/imap) |
| libs/pslib | `libs/pslib` | pslib-0.4.6 | `4c6b93b` | 2021-11-19 | 1 | [Link](https://github.com/winlibs/pslib) |
| libs/libmcrypt | `libs/libmcrypt` | libmcrypt-2.5.8-3-g1e93cbe | `1e93cbe` | 2019-09-17 | 4 | [Link](https://github.com/winlibs/libmcrypt) |
| libs/dirent | `libs/dirent` | dirent-20080629-5-gf7d4841 | `f7d4841` | 2019-09-17 | 6 | [Link](https://github.com/winlibs/dirent) |
| libs/c-ares | `libs/c-ares` | - | `6c783a9` | 2016-09-16 | 1 | [Link](https://github.com/winlibs/c-ares) |
| libs/httpd | `libs/httpd` | - | `806e1f383c` | 2025-12-22 | 34264 | [Link](https://github.com/apache/httpd) |
| libs/fribidi | `libs/fribidi` | - | `cbcc997` | 2013-09-16 | 2 | [Link](https://github.com/winlibs/fribidi) |
| libs/libvpx | `libs/libvpx` | libvpx-1.1.0-2-g5e76714 | `5e76714` | 2013-04-09 | 4 | [Link](https://github.com/winlibs/libvpx) |
| libs/libvbucket | `libs/libvbucket` | 2.1.1r-16-g5590fff | `5590fff` | 2014-03-20 | 126 | [Link](https://github.com/membase/libvbucket) |
| libs/FFmpeg | `libs/FFmpeg` | n8.1-dev-1652-g9ab2a437a1 | `9ab2a437a1` | 2025-12-28 | 122277 | [Link](https://github.com/FFmpeg/FFmpeg) |
| libs/hashcat | `libs/hashcat` | v3.00-beta-10110-g2d71af371 | `2d71af371` | 2025-11-20 | 11055 | [Link](https://github.com/hashcat/hashcat) |
| libs/radare2 | `libs/radare2` | 6.0.7-282-g291c5ff708 | `291c5ff708` | 2025-12-28 | 34538 | [Link](https://github.com/radareorg/radare2) |
| libs/C | `libs/C` | - | `e5dad3fa` | 2023-09-27 | 1536 | [Link](https://github.com/TheAlgorithms/C) |
| libs/vlc | `libs/vlc` | 4.0.0-dev-35568-g5a666ee2df | `5a666ee2df` | 2025-12-28 | 108862 | [Link](https://github.com/videolan/vlc) |
| libs/sumatrapdf | `libs/sumatrapdf` | 3.1.2rel-6557-g3e74038b3 | `3e74038b3` | 2025-12-28 | 16105 | [Link](https://github.com/sumatrapdfreader/sumatrapdf) |
| libs/mpv | `libs/mpv` | v0.41.0-41-g4ecf7293f5 | `4ecf7293f5` | 2025-12-28 | 54487 | [Link](https://github.com/mpv-player/mpv) |
| libs/sqlite | `libs/sqlite` | version-3.51.0-464-g6693e78772 | `6693e78772` | 2025-12-28 | 31132 | [Link](https://github.com/sqlite/sqlite) |
| libs/tinyxml2 | `libs/tinyxml2` | 2.0.2-933-g3324d04 | `3324d04` | 2025-11-22 | 1278 | [Link](https://github.com/leethomason/tinyxml2) |
| libs/libvips | `libs/libvips` | v8.17.0-test4-324-g9e641793c | `9e641793c` | 2025-12-24 | 9118 | [Link](https://github.com/libvips/libvips) |
| libs/opencv | `libs/opencv` | 4.12.0-525-g40ab411032 | `40ab411032` | 2025-12-27 | 35898 | [Link](https://github.com/opencv/opencv) |
| libs/MediaInfoLib | `libs/MediaInfoLib` | v25.10-17-g558a4d6bf | `558a4d6bf` | 2025-12-27 | 9633 | [Link](https://github.com/MediaArea/MediaInfoLib) |
| libs/MediaInfo | `libs/MediaInfo` | v25.10-32-g6722fae3d | `6722fae3d` | 2025-12-27 | 2717 | [Link](https://github.com/MediaArea/MediaInfo) |
| libs/mm_file | `libs/mm_file` | v1.0.0-2-g5b52913 | `5b52913` | 2024-08-22 | 15 | [Link](https://github.com/jermp/mm_file) |
| libs/TinyEXIF | `libs/TinyEXIF` | 1.0.2-13-g39bba86 | `39bba86` | 2025-11-17 | 38 | [Link](https://github.com/cdcseacave/TinyEXIF) |
| libs/libjpeg-turbo | `libs/libjpeg-turbo` | 2.1.4-435-gaf9c1c26 | `af9c1c26` | 2025-12-10 | 2762 | [Link](https://github.com/libjpeg-turbo/libjpeg-turbo) |
| libs/jhead | `libs/jhead` | 3.08-2-gdb00efb | `db00efb` | 2025-05-07 | 484 | [Link](https://github.com/Matthias-Wandel/jhead) |
| libs/libexif | `libs/libexif` | libexif-0_6_25-release-37-gcdfe2db | `cdfe2db` | 2025-12-18 | 1477 | [Link](https://github.com/libexif/libexif) |
| libs/getopt-win32 | `libs/getopt-win32` | - | `b3de5c2` | 2017-08-30 | 3 | [Link](https://github.com/nanoporetech/getopt-win32) |
| libs/raylib | `libs/raylib` | 5.5-1510-g8f834604 | `8f834604` | 2025-12-28 | 9288 | [Link](https://github.com/raysan5/raylib) |
| libs/lvgl | `libs/lvgl` | v9.3.0-805-g18354afc0 | `18354afc0` | 2025-12-27 | 12135 | [Link](https://github.com/lvgl/lvgl) |
| libs/getopt-win | `libs/getopt-win` | v2.42.0-5-g36e55dd | `36e55dd` | 2025-10-21 | 33 | [Link](https://github.com/ludvikjerabek/getopt-win) |
| libs/libunistd | `libs/libunistd` | v1.2-170-g18b4e78 | `18b4e78` | 2025-10-19 | 522 | [Link](https://github.com/robinrowe/libunistd) |
| libs/ShazamAPI | `libs/ShazamAPI` | - | `ff8f89d` | 2024-05-15 | 30 | [Link](https://github.com/Numenorean/ShazamAPI) |
| libs/seek-tune | `libs/seek-tune` | - | `a6507e8` | 2025-11-19 | 337 | [Link](https://github.com/cgzirim/seek-tune) |
| libs/ShazamIO | `libs/ShazamIO` | 0.8.1 | `b5321b5` | 2025-06-11 | 315 | [Link](https://github.com/shazamio/ShazamIO) |
| libs/audio-recognizer | `libs/audio-recognizer` | - | `3028b61` | 2016-03-16 | 10 | [Link](https://github.com/wsieroci/audio-recognizer) |
| libs/pHash | `libs/pHash` | - | `3a70447` | 2021-09-19 | 54 | [Link](https://github.com/nihas101/pHash) |
| libs/image-hash | `libs/image-hash` | - | `34f0a00` | 2021-07-26 | 212 | [Link](https://github.com/ytetsuro/image-hash) |
| libs/imghash-viewer | `libs/imghash-viewer` | v0.1.0-3-g8b72a9f | `8b72a9f` | 2024-04-28 | 28 | [Link](https://github.com/YannickAlex07/imghash-viewer) |
| libs/imagehash | `libs/imagehash` | v1.0.7-3-g1a775c3 | `1a775c3` | 2024-03-30 | 43 | [Link](https://github.com/vitali-fedulov/imagehash) |
| libs/securecopy | `libs/securecopy` | v9.7.0 | `a82171b` | 2025-08-14 | 28 | [Link](https://github.com/hansij66/securecopy) |
| libs/xattrs | `libs/xattrs` | - | `9fd00c6` | 2019-08-23 | 44 | [Link](https://github.com/hrbrmstr/xattrs) |
| libs/fuse_xattrs | `libs/fuse_xattrs` | v0.3-7-gd1e3046 | `d1e3046` | 2021-12-02 | 36 | [Link](https://github.com/fbarriga/fuse_xattrs) |
| libs/xattrlib | `libs/xattrlib` | - | `286d38b` | 2011-06-03 | 5 | [Link](https://github.com/amdf/xattrlib) |
| libs/RenStrm | `libs/RenStrm` | 1.0-2-g3874e39 | `3874e39` | 2018-06-13 | 7 | [Link](https://github.com/hernandp/RenStrm) |
| libs/metastore | `libs/metastore` | v1.1.2-25-g8f0dd4c | `8f0dd4c` | 2023-02-01 | 174 | [Link](https://github.com/przemoc/metastore) |
| libs/lsads | `libs/lsads` | 2013-12-29_release-19-ge2d1599 | `e2d1599` | 2023-10-02 | 29 | [Link](https://github.com/assarbad/lsads) |
| libs/Powershell-ADS | `libs/Powershell-ADS` | - | `5227a03` | 2020-01-07 | 3 | [Link](https://github.com/phlashko/Powershell-ADS) |
| libs/SharpADS | `libs/SharpADS` | v1.0-3-gea6d0b9 | `ea6d0b9` | 2023-08-25 | 12 | [Link](https://github.com/ricardojoserf/SharpADS) |
| libs/ADSIdentifier | `libs/ADSIdentifier` | - | `099e808` | 2025-03-18 | 22 | [Link](https://github.com/HannahVernon/ADSIdentifier) |
| libs/ads | `libs/ads` | - | `87fc3aa` | 2019-04-14 | 3 | [Link](https://github.com/katahiromz/ads) |
| libs/AlternateDataStreams | `libs/AlternateDataStreams` | - | `34cd1cb` | 2016-10-31 | 7 | [Link](https://github.com/narasimha14/AlternateDataStreams) |
| libs/DataStreamBrowser | `libs/DataStreamBrowser` | - | `88db018` | 2020-08-19 | 1 | [Link](https://github.com/Thibstars/DataStreamBrowser) |
| libs/libevent | `libs/libevent` | release-2.2.1-alpha-298-ga994a52d | `a994a52d` | 2025-11-22 | 5129 | [Link](https://github.com/libevent/libevent) |
| libs/xxHash | `libs/xxHash` | v0.7.4-998-g6697932 | `6697932` | 2025-12-17 | 2052 | [Link](https://github.com/Cyan4973/xxHash) |
| libs/libgit2 | `libs/libgit2` | v0.16.0-14146-g3ac4c0adb | `3ac4c0adb` | 2025-12-20 | 16074 | [Link](https://github.com/libgit2/libgit2) |
| libs/dokany | `libs/dokany` | v2.3.1.1000-2-gead52a9 | `ead52a9` | 2025-12-11 | 1756 | [Link](https://github.com/dokan-dev/dokany) |
| libs/pngquant | `libs/pngquant` | 3.0.3-8-ge3bdc7c | `e3bdc7c` | 2025-07-07 | 1208 | [Link](https://github.com/kornelski/pngquant) |
| libs/dunst | `libs/dunst` | v1.13.0-26-ge2593d3 | `e2593d3` | 2025-11-21 | 2584 | [Link](https://github.com/dunst-project/dunst) |
| libs/audiocraft | `libs/audiocraft` | v1.3.0-70-g896ec7c | `896ec7c` | 2025-03-13 | 253 | [Link](https://github.com/facebookresearch/audiocraft) |
| libs/hashingImage | `libs/hashingImage` | - | `f00adc9` | 2017-05-08 | 2 | [Link](https://github.com/goongong/hashingImage) |
| libs/util-linux | `libs/util-linux` | v2.42-start-1044-ge4656fa97 | `e4656fa97` | 2025-12-17 | 21447 | [Link](https://github.com/util-linux/util-linux) |
| libs/libimghash | `libs/libimghash` | - | `ec97409` | 2014-01-31 | 40 | [Link](https://github.com/shuttie/libimghash) |
| libs/perceptual-dct-hash | `libs/perceptual-dct-hash` | - | `9646476` | 2019-06-04 | 28 | [Link](https://github.com/alangshur/perceptual-dct-hash) |
| libs/pHash.c | `libs/pHash.c` | v1.1.0-7-g1d7c056 | `1d7c056` | 2025-02-08 | 31 | [Link](https://github.com/folksable/pHash.c) |
| libs/tinyphash | `libs/tinyphash` | - | `7c7c503` | 2020-09-24 | 14 | [Link](https://github.com/Wassasin/tinyphash) |
| libs/ADSman | `libs/ADSman` | - | `187e647` | 2025-06-18 | 2 | [Link](https://github.com/8gudbits/ADSman) |
| libs/ADSFileSystem | `libs/ADSFileSystem` | - | `60b6710` | 2016-06-09 | 9 | [Link](https://github.com/marcelogm/ADSFileSystem) |
| libs/Bringing-Old-Photos-Back-to-Life | `libs/Bringing-Old-Photos-Back-to-Life` | v1.0-24-g33875ec | `33875ec` | 2022-07-23 | 108 | [Link](https://github.com/microsoft/Bringing-Old-Photos-Back-to-Life) |
| vcpkg | `vcpkg` | 2022.02.02-11909-g55fab67aea | `55fab67` | 2025-12-29 | 27312 | [Link](https://github.com/microsoft/vcpkg) |
