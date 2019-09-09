/*********************************************************************************
  * File Name     : lcdshift.c
  * Created By    : Svetlana Linuxenko
  * Creation Date : [2019-09-08 18:28]
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

#include <inttypes.h>
#include <util/delay.h>

#include "shiftout.h"
#include "lcdshift.h"

void bitWrite(ShiftLCD *lcd, uint8_t bit, uint8_t bitValue);
void write4bits(ShiftLCD *lcd, uint8_t value);
void transfer();
void pulseEnable(ShiftLCD *lcd);
void command(ShiftLCD *lcd, uint8_t value);
void send(ShiftLCD *lcd, uint8_t value, uint8_t mode);

ShiftLCD * createShiftLCD(ShiftLCD *lcd, ShiftIC *ic, uint8_t rs, uint8_t e,
    uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
    uint8_t cols, uint8_t rows, uint8_t charsize) {

  /*
   * Some default settings. Anyways I like when cursor blinks,
   * but those defaults, you know )
   */
  uint8_t displayControl = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
  uint8_t displayMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

  lcd->cmd = 0;
  lcd->ic = ic;
  lcd->rs = rs;
  lcd->e = e;
  lcd->d0 = d0;
  lcd->d1 = d1;
  lcd->d2 = d2;
  lcd->d3 = d3;

  lcd->cols = cols;
  lcd->rows = rows;

  /*
   * The initialization process requires a lot of delays, sometimes people
   * use rediculously huge values, but I don't see a reason... yet ).
   * */
  _delay_ms(20);
  write4bits(lcd, 0x03);
  _delay_ms(5);
  write4bits(lcd, 0x03);
  _delay_us(110);
  write4bits(lcd, 0x03);
  _delay_us(50);

  write4bits(lcd, 0x02);

  _delay_us(50);

  if (rows > 1) {
    command(lcd, LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS);
  } else if (rows == 1 && charsize != 0) {
    command(lcd, LCD_FUNCTIONSET | LCD_4BITMODE | LCD_1LINE | LCD_5x10DOTS);
  } else {
    command(lcd, LCD_FUNCTIONSET | LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS);
  }

  _delay_ms(1);
  command(lcd, LCD_DISPLAYCONTROL | displayControl);
  _delay_ms(1);

  shiftLCDClear(lcd);
  _delay_ms(1);
  command(lcd, LCD_ENTRYMODESET | displayMode);
  _delay_ms(1);

  return lcd;
}

void transfer(ShiftLCD *lcd) {
  /*
   * The display I have, does not like any long time interruptions.
   * I think it depends on the shift IC. A hc164 IC, for an instance,
   * does not have a "latch" support that made me to make this trick
   */
  if (lcd->ic->type == IC_TYPE_HC164) {
    shiftOn(lcd->ic);
    _delay_us(10);
    shiftOut(lcd->ic, MBFIRST, lcd->cmd);
    _delay_us(10);
    shiftOff(lcd->ic);
  }

  if (lcd->ic->type == IC_TYPE_HC595) {
    shiftOff(lcd->ic);
    _delay_us(10);
    shiftOut(lcd->ic, MBFIRST, lcd->cmd);
    shiftOn(lcd->ic);
    _delay_us(10);
  }
}

void shiftLCDSend(ShiftLCD *lcd, uint8_t value, uint8_t mode) {
  send(lcd, value, mode);
}

void shiftLCDPuts(ShiftLCD *lcd, char *string) {
  char *it = string;

  for (; *it; it++) {
    send(lcd, *it, 1);
  }
}

void shiftLCDSetCursor(ShiftLCD *lcd, uint8_t col, uint8_t row) {
  uint8_t row_offsets[] = {0x00,
    0x40,
    (uint8_t)(0x00 + lcd->cols),
    (uint8_t)(0x40 + lcd->cols)
  };

  if (row > lcd->rows) {
    row = lcd->rows - 1;
  }

  command(lcd, LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void bitWrite(ShiftLCD *lcd, uint8_t bit, uint8_t bitValue) {
  lcd->cmd &= ~(1 << bit);

  if (bitValue != 0) {
    lcd->cmd |= (1 << bit);
  }
}

void shiftLCDClear(ShiftLCD *lcd) {
  command(lcd, LCD_CLEARDISPLAY);
  _delay_us(500);
}

void command(ShiftLCD *lcd, uint8_t value) {
  send(lcd, value, 0);
}

void send(ShiftLCD *lcd, uint8_t value, uint8_t mode) {
  bitWrite(lcd, lcd->rs, mode);
  transfer(lcd);
  write4bits(lcd, value >> 4);
  write4bits(lcd, value);
}

void pulseEnable(ShiftLCD *lcd) {
  bitWrite(lcd, lcd->e, 0);
  transfer(lcd);
  _delay_us(10);
  bitWrite(lcd, lcd->e, 1);
  transfer(lcd);
  _delay_us(10);
  bitWrite(lcd, lcd->e, 0);
  transfer(lcd);
  _delay_us(10);
}

void write4bits(ShiftLCD *lcd, uint8_t value) {
  bitWrite(lcd, lcd->d0, (value >> 0) & 1);
  bitWrite(lcd, lcd->d1, (value >> 1) & 1);
  bitWrite(lcd, lcd->d2, (value >> 2) & 1);
  bitWrite(lcd, lcd->d3, (value >> 3) & 1);
  transfer(lcd);
  pulseEnable(lcd);
}
