# General

Application for host communication with the self-balancing-bike microcontroller.

## About

SBB Host is a desktop application used for the communication with the self-balancing-bike microcontroller via USB. It can be used for signal streaming from the microcontroller, control of the motor drivers, and reading of the log files (having extension *.sbb).

## Files

Files:

* `SBBHost.sln`, `SBBHost.vcxproj`, `SBBHost.vcxproj.filters`,`SBBHost.vcxproj.user` ,`SBBHost.rc` ,`resource.h`: project files necessary for *Visual Studio* (in Windows only)
* `Makefile`: makefile necessary for compilation and installation in Linux
* `Doxyfile`: configuration file for documentation generation from source code using *doxygen*
* `configure`: configure script (to use before building or installation)
* `installation_path.txt`: installation path (updated by `configure`). Do not delete this file!

Code folders:

* `./include`: contains `.h` files included in source code
* `./src`: contains source `.c` or `.cpp` files, inclduing main file `main.cpp`
* `./lib`: contains a folder for each used library

Additional folders:

* `./resource`: contains additional resource files necessary for the application
* `./docs/html`: contains the documentation in html (generated automatically from source code using *doxygen*)
* `./bin`: contains binary files
* `./.build`: contains temporary files created during compilation
* `./.vscode`: contains VS code property file(s)

Installer files and folders:

* `./installer`: folder containing the installer files
* `./installer/installer.vdproj`: installer project necessary for *Visual Studio* (in Windows only)
* `./installer/win64`: contains the Windows x64 installer
* `./installer/linux`: contains the Linux installation files

Note that some of the above folders are created during building only.

## Prerequisites

Building and installation work on Windows x64 or Linux.

For only installation in Windows x64 no prerequisites are necessary: all binary files are provided with the installer. 

For Linux installation it is necessary to install

* *make*: this usually can be simple done by using in the terminal
  
  ```bash
  sudo pacman -Sy make
  ```
  
  \note The code above works with Arch Linux distributions, similar commands may be used in other Linux distributions (e.g. `apt install make` in Debian/Ubuntu).

* *gtkmm4* libraries, including all its dependencies

* *fftw3* libraries
  See *Installation of libraries* for details on the installation of libraries.

For building the code and the installer, the following prerequisites are necessary:

