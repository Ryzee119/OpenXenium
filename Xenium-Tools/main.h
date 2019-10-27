/*
 * xenium-tools is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * This software has been developed using NXDK. The cross-platform, open-source SDK to develop for Original Xbox
 * See https://github.com/XboxDev/nxdk
 */

/*
 * Function:  drawMainMenu 
 * --------------------
 * Checks for Xenium compatible hardware and redraws the main menu
 *    clearing any existing on screen text in the process
 *
 * returns: void
 */
void drawMainMenu(void);

/*
 * Function:  getButton 
 * --------------------
 * Checks if a button is currently being pressed.
 *
 * input1: Pointer to the SDL GameController handle
 * input2: SDL_GameControllerButton definition enum
 * 
 * returns: 1 if button is being pressed. 0 otherwise.
 */
unsigned char getButton(SDL_GameController * , SDL_GameControllerButton);

/*
 * Function:  getButton 
 * --------------------
 * Spinlocks until the specified button is pressed.
 *
 * input1: Pointer to the SDL GameController handle
 * input2: SDL_GameControllerButton definition enum
 * 
 * returns: void.
 */
void waitForButton(SDL_GameController * , SDL_GameControllerButton);

/*
 * Function:  getButton 
 * --------------------
 * Check if a button has just been actived (from 0 to 1)
 *
 * input1: Pointer to the SDL GameController handle
 * input2: SDL_GameControllerButton definition enum
 * 
 * returns: 1 if the button has been and is being last call,
 * subsequent calls and calls when the button is NOT pressed will return 0
 */
unsigned char getButtonActivated(SDL_GameController * , SDL_GameControllerButton);

/*
 * Function:  checkForXenium 
 * --------------------
 * Checks the LPC port for Xenium compatible hardware.
 * this is achieved by reading the flashmemory device ID and
 * manufacturer ID and comparing them to known values.
 * 
 * returns: 1 if Xenium compatible hardware is present. 0 otherwise.
 */
unsigned char checkForXenium(void);

/*
 * Function:  dumpXenium 
 * --------------------
 * Dumps the full 2MB flash contents of a Xenium compatible device
 * to an input buffer.
 * WARNING: The user must ensure sufficient memory is allocated for the buffer.
 * 
 * input1: A pointer to an empty 2MB rx buffer
 *
 * returns: 0.
 */
char dumpXenium(unsigned char * );

/*
 * Function:  writeXeniumRaw 
 * --------------------
 * Writes a buffer to the 2MB flash contents of a Xenium compatible device
 * WARNING: The user must ensure sufficient memory is allocated for the buffer.
 * 
 * input1: A pointer to a 2MB tx buffer containing data to write
 *
 * returns: void.
 */
void writeXeniumRaw(unsigned char * buffer);

/*
 * Function:  writeXeniumRaw 
 * --------------------
 * Prints a preformatted string to the screen using debugPrint() from hal/debug.h
 * to any X,Y position on the screen. The cursor position is returned to its previous
 * position after printing.
 * 
 * input1: A pointer to a 2MB tx buffer containing data to write
 * input2: X position of the first character
 * input3: Y position of the first character
 *
 * returns: void.
 */
void debugPrintXY(char * str, int x, int y);
