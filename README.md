README MusicGenerator

Created by Oliver Getz Rodahl.

About: This program generates 4 bars of music. I removed user prompts for this version out of testing purposes. User prompts will be re-introduced in upcoming versions. This will allow you to specify length of the piece, as well as tonality.

Make sure Csound is installed.

What do?:
1. Run MusicGenerator
2. Enjoy your newly generated piece!

Version 1.5

- Split the program into two parts: A note generator script and a script that derives chords from the melodies. I connected these through a Makefile.
- Fixed a bug where the 2nd melody note would always land on beat 2.
- Wrote a function that works like a metronome.
- Made the program play the music from command line, to avoid having to open CsoundQT.