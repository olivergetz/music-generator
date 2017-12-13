//This file generates a random number that represents a scale degree.

#include <stdlib.h>

//Prints out a random note number in semitones 0-12 from one of a number of scales (scaleNum).
int noteGenerator(int scaleNum){ //scaleNum 0-1.

	//Defined locally for use in this function without disturbing other functions.
	int randomInt = 0;
	int note;

	//Here I use a multidimensional array to store scales that I can choose from later.
	//I define it here because I shouldn't have to change it anywhere else in the program.
	int scaleContainer[2][8] = {

		{0,2,4,5,7,9,11,12}, //Ionian scale
		{0,2,3,5,7,8,10,12} //Aeolian scale

	};

	randomInt = rand() % 8;

	note = scaleContainer[scaleNum][randomInt];

	return note;

}


