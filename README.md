# SokoSheep

soko_sheep is a sokoban game where you herd sheep

## Tools needed
Raylib v6.2 (Game library and utils)

w64devkit v2.10.1 (C compiler)

GPU driver with opengl 3.3 (raylib renders with opengl3.3)

Note: The raylib windows installed contains the w64devkit so it is the only thing needed to install

## Installation
1. Go to [raylib.com](https://www.raylib.com), download the installer and run it with the default location
2. Put the compiler on the PATH by following the instructions below

## Adding the compiler to the PATH
`gcc` and `make` live in `C:\raylib\w64devkit\bin` and need to be on the PATH for use

Run the following commands in powershell / windows terminal
```powershell
[Environment]::SetEnvironmentVariable("Path", "C:\raylib\w64devkit\bin;" + [Environment]::GetEnvironmentVariable("Path", "User"), "User")
```

## Build and Run
From the project folder:

```powershell
make run
```
or
```powershell
./build_run.bat
```