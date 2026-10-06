#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <debugger.h>
#include "luacode.h"
#include "Engine.h"

using Engine = Luwow::Engine::Engine;
using Package = Luwow::Engine::Package;
using Message = Luwow::Engine::Message;

// Handles compiler-compile: compiles the script at message.data and replies with its bytecode
void compileRequest(void* context, Message& message) {
    std::filesystem::path modulePath(message.data);
    std::ifstream file(modulePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open script file: " + modulePath.string());
    }
    std::string script(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );
    file.close();
    
    std::cout << "[Debugger] Compiling \"" << modulePath << "\"" << std::endl;

    lua_CompileOptions options = {};
    options.optimizationLevel = 1;
    options.debugLevel = 2;
    options.typeInfoLevel = 1;
    options.coverageLevel = 0;
    options.vectorLib = nullptr;
    options.vectorCtor = nullptr;
    options.vectorType = nullptr;
    options.mutableGlobals = nullptr;
    options.userdataTypes = nullptr;
    options.librariesWithKnownMembers = nullptr;
    options.libraryMemberConstantCb = nullptr;
    options.libraryMemberTypeCb = nullptr;
    options.disabledBuiltins = nullptr;

    size_t bytecodeSize = 0;
    char* bytecode = luau_compile(script.c_str(), script.length(), &options, &bytecodeSize);
    if (!bytecode) {
        throw std::runtime_error("Failed to compile script: " + modulePath.string());
    }
    message.data = std::string(bytecode, bytecodeSize);
    free(bytecode);
}

luau::debugger::Debugger* pDebugger = nullptr;

void debuggerRequest(void* context, Message& message)
{
    if (!pDebugger) return;
    std::filesystem::path filePath = (std::filesystem::current_path() / std::filesystem::path(message.data)).lexically_normal();
    std::cout << "[Debugger] Loading file \"" << message.data << "\" and naming it \"" << filePath.string() << "\"" << std::endl;
    pDebugger->onLuaFileLoaded(message.state, filePath.string(), true);
};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: remotedebug <port> <script.luau>" << std::endl;
        std::cerr << "Note that port 59000 is recommended." << std::endl;
        return 1;
    }

    int remoteDebuggerPort = std::atoi(argv[1]);

    // Initialize the luau-debugger component
    auto log_handler = [](std::string_view msg) {
        printf("%s", msg.data());
    };
    auto error_handler = [](std::string_view msg) {
        fprintf(stderr, "%s", msg.data());
    };
    luau::debugger::log::install(log_handler, error_handler);    
    luau::debugger::Debugger debugger(true);
    pDebugger = &debugger;

    try
    {
        std::filesystem::path filePath = std::filesystem::path(argv[2]);
        if (filePath.is_relative()) {
            filePath = (std::filesystem::current_path() / filePath).lexically_normal();
        }
        
        // Initialize the engine
        Engine engine((Package()), filePath);
        engine.handle(Luwow::Engine::Topics::CompilerCompile, compileRequest, nullptr);
        engine.handle(Luwow::Engine::Topics::DebuggerLoad, debuggerRequest, nullptr);
        engine.initialize(argc, argv);

        debugger.initialize(engine.getMainState());
        debugger.listen(remoteDebuggerPort);

        engine.run();

        debugger.stop();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    pDebugger = nullptr;
    return 0;
}
