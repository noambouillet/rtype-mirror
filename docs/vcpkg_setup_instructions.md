## Dependency Management with Vcpkg

This project uses [Vcpkg](https://vcpkg.io/) in **Manifest Mode** to manage C++ dependencies (SFML, Asio, GTest). 
We chose Vcpkg because it offers excellent native compatibility with both Windows (MSVC) and Linux, integrates seamlessly with CMake via a *toolchain* file, and features a robust binary caching system which is essential for preserving our Continuous Integration (CI) build quotas.

### Prerequisites

#### Windows

Install the following tools:

1. **Git** — [Download Git](https://git-scm.com/downloads)
2. **Visual Studio 2026 Community** — [Download Visual Studio](https://visualstudio.microsoft.com/downloads/)
3. **CMake 3.21 or newer** — included with the Visual Studio C++ development tools, or available from [cmake.org](https://cmake.org/download/)

During Visual Studio installation, open the Visual Studio Installer and select the **Desktop development with C++** workload.

Make sure the following components are installed:

- MSVC C++ compiler and build tools
- Windows SDK
- C++ CMake tools for Windows
- Ninja build system (recommended for alternative CMake generators)

Visual Studio provides the compiler, CMake integration, and debugging tools needed for the project.

After installation, open PowerShell and verify:

```powershell
git --version
cmake --version
```

#### Linux (Ubuntu/Debian)

Install the compiler, CMake, Ninja, Git, and required system libraries:

```bash
sudo apt-get update

sudo apt-get install -y \
    git cmake ninja-build curl zip unzip tar \
    pkg-config build-essential \
    libx11-dev libxcursor-dev libxi-dev \
    libgl1-mesa-dev libfontconfig1-dev \
    libudev-dev
```

Some Linux distributions may require additional development packages for SFML.

Verify:

```bash
git --version
cmake --version
g++ --version
ninja --version
```

**Note:** CMake 3.21 or newer is required for the provided presets. Some older Linux distributions may ship an earlier version.

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

This downloads or prepares the Vcpkg executable. It does not yet build all project dependencies.

Verify that Vcpkg works:

**Windows:**

```powershell
.\vcpkg\vcpkg.exe version
```

**Linux:**

```bash
./vcpkg/vcpkg version
```

**3. Generate the project with CMake**
When calling CMake, we must provide the Vcpkg toolchain file. Vcpkg will automatically read the `vcpkg.json` file, download, and build the missing dependencies (this step may take a few minutes the first time).
```bash
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake
```

On windows, run the following:

```powershell
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE="$PWD/vcpkg/scripts/buildsystems/vcpkg.cmake"
```

**4. Build the project**
Once the CMake generation is complete, start the compilation:
```bash
cmake --build build
```
The server executable is `build/r-type_server` on Linux and `build/Debug/r-type_server.exe` with the default Visual Studio generator on Windows. The client target will be available when its source tree is added. To install its SFML dependency, configure with `-DVCPKG_MANIFEST_FEATURES=client`.

**5. Run and verify the server**

The server listens on UDP port 6767 by default. Pass a different port as its only argument:

```bash
./build/r-type_server 6767
```

On Windows, run `build\Debug\r-type_server.exe 6767` (or use the output path for your chosen CMake generator). Stop it with Ctrl+C. The server has no graphical interface. It currently accepts joins, disconnects, and input commands, and sends protocol errors and departure notices. Its fixed tick does not run gameplay or send snapshots; that requires the planned ECS integration. Clients use the binary protocol in [RFC.md](RFC.md).

The UDP smoke test requires Python 3 and local UDP socket access:

```bash
ctest --test-dir build --output-on-failure
```
