target("flecs")
    
    set_kind("static")
    add_files("src/**.c")
    add_headerfiles("src/**.h")
    add_includedirs("include", { public = true })

    if is_os("windows") then
        add_links("wsock32", "ws2_32")
    end

    if is_os("linux") then
        add_links("pthread")
    end

    if is_mode("debug") then
        add_defines("FLECS_DEBUG")
    elseif is_mode("release") then
        add_defines("FLECS_NDEBUG")
    end

target_end()