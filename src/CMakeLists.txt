add_executable(Luwow.RemoteDebug)
target_include_directories(Luwow.RemoteDebug PRIVATE
    ${LUAU_ROOT}/VM/include
    ${LUAU_ROOT}/VM/src
    ${LUAU_ROOT}/Compiler/include
    ${LUAU_ROOT}/CLI
    ${LUAU_ROOT}/Config/include
    ${LUAU_DEBUGGER_ROOT}/debugger/include
    ${ENGINE_ROOT}
if(WIN32)
    ${GUI_ROOT}/src/Gui
    ${GUI_ROOT}/src/Gui/Windows
elseif(APPLE)
    ${GUI_ROOT}/src/Gui
    ${GUI_ROOT}/src/Gui/Mac
endif()
)
target_link_libraries(Luwow.RemoteDebug PRIVATE
    Luwow.Engine
    Luwow.Gui
    Luau.VM
    Luau.Compiler
    Luau.CLI.lib
    Luau.Config
    Luau.Debugger
)
set_target_properties(Luwow.RemoteDebug PROPERTIES OUTPUT_NAME remotedebug)
target_sources(Luwow.RemoteDebug PRIVATE main.cpp)