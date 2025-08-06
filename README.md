# Bad Apple Ascii Terminal

Play bad apple on terminal.

# Installation

This program requires mpg123 installed. Install it with:

- sudo apt install mpg123 (on Debian/Ubuntu)
- sudo pacman -S mpg123 (on Arch based)

Then install the program with:

- make install

Run "badapple" on terminal and enjoy :D

# Notes

This program was only tested on my machine, which is a Debian 12 with 
1600x900 resolution. It might not work on Windows. A port is to be made.

For a better experience, play it on fullscreen and adjust the font size.

# Usage

## badapple.c

This is the main program, responsible for streaming assets/badapple.dat frames
with specified char resolution.

## process_frames.py

This program is used for converting the bad apple frames into a specified char
resolution. To use it, activate the virtual env with:

- source bin/activate

Then run:

- python3 src/process_frames.py

This will replace assets/badapple.dat. Accordingly modify SCREEN_HEIGHT and 
SCREEN_WIDTH on badapple.c to transmit the video correctly.

This python file requires natsort and PIL, so if you're unable to activate
the venv for any reason, install those packages system-wide or create your
own venv to install.