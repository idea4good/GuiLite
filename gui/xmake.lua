target("gui")
    set_kind("static")
    add_files("src/*.cpp")
    add_includedirs("core_include", "gui_include")
