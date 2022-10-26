#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include "structs.h"

Config readConfig(const char* filePath, const char* mode) {
	FILE* configFile;
	configFile = fopen(filePath, mode);

	Config config = {
		.save = {
			.isNewState = 1,
			.lastLevelPlayed = 1
		},
		.settings = {
			.audio = 50,
			.resWidth = 1920,
			.resHeight = 1080
		},
		.keybinds = {
			.moveUp = KEY_W,
			.moveLeft = KEY_A,
			.moveDown = KEY_S,
			.moveRight = KEY_D,
			.undoMove = KEY_U,
			.resetMap = KEY_R,
			.exitGame = KEY_ESCAPE
		}
	};

	if (configFile == NULL) {
		fprintf(stderr, "Error opening file.");
	}
	else {
		fread(&config, sizeof(config), 1, configFile);
	}

	if (configFile != NULL) {
		fclose(configFile);
	}

	return config;
}

void createConfig(const char* filePath, const char* mode) {
	FILE* configFile;
	configFile = fopen(filePath, mode);

	Config config = {
		.save = {
			.isNewState = 1,
			.lastLevelPlayed = 0
		},
		.settings = {
			.audio = 50,
			.resWidth = 1920,
			.resHeight = 1080
		},
		.keybinds = {
			.moveUp = KEY_W,
			.moveLeft = KEY_A,
			.moveDown = KEY_S,
			.moveRight = KEY_D,
			.undoMove = KEY_U,
			.resetMap = KEY_R,
			.exitGame = KEY_ESCAPE
		}
	};

	if (configFile == NULL) {
		fprintf(stderr, "Error opening file.");
	}
	else {
		fwrite(&config, sizeof(config), 1, configFile);
	}

	if (configFile != NULL) {
		fclose(configFile);
	}
}

void updateConfig(Config config, const char* filePath) {
	FILE* configFile;
	configFile = fopen(filePath, "w");

	if (configFile == NULL) {
		fprintf(stderr, "Error opening file.");
	}
	else {
		fwrite(&config, sizeof(config), 1, configFile);
	}

	if (configFile != NULL) {
		fclose(configFile);
	}
}