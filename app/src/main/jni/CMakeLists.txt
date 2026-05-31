cmake_minimum_required(VERSION 3.22.1)

project(IlvMagic)

# ============================================================
# GLOBAL FLAGS
# ============================================================

add_compile_options(
    -Wno-error=format-security
    -fvisibility=hidden
    -ffunction-sections
    -fdata-sections
    -w
    -fno-rtti
    -fno-exceptions
    -fpermissive
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_SHARED_LINKER_FLAGS
    "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--gc-sections,--strip-all"
)

# ============================================================
# PREBUILT STATIC LIBRARIES
# ============================================================

# dobby
add_library(libdobby STATIC IMPORTED)
set_target_properties(libdobby PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/Dobby/libraries/${ANDROID_ABI}/libdobby.a"
)

# ctorHook (FIXED)
add_library(libctorHook STATIC IMPORTED)
set_target_properties(libctorHook PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/ctorHook/libs/${ANDROID_ABI}/libctorHook.a"
)

# ============================================================
# SOURCE FILES
# ============================================================

file(GLOB_RECURSE IMGUi_SRC "${CMAKE_SOURCE_DIR}/Imgui/*.cpp")
file(GLOB_RECURSE IL2CPP_SRC "${CMAKE_SOURCE_DIR}/Il2Cpp/*.cpp")
file(GLOB_RECURSE KITTYMEM_SRC "${CMAKE_SOURCE_DIR}/KittyMemory/*.cpp")
file(GLOB_RECURSE LIBZIP_SRC "${CMAKE_SOURCE_DIR}/libzip/*.c")

file(GLOB MAIN_SRC "${CMAKE_SOURCE_DIR}/*.cpp" "${CMAKE_SOURCE_DIR}/*.c")

# ============================================================
# MAIN LIBRARY
# ============================================================

add_library(IlvMagic SHARED
    ${IMGUi_SRC}
    ${IL2CPP_SRC}
    ${KITTYMEM_SRC}
    ${MAIN_SRC}
    ${LIBZIP_SRC}
)

# ============================================================
# INCLUDE DIRECTORIES
# ============================================================

target_include_directories(IlvMagic PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/libzip
    ${CMAKE_SOURCE_DIR}/foxcheats/includes
    ${CMAKE_SOURCE_DIR}/IL2CppSDKGenerator/BasicStructs
)

# ============================================================
# LINK LIBRARIES
# ============================================================

target_link_libraries(IlvMagic
    libdobby
    libctorHook
    log
    android
    z
    EGL
    GLESv3
    GLESv2
)