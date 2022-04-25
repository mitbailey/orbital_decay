# Orbital Decay
Program to calculate the time to re-entry of a low Earth-orbiting satellite.

## Use Instructions
### Linux
1. Run the following commands in a Terminal from within the repository directory:  
```
$ make
$ ./decay.out
```

2. Follow the on-screen prompts. 

### Windows
The program is not currently available to run on Windows. However, WSL2 can be used to run the program as if using Linux.

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