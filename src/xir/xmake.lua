target("lc-xir")
_config_project({
    project_kind = "shared",
    batch_size = 4
})
add_files("*.cpp")
add_files("instructions/*.cpp")
add_files("metadata/*.cpp") 
add_files("translators/*.cpp")
add_files("passes/*.cpp")
add_deps("lc-ast")
-- TODO yyjson to be removed
add_packages("yyjson")
--------------------------------------------------
add_defines("LUISA_ENABLE_XIR", { public = true })
add_defines("LC_XIR_EXPORT_DLL")
-- TODO: enable test
target_end()