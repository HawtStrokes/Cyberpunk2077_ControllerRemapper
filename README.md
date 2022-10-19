## Cyberpunk2077\_ControllerRemapper

Remap Controller Buttons for Cyberpunk 2077.

<img src="https://github.com/HawtStrokes/Cyberpunk2077_ControllerRemapper/blob/main/Resources/SampleImage.png" width=90% height=90%>

As of patch 1.6, there is virtually no concrete way to change the button layouts for controllers in the game. This tool allows the remapping of controller buttons to actions with a few extra:

1.  Button combos support
2.  Single Tap or Double Tap Mode
3.  Hold or Toggle Mode

It also provides persistence capability, allowing you to store button layouts on disk, so you can have multiple button layout presets you can pick and load. Resetting to defaults is also possible.

## **Sample**

![Cyberpunk2077_ControllerRemapper Gif Sample](/Resources/CyberpunkControllerRemapperSample.gif)

## **Build**

### Cloning

In a terminal, enter the following commands.

```plaintext
git clone --recursive https://github.com/HawtStrokes/Cyberpunk2077_ControllerRemapper.git
cd Cyberpunk2077_ControllerRemapper/External/
git submodule update --recursive --init
```

### GLFW

Use Cmake to generate the solution file. For ease of building, I suggest using cmake-gui. Open cmake-gui and point the source code to `<complete absolute path to>\Cyberpunk2077_ControllerRemapper\External\glfw` and set the binaries location to `<complete absolute path to>\Cyberpunk2077_ControllerRemapper\External\glfw\build`

Open the generated solution file in the build folder and build `Debug|x64` and `Release|x64` configurations.

### HawtLib

Open the solution file in the Cyberpunk2077\_ControllerRemapper root directory, and build `Release_DLL` and `Debug_DLL` configurations of HawtLib.

In file explorer, go to the following directories and open the appropriate batch files:

*   In`Cyberpunk2077_ControllerRemapper\External\HawtLib\HawtLib\Scripts` open `MakeIncludes.bat`
*   In`Cyberpunk2077_ControllerRemapper\ControllerRemapper\Scripts` open `CopyDLL.bat`

If all steps are followed correctly, you should then be able to build all configurations of Cyberpunk2077\_ControllerRemapper.

## **How It Works**

It takes the currently configured binds and builds minimized XML files associated with Input Mappings, which will replace the ones in the game folder.

## **Dependencies**

- https://github.com/glfw/glfw
- https://github.com/ocornut/imgui
- https://github.com/fmtlib/fmt
- https://github.com/HawtStrokes/HawtLib
