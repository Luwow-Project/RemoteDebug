# RemoteDebug
Remote debugger for the Luwow-Project.

This repository is not stand-alone. You must build it externally from within another
repository like https://github.com/Luwow-Project/Release

## Project Overview

This project contains the source for the Luwow Remote-Debug, which can be ran using the `remotedebug` executable.

Upon running, it will create a local server, which by using your IDE's debugger, can be connected to. After connecting, you should be able to see the details of your main Luau thread's stack.

## Adding to your Project

Use the git submodule add command to add the following submodules to your project:
```bash
git submodule add https://github.com/luau-lang/luau extern/luau
git submodule add https://github.com/Luwow-Project/RemoteDebug/ extern/debug
git submodule add https://github.com/sssooonnnggg/cppdap extern/cppdap
git submodule add https://github.com/sssooonnnggg/luau-debugger extern/luau-debugger
git submodule add https://github.com/Luwow-Project/GUI libraries/gui
```

In your `.gitmodules` file, the submodules should look something like:

```
[submodule "extern/debug"]
	path = extern/debug
	url = https://github.com/Luwow-Project/RemoteDebug/
[submodule "extern/luau"]
	path = extern/luau
	url = https://github.com/luau-lang/luau
[submodule "extern/cppdap"]
	path = extern/cppdap
	url = https://github.com/sssooonnnggg/cppdap
[submodule "extern/luau-debugger"]
	path = extern/luau-debugger
	url = https://github.com/sssooonnnggg/luau-debugger
[submodule "libraries/gui"]
	path = libraries/gui
	url = https://github.com/Luwow-Project/GUI
```

After adding these submodules, you must then add these fields to your `CMakeLists.txt` file:

```
set(CPP_DAP_ROOT ${CMAKE_SOURCE_DIR}/extern/cppdap)
set(LUAU_DEBUGGER_ROOT ${CMAKE_SOURCE_DIR}/extern/luau-debugger)
set(DEBUGGER_ROOT ${CMAKE_SOURCE_DIR}/extern/debug/src)

add_subdirectory(${CPP_DAP_ROOT})
add_subdirectory(${LUAU_DEBUGGER_ROOT}/debugger)
add_subdirectory(${DEBUGGER_ROOT})
```

Next, you have to configure your IDE to launch a debugger instance. Examples of how to do so are shown below.

### Visual Studio Code Configuration

In Visual Studio Code, you can configure your `launch.json` file like below:

```json
{
    "configurations": [
        {
            "name": "attach to luau debugger",
            "type": "luau",
            "request": "attach",
            "address": "localhost",
            "port": 59000
        }
    ]
}
```

-----

After configuration, you can then proceed with the building process. When the building process finishes, you can start using the debugger executable.

To learn more information about the building process, you can head over to the [documentation](https://luwow-project.github.io/Documentation/), or you can check out the [official release project](https://github.com/Luwow-Project/Release) of Luwow.