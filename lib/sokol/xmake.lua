target("sokol")
    
    set_kind("headeronly")
    add_defines("SOKOL_IMPL")
    add_includedirs("include", { public = true })

target_end()