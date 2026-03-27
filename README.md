# RemoteDebug
Remote debugger for the Luwow-Project.

This repository is not stand-alone. You must build it externally from within another
repository like https://github.com/Luwow-Project/Release

## Project Overview

This project contains the source for the Luwow Remote-Debug, which can be ran using the `remotedebug` executable.

Upon running, it will create a local server, which by using your IDE's debugger, can be connected to. After connecting, you should be able to see the details of your main Luau thread's stack.

### Visual Studio Code Configuration

In Visual Studio Code, you can configure your `launch.json` file like below to use it.

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