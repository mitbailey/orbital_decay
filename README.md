# Orbital Decay
## Description
Program to calculate the time to re-entry of a low Earth-orbiting satellite.

## Prerequisites
__For Windows, Mingw-w64 is required to compile and / or run the program.__

To install Mingw-w64, follow steps 3 and 4 under the 'Prerequisites' section of the following document, which are also listed below:  
https://code.visualstudio.com/docs/cpp/config-mingw  

1. Proceed to https://www.msys2.org/.
2. Follow the instructions under the red "Installation" header beginning with downloading the MSYS2 installer. For clarity, some of those instructions have been mirrored below.
3. Once the MSYS2 terminal has opened automatically, run the following command:  
   `pacman -Syu`  
   _Note that this may take a minute..._

4. Once the MSYS2 terminal closes, use the Windows Start menu to search for and open "MSYS2 MSYS".
5. Once the MSYS2 terminal has opened, run the following command:  
   `pacman -Su`  
    _Note that this may take a minute..._   
6. To install mingw-w64, run the following command:  
   `pacman -S --needed base-devel mingw-w64-x86_64-toolchain`  
   _Note that this may take a minute..._   
7. Now that Mingw-w64 is installed, it must be added to the windows `PATH` environment variable. To do so, begin by making note of the path to Mingw-w64's `bin` folder. By default it is `C:\msys64\mingw64\bin`.
8. Search "Edit the system environment variables" in the Windows Start menu and select the entry of the same name.
9. Click the "Environment Variables..." option.
10. Find the variable labeled "Path", click the entry, and then click the "Edit..." button.
11. Click the "New" button and paste in the Mingw-w64 path found in step 7.
12. Select OK to confirm. Close and reopen any active console windows.

## Compilation
### Windows  
Double-click on `make.bat`  
or open Command Prompt in the orbital_decay/ directory and execute  
`make.bat`  
(`./make.bat` for PowerShell)  
  
### Linux  
Open Terminal in the orbital_decay/ directory and execute  
`make`  

## Use Instructions
### Linux
1. Run the following commands in a Terminal from within the repository directory:  
```
$ make
$ ./decay.out
```

2. Follow the on-screen prompts. 

### Windows
The Windows executable can be downloaded from the Releases (https://github.com/mitbailey/orbital_decay/releases) and run. Alternatively, the code can be compiled on Windows using Mingw-w64 and the instructions above (int the Compilation section).

## Finding Data
The program requires knowledge of the solar 10.7 cm radio flux intensity (F10.7 scale) and the geomagnetic field intensity (A Index) during the mission. Data for both of these can be found at the links below:

_Solar Radio Flux (F10.7)_    
https://www.spaceweather.gc.ca/forecast-prevision/solar-solaire/solarflux/sx-en.php

_Geomagnetic A Index_    
https://www.swpc.noaa.gov/products/station-k-and-indices

## Constants
The minimum possible altitude for a satellite is considered to be the Karman Line at 100km above sea level.

The calculator's temporal step size is 60 seconds.

## Limitations
The calculator assumes a circular initial orbit and requires an initial altitude less than 5,877.5 km.