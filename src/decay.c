/**
 * @file decay.c
 * @author Mit Bailey (mitbailey99@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021.06.01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <math.h>
#include "decay.h"

// Old      New
// dT       delta_time
// D9       delta_time_sec
// A        satellite_area
// H        altitude
// H1       Print height increment? altitude_1
// H2       Print height? altitude_2
// R        orbital_radius
// Ap       geo_a_index
// F10      solar_r_flux
// M        satellite_mass
// P        orbital_period (seconds)
// Pm       orbital_period_mins (minutes)
// dP       delta_period
// T        elapsed_time

// TODO: Calculations solved for other variables, ie find maximum average Ap and F10.7 given an altitude and desited time-of-flight.

/**
 * @brief Calculates the amount of time it will take a satellite to descend to below the Karman Line due to air resistance.
 * 
 * All values should be in meters, seconds, or kilograms.
 * 
 * @param satellite_mass Mass of the satellite in kilograms.
 * @param satellite_area Effective windward area of the satellite in square meters.
 * @param altitude_initial The initial altitude of the satellite above the Earth's surface in kilometers.
 * @param solar_radio_flux The average Solar Radio Flux during the satellite's lifetime.
 * @param geomagnetic_a_index The average Geomagnetic A Index during the satellite's lifetime.
 */
void decay_calculate(double satellite_mass, double satellite_area, double altitude, double solar_radio_flux, double geomagnetic_a_index, int delta_time)
{
    altitude *= 1000; // Convert altitude (km) to altitude (m).
    //const int delta_time = 864; // Change in time per iteration measured in whole seconds.
    int elapsed_time = 0; // Elapsed time measured in whole seconds.
    double orbital_radius = EARTH_RADIUS + altitude; // Orbital radius measured in meters.
    double orbital_period = 2.0 * PI * sqrt(pow(orbital_radius, 3.0) / EARTH_MASS / GRAVITATIONAL_CONSTANT); // Orbital period measured in fractional seconds.

    double SH = 0.0; // A placeholder calculation for atmospheric_density.
    double atmospheric_density = 0.0; // Atmospheric density at the satellite's current altitude.
    double delta_period = 0.0; // Change in orbital period from previous iteration.

    printf("TIME (days)\t\tHEIGHT (km)\t\tPERIOD (minutes)\n");

    while (altitude >= KARMAN_LINE)
    {
        SH = (900 + 2.5 * (solar_radio_flux - 70) + 1.5 * geomagnetic_a_index) / (27 - 0.012 * ((altitude / 1000) - 200));
        atmospheric_density = 6e-10 * exp(-((altitude / 1000) - 175) / SH);

        delta_period = 3 * PI * satellite_area / satellite_mass * orbital_radius * atmospheric_density * (double)delta_time;

        // TODO: Devise a more advanced print-out method.
        if (elapsed_time % 86400 == 0)
        {
            printf("%d\t\t%f\t\t%f\n", elapsed_time, altitude / 1000, orbital_period);
        }

        orbital_period -= delta_period;
        elapsed_time += delta_time;
        orbital_radius = pow((pow(orbital_period, 2.0) * GRAVITATIONAL_CONSTANT * EARTH_MASS / 4 / pow(PI, 2.0)), 0.33333);
        altitude = orbital_radius - EARTH_RADIUS;
    }

    printf("Re-entry after %f days (%f years).\n", (double)elapsed_time / (3600 * 24), (double)elapsed_time / (3600 * 24 * 365));
    printf("%d %f %f\n", elapsed_time, altitude / 1000, orbital_period);
}

void decay_calculate_old(double M, double A, double H, double F10, double Ap)
{
    double Re = 6378000;
    double Me = 5.98e24;
    double G = 6.67e-11;
    double pi = 3.1416;

    double T = 0;
    double dT = 0.1;
    double D9 = dT * 3600 * 24;
    double R = Re + H * 1000;
    double P = 2 * pi * sqrt(R * R * R / Me / G);

    while (H >= 180)
    {
        double SH = (900 + 2.5 * (F10 - 70) + 1.5 * Ap) / (27 - 0.012 * (H - 200));
        double DN = 6e-10 * exp(-(H - 175) / SH);
        double dP = 3 * pi * A / M * R * DN * D9;

        P = P - dP;
        T = T + dT;
        R = pow((G * Me * P * P / 4 / pi / pi), 0.33333);
        H = (R - Re) / 1000;
    }

    printf("Re-entry after %f days (%f years).\n", T, T / 365);
    printf("%f %f %f\n", T, H, P / 60);
}