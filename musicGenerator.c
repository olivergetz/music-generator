/* 

	Description: This program writes music to a csd file.

	Run: change to current directory and type 'make'.

	Oliver Getz Rodahl
	Audio Programming in C

*/

//TODO: Melodynote = 3rd or 5th of a chord. Choose randomly.
//TODO: User prompts.
//TODO: Split code into 3 files: chords, melody, and performer. Link together using makefile.
//TODO: Use commandline arguments to determine the length of the piece.
//TODO: Stricter melodic rules.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "musicGenerator.h"

//Header Text
#define LINE   "		_____________________________________"
#define TITLE  "			  ~Music Generator~"
#define AUTHOR "		 	by Oliver Getz Rodahl"

//Terms
#define true 1

//Define some variables
char csdFileName[256] = "Generated-Music.csd";	//Used to store the name of the desired csd file.
int noteAmount = 17; //Amount of pitches to generate.
int tempo = 120;
float duration = 0.5;
float octave = 9.0; //Determines which octave the melody is starting from.

//Pointers
FILE * csdFilePointer;

// function prototypes
void printHeader();
void printScore();
void printOrchestra();
void melodyGen();
void chordGen();
void metronome();

//Create header
void printHeader(){

	printf("%s\n\n", LINE);
	printf("%s\n", TITLE);
	printf("%s\n", AUTHOR);
	printf("%s\n\n", LINE);

}

//Relies on the printScore function. Prints a patch to be used in Csound.
void printOrchestra(){

	//Print orchestra and score to CSD file.
	fprintf(csdFilePointer, "<CsoundSynthesizer>\n");
	fprintf(csdFilePointer, "<CsOptions>\n");
	fprintf(csdFilePointer, "-odac\n");
	fprintf(csdFilePointer, "</CsOptions>\n");
	fprintf(csdFilePointer, "<CsInstruments>\n");
	fprintf(csdFilePointer, "0dbfs = 1\n");

	fprintf(csdFilePointer, "instr 1\n"); //Instrument used for melody
	fprintf(csdFilePointer, "iamp = .5\n");
	fprintf(csdFilePointer, "ifreq = cpspch(p5)\n");
	fprintf(csdFilePointer, "ifn = 1 ;f table number\n");
	fprintf(csdFilePointer, "asig oscil iamp, ifreq, ifn\n");
	fprintf(csdFilePointer, "kenv adsr 0.0005, .05, 1, .05\n"); //ENVELOPE
	fprintf(csdFilePointer, "out asig * kenv\n");
	fprintf(csdFilePointer, "endin\n\n");

	fprintf(csdFilePointer, "instr 2\n"); //Instrument used for chords
	fprintf(csdFilePointer, "iamp = .3\n");
	fprintf(csdFilePointer, "ifreq = cpspch(p5)\n");
	fprintf(csdFilePointer, "ifn = 2 ;f table number\n");
	fprintf(csdFilePointer, "asig oscil iamp, ifreq, ifn\n");
	fprintf(csdFilePointer, "out asig\n");
	fprintf(csdFilePointer, "endin\n\n");

	fprintf(csdFilePointer, "</CsInstruments>\n");
	printScore(tempo); //Prints a score.
	fprintf(csdFilePointer, "</CsoundSynthesizer>\n");

}

void printScore(int bpm){

	//Open the score
	fprintf(csdFilePointer, "<CsScore>\n");
	fprintf(csdFilePointer, "f1 0 16384 10 .5 ;table number, don't know, resolution, waveform, harmonics\n");
	fprintf(csdFilePointer, "f2 0 16384 10 .5 ;table number, don't know, resolution, waveform, harmonics\n");
	fprintf(csdFilePointer, "t 0 %d\n", bpm);

	melodyGen(noteAmount); //generates random notes.
	metronome(noteAmount);

	//Close the score
	fprintf(csdFilePointer, "</CsScore>\n");


}

