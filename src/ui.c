/**
 * @file ui.c
 * @author Mit Bailey (mitbailey99@gmail.com)
 * @brief Orbital Decay Simulation Program
 * @version See Git tags for version information.
 * @date 2021.06.01
 *
 * @copyright Copyright (c) 2021
 *
 */

// #include <stdlib.h>
#include "meb_print.h"
#include "decay.h"

int main(void)
{
    int user_input = 0;
    float satellite_mass = 0.f, satellite_area = 0.f, altitude = 0.f, solar_radio_flux = 0.f, geomagnetic_a_index = 0.f, mission_time = 0.f;

get_input:
    bprintlf("What would you like to calculate?");
    bprintlf("(1) Time to re-entry.");
    bprintlf("(2) Maximum average Solar Radio Flux.");
    bprintlf("(3) Maximum average Geomagnetic A Index.");
    bprintlf("(0) QUIT");
    bprintf("> ");
    if (scanf("%d", &user_input) != 1)
    {
        bprintlf(RED_FG "ERROR: Invalid input.\n");
        goto get_input;
    }
    while ((getchar()) != '\n')
        ;

    switch (user_input)
    {
    case 1: // Calculate time to re-entry.
        bprintlf("Enter the following values separated by spaces:");
        bprintlf("Satellite Mass (kg)   Windward Area (m^2)   Initial Altitude (km)   Avg. Solar Radio Flux (F10.7)   Avg. Geomagnetic A Index");
        bprintf("> ");
        if (scanf("%f %f %f %f %f", &satellite_mass, &satellite_area, &altitude, &solar_radio_flux, &geomagnetic_a_index) != 5)
        {
            bprintf(RED_FG "ERROR: Invalid input.\n");
            while ((getchar()) != '\n')
                ;
            goto get_input;
        }
        else if (altitude > MAX_ALTITUDE)
        {
            bprintlf(RED_FG "ERROR: Maximum altitude of %.1f km exceeded.\n", MAX_ALTITUDE);
            goto get_input;
        }
        decay_calculate(satellite_mass, satellite_area, altitude, solar_radio_flux, geomagnetic_a_index);
        bprintlf();
        goto get_input;
    case 2:
        bprintlf("Enter the following values separated by spaces:");
        bprintlf("Satellite Mass (kg)   Windward Area (m^2)   Initial Altitude (km)   Avg. Geomagnetic A Index   Mission Time (days)");
        bprintf("> ");
        if (scanf("%f %f %f %f %f", &satellite_mass, &satellite_area, &altitude, &geomagnetic_a_index, &mission_time) != 5)
        {
            bprintlf(RED_FG "ERROR: Invalid input.\n");
            while ((getchar()) != '\n')
                ;
            goto get_input;
        }
        else if (altitude > MAX_ALTITUDE)
        {
            bprintlf(RED_FG "ERROR: Maximum altitude of %.1f km exceeded.\n", MAX_ALTITUDE);
            goto get_input;
        }
        decay_max_avg_srf(satellite_mass, satellite_area, altitude, geomagnetic_a_index, mission_time);
        bprintlf();
        goto get_input;
    case 3:
        bprintlf("Enter the following values separated by spaces:");
        bprintlf("Satellite Mass (kg)   Windward Area (m^2)   Initial Altitude (km)   Avg. Solar Radio Flux (F10.7)   Mission Time (days)");
        bprintf("> ");
        if (scanf("%f %f %f %f %f", &satellite_mass, &satellite_area, &altitude, &solar_radio_flux, &mission_time) != 5)
        {
            bprintlf(RED_FG "ERROR: Invalid input.\n");
            while ((getchar()) != '\n')
                ;
            goto get_input;
        }
        else if (altitude > MAX_ALTITUDE)
        {
            bprintlf(RED_FG "ERROR: Maximum altitude of %.1f km exceeded.\n", MAX_ALTITUDE);
            goto get_input;
        }
        decay_max_avg_geo(satellite_mass, satellite_area, altitude, solar_radio_flux, mission_time);
        bprintlf();
        goto get_input;
    case 0:
        return 1;
    default:
        bprintlf(RED_FG "ERROR: Invalid value %hhu.\n", user_input);
        goto get_input;
    }
}