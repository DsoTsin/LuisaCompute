if get_config("_lc_vk_sdk_dir") then
    target("volk")
    set_kind("headeronly")
    add_includedirs(".", {
        public = true
    })
    add_defines("VK_NO_PROTOTYPES", {
        public = true
    })
    on_load(function(target)
        if is_plat("windows") then
            local sdk_dir = get_config("_lc_vk_sdk_dir")
            target:add("includedirs", path.join(sdk_dir, "Include"), {
                public = true
            })
        end
    end)
    target_end()
end
