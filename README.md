# HAL
Heavy ion Analysis Libraries

Framework for analysis of the collisions of heavy ions. 

## ⚙️ Prerequisites
This software requires:
 
- [ROOT 6](https://root.cern/)
- [GSL (GNU Scientific Library)](https://www.gnu.org/software/gsl/)
- cmake 3.11 or never
- (optional) Python 3 + `http.server` module (for interactive reports)
 
---

## 🐳 Installation

### Docker

You can use pre-built Docker images:

```bash
docker pull ghcr.io/danielwielanek/root-container:latest
```

Then run the container:

```bash
docker run --rm -it ghcr.io/danielwielanek/root-container:latest
```

> ⚠️ Note: This container is based on the latest main branch.

### Building from source code

1. Create a build directory:

```bash
cd HAL
mkdir build
cd build
```

2. Run CMake and install [N - number of cores for compilation]:

```bash
cmake -DCMAKE_INSTALL_PREFIX=[install_path] ..
make -j[N] && make install
```

Example:

```bash
cmake -DCMAKE_INSTALL_PREFIX=../inst ..
make -j8 && make install
```

## 🚀 Usage

To use HAL, source the configuration script:

```bash
source [INSTALL_DIRECTORY]/bin/hal_config.sh
```

> ⚠️ Note: sourcing should be done once (per session/terminal)


## ⚙️ Additional CMake Options

> ℹ️ Use -D[OPTION]=[VALUE] with cmake to enable features.

For Typical Users
 * **EXAMPLES=ON** – enables example generators (Unigen, OTF)

For Advanced Use
 * **INCLUDE_HAL_SUBDIR=TRUE** – install headers in Hal/ subdirectory (for use as a dependency, e.g., in CbmROOT)
 * **JSROOT_DIR=[path]** – specify custom JSROOT directory if ROOT's version causes issues

If Compilation Fails
 * **CMAKE_CXX_STANDARD=[XX]** – manually set C++ standard (default is 17).
 * **ROOTSYS=[path]** – manually specify ROOT path
 * **GLS_DIR=[path]** – manually specify GSL path

>   ⚠️ Note: to check ROOT cxx standard version use command:

```bash
root-config --cflags
```

if returns -std=c++17 your standard is 17

📦 Applications

This software includes several helper tools:

 * **hal-report** – generates HTML reports (requires local server for JSROOT, e.g. python3 -m http.server)
 * **hal-merger** – efficient merging of multiple HAL files
 * **hal-jobs** – submit cluster jobs without painful Bash scripting
 * **hal-cmake** – helper for building libraries/classes for cuts

📝 To Do
   * Simplify Buffer class (use pointers instead of int maps?)
   * Consider using std::function for custom class cuts
   * Add macro-level cut generator

📚 Documentation

    Page @subpage femto_analysis

    Page @subpage subpage_2


 * simplification of buffer class - using pointers instead of int maps?
 * std::fuction for custom class cuts?
 * removing duplicate cut monitors

