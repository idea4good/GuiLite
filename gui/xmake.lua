target("gui")
    set_kind("static")
    add_deps("core")
    add_files("src/*.cpp")
    add_headers("(gui_include/*.h)")
    add_includedirs(".")
