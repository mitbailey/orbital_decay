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
 * @brief 
 * 
 * @param satellite_mass 
 * @param satellite_area 
 * @param altitude_initial 
 * @param solar_radio_flux 
 * @param geomagnetic_a_index 
 */
void decay_calculate(double satellite_mass_kg, double satellite_area_m2, double altitude_km, double solar_radio_flux, double geomagnetic_a_index)
{
    double elapsed_time_days = 0.0;
    double delta_time_days = 0.01;
    double delta_time_sec = delta_time_days * 3600 * 24;
    double altitude_m = altitude_km * 1000;
    double orbital_radius_m = EARTH_RADIUS_M + altitude_m;
    double orbital_period_sec = 2.0 * PI * sqrt(pow(orbital_radius_m, 3.0) / EARTH_MASS_KG / GRAVITATIONAL_CONSTANT);

    double SH = 0.0;
    double atmospheric_density = 0.0;
    double delta_period_sec = 0.0;

    printf("TIME (days)\tHEIGHT (km)\tPERIOD (minutes)\n");

    for (int i = 0; altitude_m >= KARMAN_LINE_M; i++)
    {
        SH = (900 + 2.5 * (solar_radio_flux - 70) + 1.5 * geomagnetic_a_index) / (27 - 0.012 * ((altitude_m / 1000) - 200));
        atmospheric_density = 6e-10 * exp(-((altitude_m / 1000) - 175) / SH);

        delta_period_sec = 3 * PI * satellite_area_m2 / satellite_mass_kg * orbital_radius_m * atmospheric_density * delta_time_sec;

        // Print-outs went here.
        // TODO: Devise a more advanced print-out method.
        if (i % 10000 == 0)
        {
            printf("%f\t%f\t%f\n", elapsed_time_days, altitude_m / 1000, orbital_period_sec);
        }

        orbital_period_sec -= delta_period_sec;
        elapsed_time_days += delta_time_days;
        orbital_radius_m = pow((pow(orbital_period_sec, 2.0) * GRAVITATIONAL_CONSTANT * EARTH_MASS_KG / 4 / pow(PI, 2.0)), 0.33333);
        altitude_m = orbital_radius_m - EARTH_RADIUS_M;
    }

    printf("Re-entry after %f days (%f years).\n", elapsed_time_days, elapsed_time_days / 365);
    printf("%f %f %f\n", elapsed_time_days, altitude_m / 1000, orbital_period_sec / 60);
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