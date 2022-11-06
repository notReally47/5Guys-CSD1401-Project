#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <errno.h>
#include "structs.h"
#include "defines.h"
#include "settings.h"
#include "cprocessing.h"

/*Reads config.ini*/
Config readFile() {
	/*Default settings if anything goes wrong. Will return this if anything goes wrong with reading.*/
	Config gameConfig = {
	.save.isNewState = 1,
	.save.lastLevelPlayed = 1,
	.settings.windowed = 1,
	.settings.audio = 50,
	.settings.resolutionHeight = (float)CP_System_GetDisplayHeight() / 2.f,
	.settings.resolutionWidth = (float)CP_System_GetDisplayWidth() / 2.f,
	.keybinds.moveUp = 'W',
	.keybinds.moveLeft = 'A',
	.keybinds.moveDown = 'S',
	.keybinds.moveRight = 'D',
	.keybinds.undoMove = 'U',
	.keybinds.resetMap = 'R'
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