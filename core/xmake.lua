target("core")
    set_kind("static")
    add_files("src/*.cpp")
    add_headers("(core_include/*.h)")
    add_includedirs(".")
    if is_plat("windows") then
        add_files("src/adapter/*_win.cpp")
    else
        add_files("src/adapter/*_linux.cpp")
    end
