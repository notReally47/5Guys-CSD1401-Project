#pragma once
#include "structs.h"
#include "defines.h"

/*Read config file.*/
Config readFile(void);

/*Overwrites/writes config file.*/
void writeConfig(Config config);