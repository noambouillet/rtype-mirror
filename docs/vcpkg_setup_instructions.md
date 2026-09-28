## Dependency Management with Vcpkg

This project uses [Vcpkg](https://vcpkg.io/) in **Manifest Mode** to manage C++ dependencies (SFML, Asio, GTest). 
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
The generated executables (`r-type_server`, `r-type_client`) will be located in the `build/` directory.