/**
 * @file test.c
 * @author Mit Bailey (mitbailey99@gmail.com)
 * @brief 
 * @version 0.2
 * @date 2021.06.01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdlib.h>
#include "decay.h"

int main (void) {

    //decay_calculate_old(3.0, 0.03, 465, 70, 10);

    decay_calculate(3.0, 0.03, 465, 70, 10);
    decay_max_avg_srf(3.0, 0.03, 465, 10, 3000);
    decay_max_avg_geo(3.0, 0.03, 465, 70, 3000);

    return 1;
}