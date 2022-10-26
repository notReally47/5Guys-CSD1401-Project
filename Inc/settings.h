#pragma once
#include <stdio.h>
#include "structs.h"

Config readConfig(const char* filePath, const char* mode);
void createConfig(const char* filePath, const char* mode);
void updateConfig(Config config, const char* filePath);