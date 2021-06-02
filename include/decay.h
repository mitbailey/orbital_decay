/**
 * @file decay.h
 * @author Mit Bailey (mitbailey99@gmail.com)
 * @brief Orbital Decay Simulation Program
 * 
 * Based on the SATELLITE ORBITAL DECAY program written in QuickBASIC presented in
 * https://www.sws.bom.gov.au/Category/Educational/Space%20Weather/Space%20Weather%20Effects/SatelliteOrbitalDecayCalculations.pdf
 * 
 * @version 0.1
 * @date 2021.06.01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#define EARTH_RADIUS_M 6378000 // m
#define EARTH_MASS_KG 5.98e24 // kg
#define GRAVITATIONAL_CONSTANT 6.67e-11
#define PI 3.1415926
#define KARMAN_LINE_KM 100 // km

void decay_calculate(double satellite_mass, double satellite_area, double altitude_initial, double solar_radio_flux, double geomagnetic_a_index);

void decay_calculate_old(double M, double A, double H, double F10, double Ap);