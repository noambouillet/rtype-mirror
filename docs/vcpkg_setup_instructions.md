## Dependency Management with Vcpkg

This project uses [Vcpkg](https://vcpkg.io/) in **Manifest Mode** to manage C++ dependencies (SFML and Asio).
We chose Vcpkg because it offers excellent native compatibility with both Windows (MSVC) and Linux, integrates seamlessly with CMake via a *toolchain* file, and features a robust binary caching system which is essential for preserving our Continuous Integration (CI) build quotas.

### Prerequisites

- **Git**
- **CMake** (version 3.15 or higher)
- A C++ compiler (GCC/Clang on Linux, MSVC on Windows)
- (Linux only) Basic development tools: 
  ```bash
  sudo apt-get install curl zip unzip tar pkg-config build-essential libx11-dev libxcursor-dev libxi-dev libgl1-mesa-dev libfontconfig1-dev libudev-dev
  ```

### Setup and Build

**1. Clone the project with its submodules**
Vcpkg is included as a Git submodule. When cloning the repository, make sure to fetch it:
```bash
git clone --recursive git@github.com:EpitechPGE3-2026/G-ING-500-LIL-5-1-rtype-1.git r-type
cd r-type
```
*(If you already cloned the repository without the `--recursive` flag, simply run `git submodule update --init --recursive` from the root of the project)*

**2. Bootstrap Vcpkg**
Before the first use, Vcpkg needs to compile its own executable. Run the script corresponding to your operating system:
- **On Linux:**
  ```bash
  ./vcpkg/bootstrap-vcpkg.sh
  ```
- **On Windows:**
  ```cmd
  .\vcpkg\bootstrap-vcpkg.bat
  ```

**3. Generate the project with CMake**
When calling CMake, we must provide the Vcpkg toolchain file. Vcpkg will automatically read the `vcpkg.json` file, download, and build the missing dependencies (this step may take a few minutes the first time).
```bash
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake
```

**4. Build the project**
Once the CMake generation is complete, start the compilation:
```bash
cmake --build build
```
The build creates both `r-type_server` and `r-type_client`. On Linux, they are `build/r-type_server` and `build/r-type_client`; with the default Visual Studio generator on Windows, they are under `build/Debug/`. The client uses SFML 3.0.2, installed by vcpkg during configuration.

If reusing a build directory configured with the former optional client feature, remove its cached switches once with `cmake -S . -B build -U VCPKG_MANIFEST_FEATURES -U RTYPE_BUILD_CLIENT` before rebuilding.

**5. Run and verify the server**

The server listens on UDP port 6767 by default. Pass a different port as its only argument:

```bash
./build/r-type_server 6767
```

On Windows, run `build\Debug\r-type_server.exe 6767` (or use the output path for your chosen CMake generator). Stop it with Ctrl+C. The server has no graphical interface. It currently accepts joins, disconnects, and input commands, and sends protocol errors and departure notices. Its fixed tick does not run gameplay or send snapshots; that requires the planned ECS integration. Clients use the binary protocol in [RFC.md](RFC.md).

Run the graphical client from the build directory against a local server with `cd build && ./r-type_client`. The build copies `assets/` beside the executable, including for Visual Studio configurations such as `build/Debug/`; run the executable from that directory so its relative `assets/` path resolves. Rebuild after changing asset files to refresh the copied directory. The client loads the checkerboard fallback and required player sheet before opening the window. The player ship comes from `r-typesheet1.gif`; its sheet name and pixel region are defined in `src/client/SFML/SpriteRegions.hpp`. Its asset manager caches each sheet as one texture and uses pixel rectangles to select sprites. A later missing sheet uses `magenta-checkerboard-200px.png` at the requested sprite size and reports the failed path once. Keep the manager alive while its sprites are in use. The client connects to `localhost:6767` by default. Pass `[host] [port]` or just a port to override these values; `-h` prints help. Arrow keys change the input state and Space sends Fire; a connected joystick also supplies movement and Fire. The visible sprite moves locally until the server sends world state. Use `--log-file=client.log` to append this process's logs to a file instead of stdout. Ctrl+C or closing the window sends `DISCONNECT`.
