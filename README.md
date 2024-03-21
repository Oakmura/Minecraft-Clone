# Minecraft-Clone
This is an effort to make Minecraft from scratch with D3D11

## Table of Contents

- [Snapshots](#snapshots)
- [Getting Started](#Geting-Started)
	- [Downloading the repository](#Downloading-the-repository)
	- [Generating Project Files](#Generating-project-files)
	- [Building and Running](#Building-and-running)
- [Controls](#controls)
- [To-dos](#to-dos)
	- [Featues](#features)
	- [Fixes](#fixes)
- [P.S.](#ps)
- [Acknowledgements](#acknowledgements)

<a name = "snapshots"></a>
**Snapshot as of now**
![world_with_snow](https://github.com/Oakmura/Minecraft-Clone/assets/89961585/d36600fb-8d9c-461e-9b8c-81bf65f6c3a1)

**Cave system**
![cave](https://github.com/Oakmura/Minecraft-Clone/assets/89961585/df486057-1a03-4bd1-bf1a-6baea97fc04b)

**Underwater effect**
![underwater_effect](https://github.com/Oakmura/Minecraft-Clone/assets/89961585/878cbd1e-9936-4968-8e5c-8edd7c7068e6)

**Clouds**
![clouds](https://github.com/Oakmura/Minecraft-Clone/assets/89961585/d0a26c8e-7c0c-4535-9ee1-74a6eaefb724)

<a name = "Geting-Started"></a>
## Getting Started

<a name = "Downloading-the-repository"></a>
<ins>**1. Downloading the repository:**</ins>

Start by cloning the repository with `git clone --recursive https://github.com/Oakmura/Minecraft-Clone`.

If the repository was cloned non-recursively previously, use `git submodule update --init` to clone the necessary submodules.

<a name = "Generating-project-files"></a>
<ins>**2. Generating Project Files:**</ins>

Run the [GenerateProjects.bat](https://github.com/Oakmura/Minecraft-Clone/blob/main/scripts/GenerateProjects.bat) file found in `scripts` folder. This will run [premake5.lua](https://github.com/Oakmura/Minecraft-Clone/blob/main/premake5.lua) with predefined configuration settings to generate Visual Studio 2022 solution and project files.

If you want to generate project files other than VS2022, you can change the "vs2022" found in [GenerateProjects.bat](https://github.com/Oakmura/Minecraft-Clone/blob/main/scripts/GenerateProjects.bat) to other projects supported in [premake](https://premake.github.io/docs/Using-Premake/).

**Please note that if you have non-Ascii characters(i.e. Korean) in cloned path, it may not work due to lack of Unicode support in premake or lua in general.**

<a name = "Building-and-running"></a>
<ins>**3. Building And Running Files:**</ins>

It is recommended that you run **Distribution** mode. This is the most optimal build, which uses compiler optimization with logging functionality stripped and is without Console. 

However, in case of failures, try to run Debug mode. I implemented in such a way that if it works in Debug mode, it works in other modes. But, be aware that it takes a while (30s~1m) to generate and load map in Debug mode.

Build with Visual Studio and Run!

<a name = "controls"></a>
## Controls

| Button                        | Action                                                         |
|-------------------------------|----------------------------------------------------------------|
| Left mouse button             | Place/Remove block                                             |
| Right mouse button            | Change interaction mode                                        |
| W, A, S, D, Q, E              | Move                                                           |
| I                             | Toggle ImGui Window                                            |
| ESC                           | Exit                                                           |


<a name = "to-dos"></a>
## To-dos

<a name = "features"></a>
### Features
- Physics (gravity, collision detection)
- UI (crosshair, handmesh, user-interface slots)
- Animation (jump, attack)
- 3rd Person View Camera
- Custom Windows Title Bar

<a name = "fixes"></a>
### Fixes
- Water texture tearing
- Inaccurate raycasting (Block detection)

<a name = "ps"></a>
## P.S.
- There are still many things yet to implement and fix. But, for anyone who wishes to play with it, feel free to fork, make an issue, or pull request :)

<a name = "acknowledgment"></a>
## Acknowledgments

This project is built on top of [Creating a Voxel Engine in Python](https://www.youtube.com/watch?v=Ab8TOSFfNp4).  Thank you to [Coder Space](https://www.youtube.com/watch?v=Ab8TOSFfNp4) for sharing such wonderful contents.
