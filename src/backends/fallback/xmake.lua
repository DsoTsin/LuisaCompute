add_requires("embree")

target("lc-backend-fallback")
_config_project({
    project_kind = "shared",
    batch_size = 8
})
add_deps("lc-runtime", "lc-vstl")
add_files("*.cpp")
add_deps(
    "lc-vulkan-swapchain",
    "lc-xir"
)
add_packages("embree")

add_defines("LUISA_COMPUTE_FALLBACK_VERSION=4")
-- TODO: if UNIX NOT APPLE
-- link to llvm libraries
on_load(function(target, opt)
    local libs = {}
    -- local llvm_path = get_config("llvm_path")
    local llvm_path = get_config("fallback_llvm_path")
    target:add("linkdirs", path.join(llvm_path, "lib"))
    target:add("includedirs", path.join(llvm_path, "include"))
    -- LLVM 19.1.7 Available Libraries
    libs = {
        "LLVMDemangle",
        "LLVMSupport",
        "LLVMTableGen",
        "LLVMTableGenBasic",
        "LLVMTableGenCommon",
        "LLVMCore",
        "LLVMFuzzerCLI",
        "LLVMFuzzMutate",
        "LLVMFileCheck",
        "LLVMInterfaceStub",
        "LLVMIRPrinter",
        "LLVMIRReader",
        "LLVMCodeGen",
        "LLVMSelectionDAG",
        "LLVMAsmPrinter",
        "LLVMMIRParser",
        "LLVMGlobalISel",
        "LLVMCodeGenData",
        "LLVMCodeGenTypes",
        "LLVMBinaryFormat",
        "LLVMBitReader",
        "LLVMBitWriter",
        "LLVMBitstreamReader",
        "LLVMDWARFLinker",
        "LLVMDWARFLinkerClassic",
        "LLVMDWARFLinkerParallel",
        "LLVMExtensions",
        "LLVMFrontendDriver",
        "LLVMFrontendHLSL",
        "LLVMFrontendOpenACC",
        "LLVMFrontendOpenMP",
        "LLVMFrontendOffloading",
        "LLVMTransformUtils",
        "LLVMInstrumentation",
        "LLVMAggressiveInstCombine",
        "LLVMInstCombine",
        "LLVMScalarOpts",
        "LLVMipo",
        "LLVMVectorize",
        "LLVMObjCARCOpts",
        "LLVMCoroutines",
        "LLVMCFGuard",
        "LLVMHipStdPar",
        "LLVMLinker",
        "LLVMAnalysis",
        "LLVMLTO",
        "LLVMMC",
        "LLVMMCParser",
        "LLVMMCDisassembler",
        "LLVMMCA",
        "LLVMObjCopy",
        "LLVMObject",
        "LLVMObjectYAML",
        "LLVMOption",
        "LLVMRemarks",
        "LLVMDebuginfod",
        "LLVMDebugInfoDWARF",
        "LLVMDebugInfoGSYM",
        "LLVMDebugInfoLogicalView",
        "LLVMDebugInfoMSF",
        "LLVMDebugInfoCodeView",
        "LLVMDebugInfoPDB",
        "LLVMSymbolize",
        "LLVMDebugInfoBTF",
        "LLVMDWP",
        "LLVMExecutionEngine",
        "LLVMInterpreter",
        "LLVMJITLink",
        "LLVMMCJIT",
        "LLVMOrcJIT",
        "LLVMOrcDebugging",
        "LLVMOrcShared",
        "LLVMOrcTargetProcess",
        "LLVMRuntimeDyld",
        "LLVMTarget",
        "LLVMAArch64CodeGen",
        "LLVMAArch64AsmParser",
        "LLVMAArch64Disassembler",
        "LLVMAArch64Desc",
        "LLVMAArch64Info",
        "LLVMAArch64Utils",
        "LLVMARMCodeGen",
        "LLVMARMAsmParser",
        "LLVMARMDisassembler",
        "LLVMARMDesc",
        "LLVMARMInfo",
        "LLVMARMUtils",
        "LLVMX86CodeGen",
        "LLVMX86AsmParser",
        "LLVMX86Disassembler",
        "LLVMX86TargetMCA",
        "LLVMX86Desc",
        "LLVMX86Info",
        "LLVMSandboxIR",
        "LLVMAsmParser",
        "LLVMLineEditor",
        "LLVMProfileData",
        "LLVMCoverage",
        "LLVMPasses",
        "LLVMTargetParser",
        "LLVMTextAPI",
        "LLVMTextAPIBinaryReader",
        "LLVMDlltoolDriver",
        "LLVMLibDriver",
        "LLVMXRay","LLVMWindowsDriver","LLVMWindowsManifest","LTO","LLVMCFIVerify","LLVMDiff","LLVMExegesisX86","LLVMExegesisAArch64","LLVMExegesis","LLVM-C",
        "LLVMOptDriver",
        "Remarks" 
    }

    target:add("links", libs)
    target:add("deps", "lc-core", "lc-runtime", "lc-vstl")
    if is_plat("windows") then
        target:add("syslinks", "Version", "advapi32", "Shcore", "user32", "shell32", "Ole32", 'Ws2_32', {
            public = true
        })
    elseif is_plat("linux") then
        target:add("syslinks", "uuid")
    elseif is_plat("macosx") then
        target:add("frameworks", "CoreFoundation")
    end
end)

if is_plat("windows") then
    -- /wd4624 /wd4996 # do not complain about LLVM
    -- /fp:fast /fp:contract /fp:except- /ARCH:AVX2
    -- add_cxflags("/fp:fast", "/fp:contract", "/fp:except-", "/ARCH:AVX2")
    -- add_cxflags("/wd4624", "/wd4996")
    add_syslinks("ntdll", "kernel32", "user32", "gdi32", "winspool", "comdlg32", "advapi32", "shell32", "ole32", "oleaut32", "uuid", "odbc32", "odbccp32")
end 

target_end()