# Antari


Antari is an Atari 2600 emulator that I started as a way to understand the hardware of the VCS console.
My initial goal was to be able to render the backgrounds (playfield).
This version can render all sprites (though missiles are very broken), allow player movement, detect collisions, play sounds, and much, much more!

  - Built on SDL2 framework
  - 2k and 4k rom support

# New Features!
  - Square wave sound effects.
## Build Linux/Mac OS
``` # ./make.sh ```

## Usage

``` # ./gormtari <rom-name>```

## TODO
 - Fix whatever is happening with missiles. They seem to be at weird diagonals and the placement is off.
 - Implement the duplicate sprite functionality
 - Something is off with the horizontal player placement, hard-coding an offset of 16 color clocks to compensate.
 - Implement additional audio waveforms

*Note: this is a work in progress and will not perform as expected.*
