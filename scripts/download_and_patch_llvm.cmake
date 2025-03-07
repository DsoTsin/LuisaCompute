option(LUISA_COMPUTE_DOWNLOAD_LLVM "Download and patch LLVM if not found" OFF)

if (LUISA_COMPUTE_DOWNLOAD_LLVM)
    set(LLVM_DOWNLOAD_VERSION "19.1.5")
    set(LLVM_WIN_BINARY_URL "https://github.com/llvm/llvm-project/releases/download/llvmorg-${LLVM_DOWNLOAD_VERSION}/clang+llvm-${LLVM_DOWNLOAD_VERSION}-x86_64-pc-windows-msvc.tar.xz")
    message(STATUS "LLVM not found. Downloading official prebuilt binaries ${LLVM_DOWNLOAD_VERSION} from ${LLVM_WIN_BINARY_URL}.")
    include(FetchContent)
    FetchContent_Declare(
            llvm
            URL ${LLVM_WIN_BINARY_URL}
    )
    FetchContent_MakeAvailable(llvm)

    set(LLVM_DIR ${llvm_SOURCE_DIR}/lib/cmake/llvm CACHE PATH "Path to LLVMConfig.cmake" FORCE)
    find_package(LLVM REQUIRED CONFIG PATHS ${LLVM_DIR})
else ()
    message(WARNING "LLVM not found. Please either set `LLVM_DIR` to the directory containing 'LLVMConfig.cmake' or set `LUISA_COMPUTE_DOWNLOAD_LLVM=ON` to let LuisaCompute download it for you.")
endif ()
