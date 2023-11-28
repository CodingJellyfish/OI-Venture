set_project("KHFanClub")
set_version("0.0.1")

add_rules("mode.debug", "mode.release")

includes("lib/*")

target("KHFanClub")
    
    set_kind("binary")
    add_files("src/**.c")
    add_includedirs("include", { public = true })
    add_deps("flecs")
    add_deps("sokol")

    if is_mode("debug") then
        add_defines("KH_DEBUG")
    elseif is_mode("release") then
        add_defines("KH_RELEASE")
    end

target_end()