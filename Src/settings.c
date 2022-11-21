/*
All content © 2022 DigiPen Institute of Technology Singapore, all rights reserved.
Authors	: Ian Chua (i.chua@digipen.edu)
		    - Read file
			- Write file
			- Check if file exists
File	: settings.c 
Purpose	: Read and write game information to the file.
*/

#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <errno.h>
#include "structs.h"
#include "defines.h"
#include "settings.h"
#include "cprocessing.h"
#include "utils.h"

/*Reads config.dat*/
Config readFile() {
	/*Default settings if anything goes wrong. Will return this if anything goes wrong with reading.*/
	Config gameConfig = {
	.save.isNewState = 1,
	.save.lastLevelPlayed = 1,
	.settings.titleBarHeight = getTitleBarHeight(),
	.settings.windowed = 0,
	.settings.audio = 50,
	.settings.resolutionWidth = (unsigned int)GetSystemMetrics(SM_CXSCREEN),
	.settings.resolutionHeight = (unsigned int)GetSystemMetrics(SM_CYSCREEN),
	};

	FILE* file;
	/*READ ONLY*/
	file = fopen("config.dat", "r");

	if (file == NULL) {
		int err = errno;
		/*If file does not exist, create file with default settings.*/
		if (err == ENOENT) {
			writeConfig(gameConfig);
		}
		else {
			fprintf(stderr, "Error opening file.\n");
		}
	}
	else {
		fread(&gameConfig, sizeof(struct Config), 1, file);
	}

	/*Close stream*/
	if (file != NULL) {
		fclose(file);
	}

	return gameConfig;
}

void writeConfig(Config config) {
	FILE* file;
	file = fopen("config.dat", "w");

	fwrite(&config, sizeof(struct Config), 1, file);

	fclose(file);
}