* *Visual Studio* (>=2017) for Windows, or alternatively [Mingw-w64](https://www.mingw-w64.org/) (e.g. with [MSYS2]([https://www.msys2.org/](https://www.msys2.org/)))
* *gtkmm4* libraries, including all its dependencies
* *fftw3* libraries for Linux and Mingw-w64, while libraries are already given in `./lib` for Windows using *Visual Studio*
* *g++* with *make* for Linux and Mingw-w64
* *doxygen* for documentation generations

For building in Linux or with Mingw-w64 it is racommended to have also *Visual Studio Code* with *C/C+ (ms-vscode)* installed. This is not mandatory for compilation but it helps for code completation and make the compilation simpler.

## Installation (user)

* ArchLinux: installation can be performed using the AUR with pacman. 

* For other distributions the installation is manual and involves:
  
  ```
  .\configure
  make build
  make install
  ```
  
  Use `make uninstall` for unistalling. Installation requires root permission. This actually adds the following files to the root
  
  * `/usr/local/bin/sbbhost`
  * `/usr/local/share/sbbhost-config.ini`
  * `/usr/local/share/sbbhost/sbbhost-toolbar.xml`
  * `/usr/local/share/doc/sbbhost/README.md`
  * `/usr/local/share/applications/sbbhost.desktop`
  * `/usr/local/share/pixmaps/sbbhost-icon.svg`
  
  You may perform local installation (i.e. not system-wide) using `.\configure --prefix $HOME/.local`.

* Windows x64: just use `./installer/win64/SBBHost Installer.msi` to install the application in Windows using a setup wizard.
  \attention It is highly racommended to not change the installation directory. However, if you want to change the installation directory during the setup wizard, you must change the variable `gsettings_dir` in the file `path_to_installation/sbbhost-config.ini` with
  
  ```ini
  gsettings_dir = 'path_to_installation\share';
  ```
  
  \note For Windows, building is not mandatory for installation, since pre-compiled binaries are provided.

## Configuration file

After installation, the configuration file `path/to/installation/sbbhost-config.ini` may be changed in order to configure the application with special features. These include:

* Default values for buadrate and timeout of the serial port
* Header and terminator bytes of the data packet
* Enable the darkmode on startup
* Modify the signal list
* Modify the control list and their upper, lower limits and default values
* Modify the default size of windows and dialogs, as well as the appearance of the graph

However, modifications should be done carefully by expert users. In Linux, the configuration file is in `/usr/share` (for ArchLinux) or `/usr/local/share`.

## Documentation (for development)

Documentation for the source code (but not for the used libraries in `./lib`) can be generated from the source using *doxygen* (see [https://www.doxygen.nl/download.html](https://www.doxygen.nl/download.html) for download) with

```shell
doxygen
```

or also using *make*

```shell
make doc
```

Generated documentation includes only a html version in `./docs/html`, with main fil `index.html`. Documentation is also available on [https://stefphd.github.io/SBBHost/](https://stefphd.github.io/SBBHost/). A latex documentation may be also generated by setting

```doxygen
GENERATE_LATEX  = YES
```

in `Doxyfile`. The latex version will be in `./docs/latex` and can be compiled using latex with

```shell
cd ./docs/latex
make
```

which generates the pdf file `./doc/latex/refman.pdf`. Note that this requires having latex installed.

### Installation of libraries (for development)

*gtkmm4* libraries (including all its dependencies) can be installed with:

* Linux:
  
  ```bash
  sudo pacman -Sy gtkmm-4.0
  ```
  
  \note The code above works with Arch Linux distributions, similar commands may be used in other Linux distributions.

* Windows: use *vcpkg* (see [https://github.com/microsoft/vcpkg](https://github.com/microsoft/vcpkg)) for installation with *Visual Studio*. Other actions may be necessary to make *Visual Studio* working properly. See *Workaround with gtkmm in Visual Studio* for details. 

*fftw3* library can be installed with:

* Linux:
  
  ```bash
  sudo pacman -Sy fftw
  ```
  
  \note The code above works with Arch Linux distributions, similar commands may be used in other Linux distributions.

* Windows: no installation is required, since pre-compiled binaries are provided.

Alternatively, for Windows you may also use [Mingw-w64](https://www.mingw-w64.org/) with [MSYS2](https://www.msys2.org/) instead of *Visual Studio*. This requires having the `mingw-w64-x86_64-gtkmm-4.0` and `mingw-w64-x86_64-fftw` packages installed.

## Building (for develpement)

* Linux and Mingw-w64: building makes use of a makefile. Before building the executable for standalone usage (i.e. without installation), it is necessary to run the configure with
  
  ```
  ./configure --prefix .
  ```
  
  Depending on what you would like to do, there are different options
  
  * Build the code: `make build` in the terminal (does not rebuild if files do not change)
  * Clean object files: `make clean`
  * Clean built files (including binaries): `make cleaner`
  * Rebuild: `make remake` (regardless if files have changed or not)
  * Build the code and generate the documentation: just use `make` or `make all`
  
  If installed, you may also use *Visual Studio Code* using `ctrl+shift+B` to build and generate the documentation (this corresponds to `make all`).

* Windows with *Visual Studio*: just use *Visual Studio* in x64-Realase mode. Make sure to use the *C++17 (ISO/IEC 14882)* compiler. The installer is built using *Visual Studio* too.

## Workaround with *gtkmm* in *Visual Studio*

The *gtkmm-4.0* libraries have been initially meant to be used in Linux systems, therefore it is not trivial to use them in Windows, especially with *Visual Studio*. The workaround is to use *vcpkg*, which is a tool for installing packages in Windows.

Accordingly to the README file in [https://github.com/microsoft/vcpkg](https://github.com/microsoft/vcpkg), installation is performed with:

* Install the prerequisites, i.e. *Git* (see [https://git-scm.com/downloads](https://git-scm.com/downloads)), and *Visual Studio* (>=2015 declared, >=2017 for *gtkmm4*). Git is only used to clone the repository at [https://github.com/microsoft/vcpkg](https://github.com/microsoft/vcpkg) in the PC: you can also download the repository directly.

* Create a path where to install *vcpkg*. Example path is `C:\src\vcpkg` or `C:\dev\vcpkg`, e.g. using the command prompt (with administrator elevation)
  
  ```shell
  mkdir C:\src\vcpkg
  ```

* Go to `C:\src` (or the installation path employed), e.g. using
  
  ```shell
  cd C:\src
  ```

* Clone the repository using *Git*
  
  ```shell
  git clone https://github.com/microsoft/vcpkg
  ```

* Bootstrap *vcpkg* using
  
  ```shell
  .\vcpkg\bootstrap-vcpkg.bat
  ```

* Install the *gtkmm4* libraries (x64 version)
  
  ```shell
  .\vcpkg\vcpkg install gtkmm --triplet=x64-windows
  ```
  
  This includes download and building of all the necessary dependencies. Installation can takes some time.

* Install *integrate* to use *vcpkg* with *Visual Studio* (with administrator elevation) using
  
  ```shell
     .\vcpkg\vcpkg integrate install
  ```

After having installed *gtkmm-4.0*, the libraries should appear in `C:\src\vcpkg\installed\x64-windows`. There the folder `./include` contains all the include files to use in the C/C++ source, while `./bin` and `./tools` contain the corresponding binaries file (i.e. `*.dll`). You can also find the static libraries in `./lib`.

You can test if *Visual Studio* works by compiling the HelloWorld example in [https://gitlab.gnome.org/GNOME/gtkmm-documentation/tree/master/examples/book/helloworld](https://gitlab.gnome.org/GNOME/gtkmm-documentation/tree/master/examples/book/helloworld). Make sure to:

* use *Visual Studio* in x64-Realase mode.
* use the *C++17 (ISO/IEC 14882)* compiler.

If issues are found, these are probably due to an incorrect include directory created by *vcpkg* for the *Visual Studio* compiler. The workaround is to install *pkg-config*, which is a tool for generating the include directories for C/C++ libraries:

* Install *pkgconf* (alias for *pkg-config* in *vcpkg*) using
  
  ```shell
  .\vcpkg\vcpkg install pkgconf --triplet=x64-windows
  ```
  
* Set an environment variable named `VCPKG_ROOT` pointing to the vcpkg installation path and restart the command prompt. This may be performed using the terminal (with administrator elevation), e.g. (use your installation path) with

  ```shell
  setx VCPKG_ROOT C:\src\vcpkg\ /m
  ```

* Go to `%VCPKG_ROOT%\installed\x64-windows\tools\pkgconf`
  
  ```shell
  cd %VCPKG_ROOT%\installed\x64-windows\tools\pkgconf
  ```

* Run *pkg-config* with *gtkmm4* using
  
  ```shell
  pkgconf --cflags-only-I gtkmm-4.0 --with-path=%VCPKG_ROOT%\installed\x64-windows\lib\pkgconfig
  ```
  
  This prints a list of include diretories (e.g. `-IC:%VCPKG_ROOT%/installed/**`), which is the flag to pass to the C/C++ compiler for including the necessary directories.

* You may also print a nicer list of include directories using in the ***Windows PowerShell*** 
  
  ```shell
  cd $env:VCPKG_ROOT\installed\x64-windows\tools\pkgconf
  $(./pkgconf --cflags-only-I gtkmm-4.0 --with-path=$env:VCPKG_ROOT\installed\x64-windows\lib\pkgconfig) -replace "-I", "`n"
  ```
  
    \attention Make sure to use the ***Windows PowerShell***, not the *Command Prompt*.

* In *Visual Studio* put these directories in *Additional Include Directory* for the C/C++ compiler. You may also add the following additional compiler flag
  
  ```shell
  `%VCPKG_ROOT%\installed\x64-windows\tools\pkgconf\pkgconf --cflags-only-I gtkmm-4.0 --with-path=%VCPKG_ROOT%\installed\x64-windows\lib\pkgconfig`
  ```
  
  or alternatively the output of
  
  ```shell
  pkgconf --cflags-only-I gtkmm-4.0 --with-path=%VCPKG_ROOT%\installed\x64-windows\lib\pkgconfig
  ```

### Tips

* You may also install the x32 version of the packages. In such case use `.\vcpkg\vcpkg install gtkmm`and`.\vcpkg\vcpkg install pkgconf` and replace all `x64-windows` with `x86-windows` in all commands.
* When using `gtkmm` you may need to add an environment variable named `GSETTINGS_SCHEMA_DIR` pointing to `C:\src\vcpkg\installed\x64-windows\share\glib-2.0\schemas`. You may also make a local copy of this directory in your project folder, which can be copied in the binary folder. This environment variable may be either global or local (you can use `putenv` command in C)

## Known issues

* Help me to find issues
* 2+2=5

\todo Change makefile to make it compatible with Mingw-w64. Copying dlls when building. TODO is to add -mwindows in cflags and change TARGETDIR if OS=WINDOWS.

\todo add more gestures to move and zoom in plots: e.g. press roll to move, and roll to zoom
\todo drag and drop in Windows
\todo Implement help but for linux and installation
\todo Implement direct open of a *.sbb file (argv and argc)????
\todo Add plot settings (plot_graph_decim, etc)
\todo Implement PSD in plot mode