void melodyGen(int iterations){ //int = how many notes to generate.

	//Defined locally for use in this function without disturbing other functions.
	int randomInt = 0; //Used to semi-randomly determine the next note duration.
	int csdNoteInt = 0;
	float csdNote = 0.0; //The interval to be printed out.

	/* MELODY MODULE */

	//Print a sequence of notes
	for (int i = 0; i < iterations; i++){

		float subdivision;

		//Determines when the next rhythm event will happen: On the next beat, 16th, 8th, or dotted 8th.
		//Warning: Doesn't update float without 'break;'
		switch(randomInt){

			case 0:
				//On the beat.
				subdivision = 0;
				break;
			case 1:
				//On the next 16th
				subdivision = 0.25;
				break;
			case 2:
				//On the next 8th
				subdivision = 0.5;
				break;
			case 3:
				//On the next dotted 8th
				subdivision = 0.75;
				break;
			case 4:
				//On the beat again, to make it more likely for a note to occur on a strong subdivision.
				subdivision = 0;
				break;
			case 5:
				//On the 8th again, to make it more likely for a note to occur on a strong subdivision.
				subdivision = 0.5;
				break;
			default:
				subdivision = 0;
				break;

		}


		//If we're going through the loop for the first or the last time, 
		//ensure that the note generated will be the root of our key.
		if ( (i == 0) || (i == (iterations - 1)) ){

			csdNoteInt = 0;
			csdNote = octave + (round(csdNoteInt));

			//Generate a random int to determine what the duration of the next note will be for the next iteration.
			//We do this after the for-loop to ensure that the first attack will always be on the first beat.
			randomInt = rand() % 6;

		}
		else if (i == (iterations - 2)) {

			//Pick a random note from either a major scale (0) or a minor scale (1)
			csdNoteInt = noteGenerator(0);

			//Csound handles intervals like: "octave.semitones," so we have to move the semitones into their decimal place
			//then add the octave defined at the start of the program.
			csdNote = octave + (round(csdNoteInt) / 100); //Convert int to float

			//If we're on the penultimate beat, make sure the next hit is not a syncopation.
			randomInt = 0;


		}
		else {

			//Pick a random note from either a major scale (0) or a minor scale (1)
			csdNoteInt = noteGenerator(0);

			//Csound handles intervals like: "octave.semitones," so we have to move the semitones into their decimal place
			//then add the octave defined at the start of the program.
			csdNote = octave + (round(csdNoteInt) / 100); //Convert int to float
			

			//Generate a random int to determine what the duration of the next note will be for the next iteration.
			//We do this after the for-loop to ensure that the first attack will always be on the first beat.
			randomInt = rand() % 6;

		}

		//Update subdivision to advance the score by a beat.
		subdivision = subdivision + i;

		//Generate the next note using data from the previous switch statement, a duration variable, and a pitch generator (interval).
		fprintf(csdFilePointer, "i 1 %f %f 7 %f\n", subdivision, duration, csdNote);

		/* END MELODY MODULE */

		/* CHORD MODULE */

		// Play a chord on every 4th beat
		if (i % 4 == 0){

			chordGen(0, csdNoteInt, i);

		}

		/* END CHORD MODULE */

		//UPDATE VARIABLES FOR NEXT LOOP


	}

}

void metronome(int iterations){

	for (int i = 0; i < iterations; i++){

		//A pulse to measure the rhythm to, on every quarter note.
		fprintf(csdFilePointer, "i 1 %d .02 7 7\n", i);

	}

}

//Generate chords based on melodyGen.
void chordGen(int mode, int melodyNote, int beat){

	//Used to figure out which chord to play.
	int randomInt = 0;
	//Which octave to play chords in.
	float chordOctave = 8.0;

	//Make sure the note selected cannot be larger than 11.
	melodyNote = melodyNote % 12;

	//A collection of scales to use when making chords.
	int scaleContainer[2][8] = {

		{0,2,4,5,7,9,11,12}, //Ionian scale
		{0,2,3,5,7,8,10,12} //Aeolian scale

	};

	switch(randomInt){

		case 0:

			//Print a chord where the melody is the root.
			for (int i = 0; i < 6; i+=2){

				//store i in another variable, so the value can be manipulated without ruining the loop.
				int y = i;

				if ((melodyNote + i) < 0) { // If the array index is less than 0, add 8 to the index.

					int y = i + 7;

				}
				else if ((melodyNote + i) > 12) { // If the array index is more than 12, subtract 8 from the index.

					int y = i - 7;

				}	

				// Store the melody note (1-8)
				float baseNote = scaleContainer[mode][melodyNote + y];

				//Divide it so it can be used by cSound.
				baseNote = baseNote / 100;

				//Place the chord in a good octave.
				baseNote = baseNote + chordOctave;

				fprintf(csdFilePointer, "i 2 %d %f 7 %f\n", beat, 2.0, baseNote);
			}

			break;

		case 1:

			//Print a chord where the melody is the third.
			for (int i = -2; i < 4; i+=2){

				//store i in another variable, so the value can be manipulated without ruining the loop.
				int y = i;

				if ((melodyNote + i) < 0) { // If the array index is less than 0, add 8 to the index.

					int y = i + 7;

				}
				else if ((melodyNote + i) > 12) { // If the array index is more than 12, subtract 8 from the index.

					int y = i - 7;

				}	

				// Store the melody note (1-8)
				float baseNote = scaleContainer[mode][melodyNote + y];

				//Divide it so it can be used by cSound.
				baseNote = baseNote / 100;

				//Place the chord in a good octave.
				baseNote = baseNote + chordOctave;

				fprintf(csdFilePointer, "i 2 %d %f 7 %f\n", beat, 2.0, baseNote);
			}

			break;

		case 2:

			//Print a chord where the melody is the fifth.
			for (int i = -4; i < 0; i+=2){

				// Store the melody note (1-8)
				float baseNote = scaleContainer[mode][melodyNote + i];

				//Divide it so it can be used by cSound.
				baseNote = baseNote / 100;

				//Place the chord in a good octave.
				baseNote = baseNote + chordOctave;

				fprintf(csdFilePointer, "i 2 %d %f 7 %f\n", beat, 2.0, baseNote);
			}

			break;


	}



}

int main(int argc, char* argv[]){ //argv[1] = iterations and amount of notes.

	//Print Header
	printHeader();

	//Seed the random number generator at the beginning of the program to avoid repeating rhythmic patterns.
	srand(time(NULL));

	//Create a csd file if none exist.
	csdFilePointer = fopen(csdFileName, "w");

	printOrchestra();

	//Close file pointer
	fclose(csdFilePointer);

	//Play music from cSound. -d hides the cSoundQT interface.
	char commandline[1000] = "csound -odac -d ";
	strcat(commandline, csdFileName);
	system (commandline);

	/* TESTING AREA */



	return 0;

}