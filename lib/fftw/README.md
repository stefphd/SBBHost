# FFTW library folder

This is the FFTW library folder to use with Windows (version 3.3.5).

Subfolders necessary for compilation are:
* `./bin` contains the binary files (`*.dll` library, i.e. dynamic)
* `./include` contains the header files (`*.h` files) to be included in the source code
* `./lib` contains the library file (`*.lib` library, i.e. static)

To use this library:
* Use

	```C
	#include <fftw.h>
	```

	in your sorce code
	
* Add `./include` in your additional include directories
* Add `./lib` in your additional library directories for the linker (this makes use of the static library)
* Add *libfftw3-3.lib* as an additional dependecies for the linker
* Copy the contents of `./bin` in your *bin* directoy