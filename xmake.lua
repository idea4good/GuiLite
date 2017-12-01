-- project
set_project("GuiLite")

-- version
set_version("1.0.1")
set_xmakever("2.1.6")

-- the debug or release mode
if is_mode("debug") then
    
    -- enable the debug symbols
    set_symbols("debug")

    -- disable optimization
    set_optimize("none")

    -- link libcmtd.lib
    if is_plat("windows") then 
        add_cxflags("-MTd") 
    end

elseif is_mode("release") then

    -- set the symbols visibility: hidden
    set_symbols("hidden")

    -- strip all symbols
    set_strip("all")

    -- enable fastest optimization
    set_optimize("fastest")

    -- link libcmt.lib
    if is_plat("windows") then 
        add_cxflags("-MT") 
    end
end

-- for windows
if is_plat("windows") then 
    add_cxxflags("-EHsc")
    add_defines("UNICODE", "_UNICODE")
    add_ldflags("-nodefaultlib:\"msvcrt.lib\"")
	add_links("ws2_32", "IPHlpApi", "kernel32", "user32", "gdi32")
end

-- include project sources
includes("core", "gui") 
