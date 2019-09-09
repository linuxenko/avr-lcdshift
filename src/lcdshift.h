/*********************************************************************************
  * File Name     : lcdshift.h
  * Created By    : Svetlana Linuxenko
  * Creation Date : [2019-09-08 16:51]
  * Last Modified : [2019-09-09 19:35]
  * Description   : LCD with shift driver for AVR
**********************************************************************************/

/*
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef _LCDSHIFT_H_
#define _LCDSHIFT_H_

#include "shiftout.h"

/*
 * The display just an object
 */
typedef struct _lcd_def {
  /*
   * Preconfigured "shift" IC as a display driver.
   * Could be any supported type.
   */
  ShiftIC *ic;

  /*
   * Which pin of the "shift" IC coresponds for the display
   * specific lcd pin.
   */
  uint8_t rs;
  uint8_t e;
  uint8_t d0;
  uint8_t d1;
  uint8_t d2;
  uint8_t d3;

  uint8_t cols;
  uint8_t rows;

  /*
   * Just a command buffer for internal usage.
   */
  uint8_t cmd;

} ShiftLCD;

/*
 * A try to configure everything inside of a single function.
 * This function fills a provided ShiftLCD object and applies
 * predefined settings.
 */
ShiftLCD * createShiftLCD(ShiftLCD *lcd, ShiftIC *ic, uint8_t rs, uint8_t e,
    uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
    uint8_t cols, uint8_t rows, uint8_t charsize);

/*
 * Standard interface to LCD.
 */
void shiftLCDSetCursor(ShiftLCD *lcd, uint8_t col, uint8_t row);
void shiftLCDPuts(ShiftLCD *lcd, char *string);
void shiftLCDClear(ShiftLCD *lcd);

/*
 * Low level interface for either configuration or another lcd specific stuff.
 */
void shiftLCDSend(ShiftLCD *lcd, uint8_t value, uint8_t mode);

/*
 * Our precious (:
 */
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

#endif
