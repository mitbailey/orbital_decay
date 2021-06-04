/**
 * @file decay.c
 * @author Mit Bailey (mitbailey99@gmail.com)
 * @brief 
 * @version 0.2
 * @date 2021.06.01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
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

double decay_max_avg_srf(double satellite_mass, double satellite_area, double altitude, double geomagnetic_a_index, int mission_time)
{
    mission_time *= 86400; // Convert mission time (days) to mission time (seconds).

    int time_of_flight = 0;
    double solar_radio_flux = 70;

    // While time_of_flight is not within a day of the desired mission time...
    double increment = 1;
    bool was_less_than_desired = true;

    printf("Working...\n");
    fflush(stdout);

    for (int attempt = 0; attempt < 100; attempt++)
    {
        time_of_flight = decay_calculate_suppressed(satellite_mass, satellite_area, altitude, solar_radio_flux, geomagnetic_a_index);

        printf("Attempt %d: Descrepancy of %d seconds with SRF of %f\n", attempt, time_of_flight - mission_time, solar_radio_flux);
        fflush(stdout);

        if (time_of_flight < mission_time)
        {
            if (!was_less_than_desired)
            {
                increment *= 0.5;
                was_less_than_desired = true;
            }

            solar_radio_flux -= increment;
        }
        else if (time_of_flight > mission_time)
        {
            if (was_less_than_desired)
            {
                increment *= 0.5;
                was_less_than_desired = false;
            }

            solar_radio_flux += increment;
        }
        else
        {
            break;
        }
    }

    printf("Desired mission time: %d seconds, %d days.\n", mission_time, mission_time / 86400);
    printf("Time of flight found to be %d with a F10.7 index Solar Radio Flux of %f and a Geomagnetic A Index of %f (%d second descrepancy).\n", time_of_flight, solar_radio_flux, geomagnetic_a_index, time_of_flight - mission_time);

    return solar_radio_flux;
}

double decay_max_avg_geo(double satellite_mass, double satellite_area, double altitude, double solar_radio_flux, int mission_time)
{
    mission_time *= 86400; // Convert mission time (days) to mission time (seconds).

    int time_of_flight = 0;
    double geomagnetic_a_index = 10;

    // While time_of_flight is not within a day of the desired mission time...
    double increment = 1;
    bool was_less_than_desired = true;

    printf("Working...\n");
    fflush(stdout);

    for (int attempt = 0; attempt < 100; attempt++)
    {
        time_of_flight = decay_calculate_suppressed(satellite_mass, satellite_area, altitude, solar_radio_flux, geomagnetic_a_index);

        printf("Attempt %d: Descrepancy of %d seconds with GEO of %f\n", attempt, time_of_flight - mission_time, geomagnetic_a_index);
        fflush(stdout);

        if (time_of_flight < mission_time)
        {
            if (!was_less_than_desired)
            {
                increment *= 0.5;
                was_less_than_desired = true;
            }

            geomagnetic_a_index -= increment;
        }
        else if (time_of_flight > mission_time)
        {
            if (was_less_than_desired)
            {
                increment *= 0.5;
                was_less_than_desired = false;
            }

            geomagnetic_a_index += increment;
        }
        else
        {
            break;
        }
    }

    printf("Desired mission time: %d seconds, %d days.\n", mission_time, mission_time / 86400);
    printf("Time of flight found to be %d with a F10.7 index Solar Radio Flux of %f and a Geomagnetic A Index of %f (%d second descrepancy).\n", time_of_flight, solar_radio_flux, geomagnetic_a_index, time_of_flight - mission_time);

    return solar_radio_flux;
}

int decay_calculate_suppressed(double satellite_mass, double satellite_area, double altitude, double solar_radio_flux, double geomagnetic_a_index)
{
    altitude *= 1000;                                                                                        // Convert altitude (km) to altitude (m).
    int delta_time = TEMPORAL_RESOLUTION;                                                                    // Change in time per iteration measured in whole seconds.
    int elapsed_time = 0;                                                                                    // Elapsed time measured in whole seconds.
    double orbital_radius = EARTH_RADIUS + altitude;                                                         // Orbital radius measured in meters.
    double orbital_period = 2.0 * PI * sqrt(pow(orbital_radius, 3.0) / EARTH_MASS / GRAVITATIONAL_CONSTANT); // Orbital period measured in fractional seconds.

    double SH = 0.0;                  // A placeholder calculation for atmospheric_density.
    double atmospheric_density = 0.0; // Atmospheric density at the satellite's current altitude.
    double delta_period = 0.0;        // Change in orbital period from previous iteration.

    while (altitude >= KARMAN_LINE)
    {
        SH = (900 + 2.5 * (solar_radio_flux - 70) + 1.5 * geomagnetic_a_index) / (27 - 0.012 * ((altitude / 1000) - 200));
        atmospheric_density = 6e-10 * exp(-((altitude / 1000) - 175) / SH);

        delta_period = 3 * PI * satellite_area / satellite_mass * orbital_radius * atmospheric_density * (double)delta_time;

        if (elapsed_time >= 1.577e9) {
            printf("Satellite lifetime exceeds 50 years.\n");
            return -1;
        }

        orbital_period -= delta_period;
        elapsed_time += delta_time;
        orbital_radius = pow((pow(orbital_period, 2.0) * GRAVITATIONAL_CONSTANT * EARTH_MASS / 4 / pow(PI, 2.0)), 0.33333);
        altitude = orbital_radius - EARTH_RADIUS;
    }

    return elapsed_time;
}

int decay_calculate(double satellite_mass, double satellite_area, double altitude, double solar_radio_flux, double geomagnetic_a_index)
{
    altitude *= 1000;                                                                                        // Convert altitude (km) to altitude (m).
    int delta_time = TEMPORAL_RESOLUTION;                                                                    // Change in time per iteration measured in whole seconds.
    int elapsed_time = 0;                                                                                    // Elapsed time measured in whole seconds.
    double orbital_radius = EARTH_RADIUS + altitude;                                                         // Orbital radius measured in meters.
    double orbital_period = 2.0 * PI * sqrt(pow(orbital_radius, 3.0) / EARTH_MASS / GRAVITATIONAL_CONSTANT); // Orbital period measured in fractional seconds.

    double SH = 0.0;                  // A placeholder calculation for atmospheric_density.
    double atmospheric_density = 0.0; // Atmospheric density at the satellite's current altitude.
    double delta_period = 0.0;        // Change in orbital period from previous iteration.

    printf("TIME (days)\tHEIGHT (km)\t\tPERIOD (minutes)\n");

    while (altitude >= KARMAN_LINE)
    {
        SH = (900 + 2.5 * (solar_radio_flux - 70) + 1.5 * geomagnetic_a_index) / (27 - 0.012 * ((altitude / 1000) - 200));
        atmospheric_density = 6e-10 * exp(-((altitude / 1000) - 175) / SH);

        delta_period = 3 * PI * satellite_area / satellite_mass * orbital_radius * atmospheric_density * (double)delta_time;

        // TODO: Devise a more advanced print-out method.
        if (elapsed_time % (7 * 86400) == 0) // 86400 sec = 1 day
        {
            printf("%d\t\t%f\t\t%f\n", elapsed_time / 86400, altitude / 1000, orbital_period);

            if (elapsed_time >= 1.577e9) {
                printf("Satellite lifetime exceeds 50 years.\n");
                return -1;
            }
        }

        orbital_period -= delta_period;
        elapsed_time += delta_time;
        orbital_radius = pow((pow(orbital_period, 2.0) * GRAVITATIONAL_CONSTANT * EARTH_MASS / 4 / pow(PI, 2.0)), 0.33333);
        altitude = orbital_radius - EARTH_RADIUS;
    }

    // Final print-out.
    printf("%d\t\t%f\t\t%f\n\n", elapsed_time / 86400, altitude / 1000, orbital_period);

    printf("Re-entry after %f days (%f years).\n", (double)elapsed_time / (3600 * 24), (double)elapsed_time / (3600 * 24 * 365));

    return elapsed_time;
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