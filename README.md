## LCD with shift driver support for AVR

Three wire LCD driver support for AVR using 74hc595 or 74hc164.

[![GPL Licence](https://badges.frapsoft.com/os/gpl/gpl.png?v=103)](https://opensource.org/licenses/GPL-3.0/)

## Why ?
  I couldn't find any code for LCD with shift driver for `avr-gcc`. Then I've decided to
  make an own with small codebase, multiple displays and different driver ICs support.

## Features

  * AVR-GCC support
  * Small codebase
  * Different hardware implementations support (tested with 74hc595 and 74hc164)
  * Multiple displays in a single runtime support (on different ports of an AVR)

## Known issues

  * The library has only 3 wire connections support. It is quite difficult to make
  it work with 2 wire connection and keep the codebase small and simple.

## Installation

The driver depends on [shiftout](https://github.com/linuxenko/avr-shiftout) library, that you can 
simple copy into your project's folder.
Then copy source files of this project.
Don't forget to change your build system's settings make it able to recognize them.

## Usage

Using 595 driver.

  * Schematic 

![595 shcematic](https://raw.githubusercontent.com/linuxenko/avr-lcdshift/dev/schematic/595-schematic.png)

  * Breadboard

![595 breadboard](https://raw.githubusercontent.com/linuxenko/avr-lcdshift/dev/schematic/595-bread.png)

The following code should drive these boards:

```c
#include "shiftout.h"
#include "lcdshift.h"

int main(void) {
  /* Our shift ic */
  ShiftIC icS;

  /* LCD object */
  ShiftLCD lcd;

  createShift(&icS, &DDRD, &PORTD, PORTD0, PORTD1, PORTD2);

  /* Type of driver must be specified explicitly */
  icS.type = IC_TYPE_HC595;

  /* Lets create it */
  createShiftLCD(&lcd, &icS, 1, 3, 4, 5, 6, 7, 16, 2, 0);

  /* Ta-da ! */
  shiftLCDPuts(&lcd, "hello");

  return 0;
}
```

The same using 74hc164

![164 breadboard](https://raw.githubusercontent.com/linuxenko/avr-lcdshift/dev/schematic/164-schematic.png)

```c
#include "shiftout.h"
#include "lcdshift.h"

int main(void) {
  /* Our shift ic */
  ShiftIC icS;

  /* LCD object */
  ShiftLCD lcd;

  createShift(&icS, &DDRD, &PORTD, PORTD0, PORTD1, PORTD2);

  /* Type of driver must be specified explicitly */
  icS.type = IC_TYPE_HC164;

  /* Lets create it */
  createShiftLCD(&lcd, &icS, 4, 5, 0, 1, 2, 3, 16, 2, 0);

  /* Ta-da ! */
  shiftLCDPuts(&lcd, "hello");

  return 0;
}
```

## API

```c
ShiftLCD * createShiftLCD(ShiftLCD *lcd, ShiftIC *ic, uint8_t rs, uint8_t e,
    uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
    uint8_t cols, uint8_t rows, uint8_t charsize);

void shiftLCDSetCursor(ShiftLCD *lcd, uint8_t col, uint8_t row);
void shiftLCDPuts(ShiftLCD *lcd, char *string);
void shiftLCDClear(ShiftLCD *lcd);

void shiftLCDSend(ShiftLCD *lcd, uint8_t value, uint8_t mode);
```


## License

  This program is free software: you can redistribute it and/or modify<br>
  it under the terms of the GNU General Public License as published by<br>
  the Free Software Foundation, either version 3 of the License, or<br>
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,<br>
  but WITHOUT ANY WARRANTY; without even the implied warranty of<br>
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the<br>
  GNU General Public License for more details.<br>

  You should have received a copy of the GNU General Public License<br>
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
