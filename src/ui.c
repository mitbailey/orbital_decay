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
#include <stdbool.h>

int main(void)
{
    int user_input = 0;
    float satellite_mass = 0.f, satellite_area = 0.f, altitude = 0.f, solar_radio_flux = 0.f, geomagnetic_a_index = 0.f, mission_time = 0.f;
    bool save_to_csv = false;
    char arg_list[128] = {0};
    int runs = 1;
    float run_increment = 50;

get_input:
    bprintlf("What would you like to calculate?");
    bprintlf("(1) Time to re-entry.");
    bprintlf("(2) Maximum average Solar Radio Flux.");
    bprintlf("(3) Maximum average Geomagnetic A Index.");
    bprintlf("(4) Save-to-CSV (TTR only): %s", save_to_csv ? "ON" : "OFF");
    bprintlf("(5) Runs: %d; Increment +%.03f km", runs, run_increment);
    // bprintlf("(5) GO BACK");
    bprintlf("(0) QUIT");
    bprintf("> ");
    if (scanf("%d", &user_input) != 1)
    {
        bprintlf(RED_FG "ERROR: Invalid input.\n");
        goto get_input;
    }
    while ((getchar()) != '\n')
        ;
intake:
    switch (user_input)
    {
    case 1: // Calculate time to re-entry.
        bprintlf("Enter the following values separated by spaces (press ENTER to cancel input):");
        bprintlf("Satellite Mass (kg)   Windward Area (m^2)   Initial Altitude (km)   Avg. Solar Radio Flux (F10.7, sfu)   Avg. Geomagnetic A Index");
        bprintf("> ");

        fgets(arg_list, sizeof(arg_list), stdin);

        if (!strcmp(arg_list, "\n"))
        {
            dbprintlf(RED_FG "Input canceled.\n")
            goto get_input;
        }

        if (sscanf(arg_list, "%f %f %f %f %f", &satellite_mass, &satellite_area, &altitude, &solar_radio_flux, &geomagnetic_a_index) != 5)
        {
            bprintf(RED_FG "ERROR: Invalid input; incorrect number of arguments entered.\n");
            while ((getchar()) != '\n')
                ;
            goto intake;
        }
        else if (altitude > MAX_ALTITUDE)
        {
            bprintlf(RED_FG "ERROR: Maximum altitude of %.1f km exceeded.\n", MAX_ALTITUDE);
            goto intake;
        }
        
        decay_calculate(satellite_mass, satellite_area, altitude, solar_radio_flux, geomagnetic_a_index, save_to_csv, runs, run_increment);
        bprintlf();
        
        goto intake;

    case 2:
        bprintlf("Enter the following values separated by spaces:");
        bprintlf("Satellite Mass (kg)   Windward Area (m^2)   Initial Altitude (km)   Avg. Geomagnetic A Index   Mission Time (days)");
        bprintf("> ");

        fgets(arg_list, sizeof(arg_list), stdin);

        if (!strcmp(arg_list, "\n"))
        {
            dbprintlf(RED_FG "Input canceled.\n")
            goto get_input;
        }

        if (sscanf(arg_list, "%f %f %f %f %f", &satellite_mass, &satellite_area, &altitude, &geomagnetic_a_index, &mission_time) != 5)
        {
            bprintf(RED_FG "ERROR: Invalid input; incorrect number of arguments entered.\n");
            while ((getchar()) != '\n')
                ;
            goto intake;
        }
        else if (altitude > MAX_ALTITUDE)
        {
            bprintlf(RED_FG "ERROR: Maximum altitude of %.1f km exceeded.\n", MAX_ALTITUDE);
            goto intake;
        }

        decay_max_avg_srf(satellite_mass, satellite_area, altitude, geomagnetic_a_index, mission_time);
        bprintlf();

        goto intake;

    case 3:
        bprintlf("Enter the following values separated by spaces:");
        bprintlf("Satellite Mass (kg)   Windward Area (m^2)   Initial Altitude (km)   Avg. Solar Radio Flux (F10.7)   Mission Time (days)");
        bprintf("> ");

        fgets(arg_list, sizeof(arg_list), stdin);

        if (!strcmp(arg_list, "\n"))
        {
            dbprintlf(RED_FG "Input canceled.\n")
            goto get_input;
        }

        if (sscanf(arg_list, "%f %f %f %f %f", &satellite_mass, &satellite_area, &altitude, &solar_radio_flux, &mission_time) != 5)
        {
            bprintf(RED_FG "ERROR: Invalid input; incorrect number of arguments entered.\n");
            while ((getchar()) != '\n')
                ;
            goto intake;
        }
        else if (altitude > MAX_ALTITUDE)
        {
            bprintlf(RED_FG "ERROR: Maximum altitude of %.1f km exceeded.\n", MAX_ALTITUDE);
            goto intake;
        }

        decay_max_avg_geo(satellite_mass, satellite_area, altitude, solar_radio_flux, mission_time);
        bprintlf();

        goto intake;

    case 4:
        save_to_csv = save_to_csv ? false : true;
        goto get_input;
    case 5:
        
        bprintlf("Enter the following values separated by spaces:");
        bprintlf("Number of Runs Desired   Altitude Increase Each Run (km)");
        bprintf("> ");

        fgets(arg_list, sizeof(arg_list), stdin);

        if (!strcmp(arg_list, "\n"))
        {
            dbprintlf(RED_FG "Input canceled.\n")
            goto get_input;
        }

        if (sscanf(arg_list, "%d %f", &runs, &run_increment) != 2)
        {
            bprintf(RED_FG "ERROR: Invalid input; incorrect number of arguments entered.\n");
            while ((getchar()) != '\n')
                ;
            goto get_input;
        }
        else if (runs < 1)
        {
            bprintlf(RED_FG "ERROR: Minimum of one run.\n");
            runs = 1;
            goto get_input;
        }
        else if (run_increment < 0.001)
        {
            bprintlf(RED_FG "ERROR: Minimum of 1 meter run increment.\n");
            run_increment = 0.001;
            goto get_input;
        }

        bprintlf();

        goto get_input;

    case 0:
        return 1;
    default:
        bprintlf(RED_FG "ERROR: Invalid value %hhu.\n", user_input);
        goto get_input;
    }
}