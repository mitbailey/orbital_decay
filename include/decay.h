/**
 * @file decay.h
 * @author Mit Bailey (mitbailey99@gmail.com)
 * @brief Orbital Decay Simulation Program
 *
 * Based on the SATELLITE ORBITAL DECAY program written in QuickBASIC presented in
 * https://www.sws.bom.gov.au/Category/Educational/Space%20Weather/Space%20Weather%20Effects/SatelliteOrbitalDecayCalculations.pdf
 *
 * Data on Solar Radio Flux and the F10.7 index can be found at
 * https://www.spaceweather.gc.ca/forecast-prevision/solar-solaire/solarflux/sx-en.php
 *
 * Data on Geomagnetic A Index can be found at
 * https://www.swpc.noaa.gov/products/station-k-and-indices
 *
 * @version See Git tags for version information.
 * @date 2021.06.01
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <stdbool.h>

#define EARTH_RADIUS 6378000 // Radius of the Earth in meters.
#define EARTH_MASS 5.98e24   // Mass of the Earth in kilograms.
#define GRAVITATIONAL_CONSTANT 6.67e-11
#define PI 3.1415926
#define KARMAN_LINE 100000     // Minimum possible altitude in meters.
#define TEMPORAL_RESOLUTION 60 // Seconds per step.
#define MAX_ALTITUDE 5877.5 // km

/**
 * @brief Finds the maximum average Solar Radio Flux the given satellite can endure while maintaining a desired time of flight.
 *
 * @param satellite_mass Mass of the satellite in kilograms.
 * @param satellite_area Effective windward area of the satellite in square meters.
 * @param altitude The initial altitude of the satellite above the Earth's surface in kilometers.
 * @param geomagnetic_a_index The average Geomagnetic A Index during the satellite's lifetime.
 * @param mission_time The desired mission duration in days.
 * @return double Maximum average Solar Radio Flux.
 */
double decay_max_avg_srf(double satellite_mass, double satellite_area, double altitude, double geomagnetic_a_index, int mission_time);

/**
 * @brief Calculates the maximum average Geomagnetic A Index the given satellite can endure while maintaining a desired time of flight.
 *
 * @param satellite_mass Mass of the satellite in kilograms.
 * @param satellite_area Effective windward area of the satellite in square meters.
 * @param altitude The initial altitude of the satellite above the Earth's surface in kilometers.
 * @param solar_radio_flux The average Solar Radio Flux during the satellite's lifetime.
 * @param mission_time The desired mission duration in days.
 * @return double Maximum average Geomagnetic A Index.
 */
double decay_max_avg_geo(double satellite_mass, double satellite_area, double altitude, double solar_radio_flux, int mission_time);

/**
 * @brief Calculates the amount of time it will take a satellite to descend to below the Karman Line due to air resistance.
 *
 * All values should be in meters, seconds, or kilograms.
 *
 * @param satellite_mass Mass of the satellite in kilograms.
 * @param satellite_area Effective windward area of the satellite in square meters.
 * @param altitude The initial altitude of the satellite above the Earth's surface in kilometers.
 * @param solar_radio_flux The average Solar Radio Flux during the satellite's lifetime.
 * @param geomagnetic_a_index The average Geomagnetic A Index during the satellite's lifetime.
 * @param save_to_csv True - saves the run data in a .csv file; False - Does not.
 * @param runs Number of times to run the calculation.
 * @param run_increment Delta-altitude per run (km).
 * @return int Elapsed time in seconds before falling below the Karman Line.
 */
int decay_calculate(double satellite_mass, double satellite_area, double altitude, double solar_radio_flux, double geomagnetic_a_index, bool save_to_csv, int runs, float run_increment);

// Decay calculate without prints, used by decay_max_avg_activity(...).
int decay_calculate_suppressed(double satellite_mass, double satellite_area, double altitude, double solar_radio_flux, double geomagnetic_a_index);