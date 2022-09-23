/**
 * @file decay.c
 * @author Mit Bailey (mitbailey99@gmail.com)
 * @brief Orbital Decay Simulation Program
 * @version See Git tags for version information.
 * @date 2021.06.01
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <meb_print.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "decay.h"

double decay_max_avg_srf(double satellite_mass, double satellite_area, double altitude, double geomagnetic_a_index, int mission_time)
{
    mission_time *= 86400; // Convert mission time (days) to mission time (seconds).

    int time_of_flight = 0;
    double solar_radio_flux = 70;

    // While time_of_flight is not within a day of the desired mission time...
    double increment = 1;
    bool was_less_than_desired = true;

    bprintlf("Working...");

    for (int attempt = 0; attempt < 100; attempt++)
    {
        time_of_flight = decay_calculate_suppressed(satellite_mass, satellite_area, altitude, solar_radio_flux, geomagnetic_a_index);

        bprintlf("Attempt %d: Descrepancy of %d seconds with SRF of %f.", attempt, time_of_flight - mission_time, solar_radio_flux);

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

    bprintlf("Desired mission time: %d seconds, %d days.", mission_time, mission_time / 86400);
    bprintlf("Time of flight found to be %d with a F10.7 index Solar Radio Flux of %f and a Geomagnetic A Index of %f (%d second descrepancy).", time_of_flight, solar_radio_flux, geomagnetic_a_index, time_of_flight - mission_time);

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

    bprintlf("Working...");

    for (int attempt = 0; attempt < 100; attempt++)
    {
        time_of_flight = decay_calculate_suppressed(satellite_mass, satellite_area, altitude, solar_radio_flux, geomagnetic_a_index);

        bprintlf("Attempt %d: Descrepancy of %d seconds with GEO of %f", attempt, time_of_flight - mission_time, geomagnetic_a_index);

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

    bprintlf("Desired mission time: %d seconds, %d days.", mission_time, mission_time / 86400);
    bprintlf("Time of flight found to be %d with a F10.7 index Solar Radio Flux of %f and a Geomagnetic A Index of %f (%d second descrepancy).", time_of_flight, solar_radio_flux, geomagnetic_a_index, time_of_flight - mission_time);

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

        if (elapsed_time >= 1.577e9)
        {
            bprintlf("Satellite lifetime exceeds 50 years.");
            return -1;
        }

        orbital_period -= delta_period;
        elapsed_time += delta_time;
        orbital_radius = pow((pow(orbital_period, 2.0) * GRAVITATIONAL_CONSTANT * EARTH_MASS / 4 / pow(PI, 2.0)), 0.33333);
        altitude = orbital_radius - EARTH_RADIUS;
    }

    return elapsed_time;
}

int decay_calculate(double satellite_mass, double satellite_area, double altitude, double solar_radio_flux, double geomagnetic_a_index, bool save_to_csv, int runs, float run_increment)
{
    altitude *= 1000;     
    int orig_altitude = altitude;
    run_increment *= 1000;          
                                                                             // Convert altitude (km) to altitude (m).
    int elapsed_time = 0;                                                                                    // Elapsed time measured in whole seconds.
    double orbital_radius = EARTH_RADIUS + altitude;                                                         // Orbital radius measured in meters.
    double orbital_period = 2.0 * PI * sqrt(pow(orbital_radius, 3.0) / EARTH_MASS / GRAVITATIONAL_CONSTANT); // Orbital period measured in fractional seconds.

    double SH = 0.0;                  // A placeholder calculation for atmospheric_density.
    double atmospheric_density = 0.0; // Atmospheric density at the satellite's current altitude.
    double delta_period = 0.0;        // Change in orbital period from previous iteration.

    FILE* fp = NULL;
    if (save_to_csv)
    {
        fp = fopen("data.csv", "a");
        if (fp == NULL)
        {
            save_to_csv = false;
        }
    }

    bprintlf("\nBEGIN CALCULATION");
    if (save_to_csv) {fprintf(fp, "BEGIN CALCULATION\n");}
    for (int i = 1; i <= runs; i++)
    {
        bprintlf("RUN %d/%d", i, runs);
        bprintlf("TIME (days)\tHEIGHT (km)\t\tPERIOD (minutes)");
        if (save_to_csv) {fprintf(fp, "\nRUN %d/%d\n", i, runs);}
        if (save_to_csv) {fprintf(fp, "TIME (days), HEIGHT (km), PERIOD (minutes)\n");}

        while (altitude >= KARMAN_LINE)
        {
            SH = (900 + 2.5 * (solar_radio_flux - 70) + 1.5 * geomagnetic_a_index) / (27 - 0.012 * ((altitude / 1000) - 200));
            atmospheric_density = 6e-10 * exp(-((altitude / 1000) - 175) / SH);

            delta_period = 3 * PI * satellite_area / satellite_mass * orbital_radius * atmospheric_density * (double)TEMPORAL_RESOLUTION;

            // TODO: Devise a more advanced print-out method.
            if (elapsed_time % (7 * 86400) == 0) // 86400 sec = 1 day
            {
                bprintlf("%d\t\t%f\t\t%f", elapsed_time / 86400, altitude / 1000, orbital_period);
                if (save_to_csv) {fprintf(fp, "%d, %f, %f\n", elapsed_time / 86400, altitude / 1000, orbital_period);}

                if (elapsed_time >= 1.577e9)
                {
                    bprintlf("Satellite lifetime exceeds 50 years.");
                    if (save_to_csv) {fprintf(fp, "Satellite lifetime exceeds 50 years.\n");}
                    return -1;
                }
            }

            orbital_period -= delta_period;
            elapsed_time += TEMPORAL_RESOLUTION;
            orbital_radius = pow((pow(orbital_period, 2.0) * GRAVITATIONAL_CONSTANT * EARTH_MASS / 4 / pow(PI, 2.0)), 0.33333);
            altitude = orbital_radius - EARTH_RADIUS;
        }

        // Final print-out.
        bprintlf("%d\t\t%f\t\t%f\n", elapsed_time / 86400, altitude / 1000, orbital_period);
        if (save_to_csv) {fprintf(fp, "%d, %f, %f\n", elapsed_time / 86400, altitude / 1000, orbital_period);}

        bprintlf("Re-entry after %f days (%f years).\n", (double)elapsed_time / (3600 * 24), (double)elapsed_time / (3600 * 24 * 365));
        if (save_to_csv) {fprintf(fp, "\nRe-entry after %f days (%f years).\n\n", (double)elapsed_time / (3600 * 24), (double)elapsed_time / (3600 * 24 * 365));}

        // Reset everything for next run.
        altitude = orig_altitude + (i * run_increment);
        elapsed_time = 0;                                                                                  
        orbital_radius = EARTH_RADIUS + altitude;                                                        
        orbital_period = 2.0 * PI * sqrt(pow(orbital_radius, 3.0) / EARTH_MASS / GRAVITATIONAL_CONSTANT);
        SH = 0.0;
        atmospheric_density = 0.0;
        delta_period = 0.0;
    }

    bprintlf("END CALCULATION");
    if (save_to_csv) {fprintf(fp, "END CALCULATION\n");}
    bprintlf("------------------------------------------");
    if (save_to_csv) {fprintf(fp, "------------------------------------------\n\n");}

    if (fp != NULL)
    {
        fclose(fp);
    }

    return elapsed_time;
}

// TODO: Stop the UI from backing out to calculation selection after performing a calculation.
// TODO: Add 'Back', 'Exit', and 'Help' options.
// TODO: Add the ability to print results to files.
// TODO: Add the ability to insert a range of values. For instance, reentry over a range of altitudes w/ some increment.
// int decay_calculate_to_file()