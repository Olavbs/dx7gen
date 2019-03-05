// This code is inspired by and made possible by the work
// of tedfelix on sourceforge. I've referenced his source
// code (https://sourceforge.net/u/tedfelix/dx7dump/ci/master/tree/)
// to figure out the structure and workings of the dx7's
// SysEx files. Eddie Lotter's breakdown of SysEx was also
// awfully helpful (http://www.2writers.com/eddie/tutsysex.htm)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

unsigned char name[10];

void generateName() {
	unsigned char uppercase[6] = { 'A', 'E', 'I', 'O', 'U', 'Y' };
	unsigned char lowercase[6] = { 'a', 'e', 'i', 'o', 'u', 'y' };

	int charset = rand() % 36;
	int politician = rand() % 12;

	if(politician > 0) {
		for(int i = 0; i < 10; i++) {
			int toggleChar = rand() % 2;
			if(i == 0) {
				memcpy(name + i, &uppercase[charset / 6], 1);
			} else if(toggleChar == 0) {
				memcpy(name + i, &lowercase[charset / 6], 1);
			} else {
				memcpy(name + i, &lowercase[charset % 6], 1);
			}	
		}
	} else { 
		const char *politicians[15] = {
			"E.Solberg ",
			"GahrStore ",
			"SivJensen ",
			"TrondGiske",
			"Lysbakken ",
			"SkeiGrande",
			"HadiaTajik",
			"C.I.Hagen ",
			"B.Moxnes  ",
			"Bondevik  ",
			"Huitfeldt ",
			"S.Listhaug",
			"Navarsete ",
			"RoeIsaksen",
			"K.Willoch "
		};

		memcpy(name, politicians[rand() % sizeof(&politicians)], 10);	
	}
}
unsigned char sysex[] = {
	0xF0,	// SysEx start
	0x43,	// Yamaha
	0x00,	// Channel
	0x09,	// Format
	0x20,	// sizeMSB
	0x00,	// sizeLSB
};

unsigned char operator[17];

void generateOperator() {
	unsigned char params[17];
	for(int i = 0; i < sizeof(params); i++) {
		if(i <= 10 || i >= 14 ) {
			params[i] = rand() % 64;
		} else if(i == 11) {
			params[i] = rand() % 16;
		} else if(i == 12) {
			params[i] = rand() % 128;
		} else if(i == 13) {
			params[i] = rand() % 32;
		}
	}

	memcpy(operator, params, sizeof(params));
}

unsigned char voice[128]; 

void generateVoice() {
	unsigned char params[16];
	for(int i = 0; i < sizeof(params); i++) {
		if(i == 8) {
			params[i] = rand() % 32;
		} else if(i == 9) {
			params[i] = rand() % 16;
		} else if(i == 14) {
			params[i] = rand() % 128;
		} else {
			params[i] = rand() % 64;
		}
	}
//	unsigned char name[10];
//	for(int i = 0; i < sizeof(name); i++) {
//		name[i] = rand() % 26 + 65;
//	}
	for(int i = 0; i < 6; i++) {
		generateOperator();
		memcpy(	voice + (i * sizeof(operator)),
			operator,
			sizeof(operator)
		);
	}
	memcpy(	voice + (6 * sizeof(operator)),
		params,
		sizeof(params)
	);
	generateName();
	memcpy(	voice + (6 * sizeof(operator)) + sizeof(params),
		name,
		sizeof(name)
	);
}

unsigned char buffer[4104];
unsigned char checksum = 0;

void calculateChecksum() {
	for(unsigned i = 6; i < 4102; i++) {
		checksum += (buffer[i] & 0x7F);
	}
	checksum = (~checksum) + 1;
	checksum &= 0x7F;
}

void build() {
	unsigned int counter = 0;  

	memcpy(buffer + counter, sysex, sizeof(sysex));
	counter += sizeof(sysex);

	for(int i = 0; i < 32; i++) {
		generateVoice();
		memcpy(	buffer + counter,
			voice,
			sizeof(voice)
		);
		counter += sizeof(voice);
	}

	calculateChecksum();
	memcpy(buffer + counter, &checksum, 1);
	counter++;

	memset(buffer + counter, 0xF7, 1);
}

int fileExists(char *filename) {
	struct stat buffer;
	return (stat (filename, &buffer) == 0);
}

int main(int argc, char *argv[]) {

	if(argc < 2) {
		printf("Too few arguments: Please provide a filename\n");
		printf("Usage: dx7gen <outfile>\n");
		return 1;
	} else if(argc > 2) {
		printf("Too many arguments\n");
		printf("Usage: dx7gen <outfile>\n");
		return 1;
	}

	char filename[50];
	strcpy(filename, argv[1]);

	char *dot = strrchr(filename, '.');
	if(dot && !strcmp(dot, ".syx")) {
	} else {
		strcat(filename, ".syx");
	}

	if(fileExists(filename)) {
		printf("%s: File already exists!\n", filename);
		return 1;
	}

	srand (time(NULL));

	build();
	
	FILE *file;
	file = fopen(filename, "wb");
	fwrite(buffer,sizeof(buffer),1,file);
	fclose(file);
	
	return 0;
}
