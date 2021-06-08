/**
 * @file ui.c
 * @author Mit Bailey (mitbailey99@gmail.com)
 * @brief Orbital Decay Simulation Program
 * @version 0.3
 * @date 2021.06.01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include "decay.h"

// Examples:
// decay_calculate_old(3.0, 0.03, 465, 70, 10);
// decay_calculate(3.0, 0.03, 465, 70, 10);
// decay_max_avg_srf(3.0, 0.03, 465, 10, 3000);
// decay_max_avg_geo(3.0, 0.03, 465, 70, 3000);

int main (void) {
    int user_input = 0;
    float satellite_mass = 0.0, satellite_area = 0.0, altitude = 0.0, solar_radio_flux = 0.0, geomagnetic_a_index = 0.0, mission_time = 0.0;

get_input:
    printf("What would you like to calculate?\n");
    printf("(1) Time to re-entry.\n");
    printf("(2) Maximum average Solar Radio Flux.\n");
    printf("(3) Maximum average Geomagnetic A Index.\n");
    printf("(0) QUIT\n");
    printf("> ");
    if (scanf("%d", &user_input) != 1) {
        printf("Invalid input.\n");
        printf("\n");
        goto get_input;
    }
    while((getchar()) != '\n');

    switch (user_input) {
        case 1: // Calculate time to re-entry.
            printf("Enter the following values separated by spaces:\n");
            printf("Satellite Mass (kg)   Windward Area (m^2)   Initial Altitude (km)   Avg. Solar Radio Flux (F10.7)   Avg. Geomagnetic A Index\n");
            printf("> ");
            if (scanf("%f %f %f %f %f", &satellite_mass, &satellite_area, &altitude, &solar_radio_flux, &geomagnetic_a_index) != 5) {
                printf("Invalid input.\n");
                printf("\n");
                while((getchar()) != '\n');
                goto get_input;
            }
            decay_calculate(satellite_mass, satellite_area, altitude, solar_radio_flux, geomagnetic_a_index);
            printf("\n");
            goto get_input;
        case 2:
            printf("Enter the following values separated by spaces:\n");
            printf("Satellite Mass (kg)   Windward Area (m^2)   Initial Altitude (km)   Avg. Geomagnetic A Index   Mission Time (days)\n");
            printf("> ");
            if (scanf("%f %f %f %f %f", &satellite_mass, &satellite_area, &altitude, &geomagnetic_a_index, &mission_time) != 5) {
                printf("Invalid input.\n");
                printf("\n");
                while((getchar()) != '\n');
                goto get_input;
            }
            decay_max_avg_srf(satellite_mass, satellite_area, altitude, geomagnetic_a_index, mission_time);
            printf("\n");
            goto get_input;
        case 3:
            printf("Enter the following values separated by spaces:\n");
            printf("Satellite Mass (kg)   Windward Area (m^2)   Initial Altitude (km)   Avg. Solar Radio Flux (F10.7)   Mission Time (days)\n");
            printf("> ");
            if (scanf("%f %f %f %f %f", &satellite_mass, &satellite_area, &altitude, &solar_radio_flux, &mission_time) != 5) {
                printf("Invalid input.\n");
                printf("\n");
                while((getchar()) != '\n');
                goto get_input;
            }
            decay_max_avg_geo(satellite_mass, satellite_area, altitude, solar_radio_flux, mission_time);
            printf("\n");
            goto get_input;
        case 0:
            return 1;
        default:
            printf("Invalid value %hhu.\n", user_input);
            printf("\n");
            goto get_input;
    }
}