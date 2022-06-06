# List of used libraries

* *cairoplotter*: for plots in *gtkmm4* (from <https://github.com/AlexB67/CairoPlot>)
* *tinymatwritter*: for saving `*.mat` file used in *MATLAB* (from <https://github.com/jkriege2/TinyMAT>)
* *fftw*: binaries for fast fourier transform in Windows only (in Linux the library is installed as a packege). See http://www.fftw.org/install/windows.html for installation in Windows.

Note that all source code of the used libraries is already included in `./lib` (no additional static or dynamic libraries are necessary).

## Tips

In order to add a new library, one should:

* Add the library (both sources and headers) in `./lib`, preferably within a dedicated folder
* In Linux: add a new include flag in the `LIB` variable of the `Makefile` with the new folder
* In Windows: add the new folder (and possible subfolders) in *Additional Include Directory* for the C/C++ compiler in *Visual Studio*
