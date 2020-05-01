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

#include <hal/debug.h>
#include <pbkit/pbkit.h>
#include <hal/video.h>
#include <hal/xbox.h>
#include <hal/input.h>
#include <hal/io.h>
#include <stdlib.h>
#include <windows.h>
#include <SDL.h>
#include <stdio.h>

#include "main.h"
#include "am29lv160mt.h"
#include "xenium.h"

#define MAXRAM 0x03FFAFFF

//From /hal/debug.c to modify position of cursor
extern int nextRow;
extern int nextCol;

SDL_GameController * gamepad;
static unsigned char * flashData;
static unsigned char * readBackBuffer;

int main() {

    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);
    SDL_Init(SDL_INIT_GAMECONTROLLER);
    gamepad = SDL_GameControllerOpen(0);

    flashData = MmAllocateContiguousMemoryEx(XENIUM_FLASH_SIZE, 0, MAXRAM,
                                             0, PAGE_READWRITE | PAGE_NOCACHE);
    readBackBuffer = MmAllocateContiguousMemoryEx(XENIUM_FLASH_SIZE, 0, MAXRAM,
                                                  0, PAGE_READWRITE | PAGE_NOCACHE);

    int initError = pb_init();
    if (initError != 0) {
        Sleep(2000);
        return 1;
    }

    pb_show_debug_screen();
    drawMainMenu();

    //Record the bank used to get into this program. Useful when rebooting.
    //If not a valid bank, probably booted with a non Xenium device. In this case
    //just set it to Bank 1 - Cromwell bootloader for Xenium.
    unsigned char initialBiosBank = IoInputByte(0xEF) & 0x0F;
    if (initialBiosBank > 10 || initialBiosBank == 0) {
        initialBiosBank = XENIUM_BANK_CROMWELL;
    }

    while (1) {
        SDL_GameControllerUpdate();

        /*DUMP XENIUM FLASH MEMORY*/
        if (getButton(gamepad, SDL_CONTROLLER_BUTTON_START) &&
            getButtonActivated(gamepad, SDL_CONTROLLER_BUTTON_X)) {

            debugPrint("Dumping 2MB of the Xenium Flash memory\n");
            Sleep(1000);

            if (checkForXenium() == 0) {
                debugPrint("Xenium device NOT detected! Something is wrong\n");
            } else {
                memset(flashData, 0x00, XENIUM_FLASH_SIZE);
                debugPrint("Dumping Xenium to 'flash.bin'...\n\n");
                dumpXenium(flashData);

                FILE * f0 = fopen("D:\\flash.bin", "wb");
                if (f0 == NULL) {
                    debugPrint("Error creating file flash.bin. No files written\n");
                } else {
                    debugPrint("Writing file...\n");
                    int chunkSize = 256;
                    for (int i = 0; i < XENIUM_FLASH_SIZE; i += chunkSize) {
                        fwrite(&flashData[i], 1, chunkSize, f0);
                        fflush(f0);
                    }
                    fclose(f0);
                    debugPrint("Dumping to flash.bin complete!\n\n");
                }
            }

            debugPrint("Press BACK to return to the main menu\n");
            waitForButton(gamepad, SDL_CONTROLLER_BUTTON_BACK);
            drawMainMenu();
        }

        /* WRITE A RAW XENIUM FLASH DUMP TO FLASH MEMORY */
        else if (getButton(gamepad, SDL_CONTROLLER_BUTTON_START) &&
                 getButtonActivated(gamepad, SDL_CONTROLLER_BUTTON_B)) {

            debugPrint("Write a raw 2MB flash dump the Xenium device\n");
            debugPrint("DO NOT POWER OFF THE XBOX\n");
            Sleep(1000);

            FILE * f0 = fopen("D:\\flash.bin", "rb");

            if (f0 == NULL) {
                debugPrint("flash.bin not found. Reboot Xbox and ensure\na 2MB flash.bin is located in the xbe directory.\n");
            } else if (checkForXenium() == 0) {
                debugPrint("Xenium device NOT detected! Something is wrong\n");
            } else {
                memset(flashData, 0x00, XENIUM_FLASH_SIZE);
                memset(readBackBuffer, 0x00, XENIUM_FLASH_SIZE);

                debugPrint("Reading flash.bin\n\n");
                fread(flashData, 1, XENIUM_FLASH_SIZE, f0);
                fclose(f0);
                Sleep(2000);
                debugPrint("Ready to flash Xenium.\nThe flash memory needs to be erased first\n");
                debugPrint("Press BLACK to begin ERASE\n");
                waitForButton(gamepad, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
                debugPrint("\n\nPerforming flash memory erase. This can take up to 1 min\n\n");
                Sleep(500);
                flashFullErase();
                writeXeniumRaw(flashData);
                debugPrint("Write Complete. Performing verification readback\n");
                dumpXenium(readBackBuffer);

                //Blank the write protected sector which is on a Genuine Xenium.
                //This will prevent verification errors on a Genuine xenium.
                memset(&readBackBuffer[0x1C0000], 0x00, 0x20000);
                memset(&flashData[0x1C0000], 0x00, 0x20000);
                if (memcmp(readBackBuffer, flashData, XENIUM_FLASH_SIZE) != 0) {
                    debugPrint("ERROR VERIFYING DATA. Try again\n");
                } else {
                    debugPrint("Verification Successful!!\n");
                }
            }

            if (f0 != NULL) {
                fclose(f0);
            }

            debugPrint("Press BACK to return to the main menu\n");
            waitForButton(gamepad, SDL_CONTROLLER_BUTTON_BACK);
            drawMainMenu();

        }

        /* PARSE AND WRITE A XOS UPDATE BINARY TO Xenium flash memory */
        else if (getButton(gamepad, SDL_CONTROLLER_BUTTON_START) &&
                 getButtonActivated(gamepad, SDL_CONTROLLER_BUTTON_Y)) {

            debugPrint("Write a Xos update file to the Xenium device\n");
            debugPrint("DO NOT POWER OFF THE XBOX\n");
            Sleep(1000);

            FILE * f0 = fopen("D:\\recovery.bin", "rb");

            if (f0 == NULL) {
                debugPrint("recovery.bin not found. Reboot Xbox and ensure\n");
                debugPrint("a 1MB recovery.bin is located in the xbe directory.\n");
            } else if (checkForXenium() == 0) {
                debugPrint("Xenium device NOT detected! Something is wrong\n");
            } else {
                memset(flashData, 0xFF, XENIUM_FLASH_SIZE);
                memset(readBackBuffer, 0x00, XENIUM_FLASH_SIZE);

                debugPrint("Reading recovery.bin\n\n");
                fread(readBackBuffer, 1, XENIUM_SIZEOF_UPDATE, f0);
                fclose(f0);
                Sleep(2000);
                //CROMWELL LOADER - Exists at offset 0x00948 in recovery.bin
                memcpy(&flashData[0x180000], & readBackBuffer[0x00948], 0x40000);
                //XENIUMOS PART1 - Exists at offset 0x40948 in recovery.bin
                memcpy(&flashData[0x100000], & readBackBuffer[0x40948], 0x80000);
                //XENIUMOS PART2 - Exists at offset 0xE0948 in recovery.bin
                memcpy(&flashData[0x1E0000], & readBackBuffer[0xE0948], 0x20000);

                debugPrint("Ready to flash Xenium.\nThe flash memory needs to be erased first\n");
                debugPrint("Press BLACK to begin ERASE\n");
                waitForButton(gamepad, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
                debugPrint("\n\nPerforming flash memory erase. This can take up to 1 min\n\n");
                Sleep(500);
                flashFullErase();
                writeXeniumRaw(flashData);
                debugPrint("Write Complete. Performing verification readback\n");
                memset(readBackBuffer, 0x00, XENIUM_FLASH_SIZE);
                dumpXenium(readBackBuffer);

                //Blank write protected sector. This will prevent verification errors on a real xenium.
                memset(&readBackBuffer[0x1C0000], 0x00, 0x20000);
                memset(&flashData[0x1C0000], 0x00, 0x20000);
                if (memcmp(readBackBuffer, flashData, XENIUM_FLASH_SIZE) != 0) {
                    debugPrint("ERROR VERIFYING DATA. Try again\n");
                } else {
                    debugPrint("Verification Successful!!\n");
                }
            }

            if (f0 != NULL) {
                fclose(f0);
            }
            debugPrint("Press BACK to return to the main menu\n");
            waitForButton(gamepad, SDL_CONTROLLER_BUTTON_BACK);
            drawMainMenu();
        }

        /* TOGGLE RGB LED ON EVERY PRESS OF A */
        else if (getButtonActivated(gamepad, SDL_CONTROLLER_BUTTON_A)) {
            static unsigned char ledcnt = 0;
            //Store current cursor position
            int row = nextRow;
            int col = nextCol;
  
            IoOutputByte(XENIUM_REGISTER_LED, ledcnt);
            debugPrint("LED ");
            switch (ledcnt) {
            case 0:
                debugPrint("Off");
                break;
            case 1:
                debugPrint("Red");
                break;
            case 2:
                debugPrint("Green");
                break;
            case 3:
                debugPrint("Amber");
                break;
            case 4:
                debugPrint("Blue");
                break;
            case 5:
                debugPrint("Purple");
                break;
            case 6:
                debugPrint("Teal");
                break;
            case 7:
                debugPrint("White");
                break;
            }
            debugPrint("        ");
            
            //Revert cursor back to original position
            nextRow = row;
            nextCol = col;

            if (++ledcnt > 7) {
                ledcnt = 0;
            }
        }

        /* REBOOT XBOX NORMALLY */
        else if (getButton(gamepad, SDL_CONTROLLER_BUTTON_START) &&
                 getButtonActivated(gamepad, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)) {

            debugPrint("BLACK Pressed - Reboot Xbox\n");
            IoOutputByte(XENIUM_REGISTER_BANKING, initialBiosBank);
            Sleep(1000);
            break;
        }

        //Recheck for Xenium device. Basically redraw the main menu
        else if (getButtonActivated(gamepad, SDL_CONTROLLER_BUTTON_BACK)) {

            debugPrint("Checking for Xenium...\n");
            Sleep(500);
            drawMainMenu();
        }
    }

    SDL_GameControllerClose(gamepad);
    pb_kill();
    XReboot();
    return 0;
}

unsigned char getButton(SDL_GameController * pad, SDL_GameControllerButton button) {
    return SDL_GameControllerGetButton(pad, button);
}

void waitForButton(SDL_GameController * pad, SDL_GameControllerButton button) {
    do {
        SDL_GameControllerUpdate();
        Sleep(50);
    } while (getButton(pad, button) == 0);
}

unsigned char getButtonActivated(SDL_GameController * pad, SDL_GameControllerButton button) {
    static unsigned short buttonState = 0x0000;
    unsigned char ret = 0;

    //Exit on invalid inputs
    if (button == -1 || button >= 16) {
        return 0;
    }

    //Return 1 if button is pressed, and the buttonState was previously 0.
    if (SDL_GameControllerGetButton(pad, button)) {
        if (!(buttonState & (1 << (unsigned short) button))) {
            ret = 1;
        }
        buttonState |= 1 << (unsigned short) button; //Set buttonState
    } else {
        buttonState &= ~(1 << (unsigned short) button); //Clear buttonState
    }

    return ret;
}

void debugPrintXY(char * str, int x, int y) {
    int row = nextRow;
    int col = nextCol;
    nextCol = x;
    nextRow = y;
    debugPrint(str);
    nextRow = row;
    nextCol = col;
}

void drawMainMenu(void) {
    debugClearScreen();
    debugPrint("Xenium Tools by Ryzee119\n");
    debugPrint("https://github.com/Ryzee119/OpenXenium\n\n");
    if (checkForXenium() == 1) {
        debugPrint("Xenium Detected!\n");
        
    } else {
        debugPrint("Xenium not Detected!\n");
    }
    debugPrint("_______________________________________________________________\n\n");

    debugPrint("START+X: Dump full 2MB of Xenium Flash Memory\n");
    debugPrint("START+B: Write a raw 'flash.bin' to Xenium Flash Memory\n");
    debugPrint("START+Y: Write XOS update 'recovery.bin' to Xenium Flash Memory\n");
    debugPrint("A:       Cycle the RGB LED\n");
    debugPrint("BACK:    Recheck for Xenium device\n\n");

    debugPrint("START+BLACK: Reboot Xbox\n");
    debugPrint("_______________________________________________________________\n");
    
    debugPrintXY("Built with NXDK https://github.com/XboxDev/nxdk/", 25, 480 - 50 - 8); //Screen height-margins-font height
}

unsigned char checkForXenium() {
    unsigned char temp[8];
    unsigned char manuf, devid;

    //A genuine xenium needs something similar to a real Xos boot process to start working
    //Must be something quirky in the CPLD. This seems to work:
    LPCmemoryRead(temp, 0x70, 8); //Couple reads just to get Xenium going
    IoOutputByte(XENIUM_REGISTER_BANKING, XENIUM_BANK_XENIUMOS);
    LPCmemoryRead(temp, 0x70, 8); //Couple reads just to get Xenium going
    IoOutputByte(XENIUM_REGISTER_BANKING, XENIUM_BANK_RECOVERY);
    LPCmemoryRead(temp, 0x70, 8); //Couple reads just to get Xenium going
    IoOutputByte(XENIUM_REGISTER_BANKING, XENIUM_BANK_CROMWELL);

    flashReset();

    manuf = getManufID();
    devid = getDevID();

    debugPrint("Manufacturer ID %02x, device ID %02x\n", manuf, devid);
    if ((manuf == XENIUM_MANUF_ID && devid == XENIUM_DEVICE_ID) ||
        (manuf == XENIUM_LASTBLAST_MANUF_ID && devid == XENIUM_LASTBLAST_ID)) {
        return 1;
        
    } else {
        return 0;
    }
}

char dumpXenium(unsigned char * buffer) {
    unsigned int address = 0;
    const unsigned short chunk_size = 128;

    for (unsigned int rawAddress = 0; rawAddress < XENIUM_FLASH_SIZE; rawAddress += chunk_size) {
        if (rawAddress == 0x000000) {
            debugPrint("Reading bank %x\r\n", rawAddress);
            IoOutputByte(XENIUM_REGISTER_BANKING, XENIUM_BANK_1_1024);
            address = 0;
        } else if (rawAddress == 0x100000) {
            debugPrint("Reading bank %x\r\n", rawAddress);
            IoOutputByte(XENIUM_REGISTER_BANKING, XENIUM_BANK_XENIUMOS);
            address = 0;
        } else if (rawAddress == 0x180000) {
            debugPrint("Reading bank %x\r\n", rawAddress);
            IoOutputByte(XENIUM_REGISTER_BANKING, XENIUM_BANK_CROMWELL);
            address = 0;
        } else if (rawAddress == 0x1C0000) {
            debugPrint("Reading bank %x\r\n", rawAddress);
            IoOutputByte(XENIUM_REGISTER_BANKING, XENIUM_BANK_RECOVERY);
            address = 0;
        }
        LPCmemoryRead( & buffer[rawAddress], address, chunk_size);
        address += chunk_size;
    }
    debugPrint("Read complete\r\n");
    return 0;
}

void writeXeniumRaw(unsigned char * buffer) {
    unsigned int bankSize = 0;

    for (unsigned int rawAddress = 0; rawAddress < XENIUM_FLASH_SIZE; rawAddress += bankSize) {
        if (rawAddress == 0x000000) {
            debugPrint("Writing bank %x\r\n", rawAddress);
            IoOutputByte(XENIUM_REGISTER_BANKING, XENIUM_BANK_1_1024);
            bankSize = 0x100000;
            
        } else if (rawAddress == 0x100000) {
            debugPrint("Writing bank %x\r\n", rawAddress);
            IoOutputByte(XENIUM_REGISTER_BANKING, XENIUM_BANK_XENIUMOS);
            bankSize = 0x80000;
            
        } else if (rawAddress == 0x180000) {
            debugPrint("Writing bank %x\r\n", rawAddress);
            IoOutputByte(XENIUM_REGISTER_BANKING, XENIUM_BANK_CROMWELL);
            bankSize = 0x40000;
            
        } else if (rawAddress == 0x1C0000) {
            debugPrint("Writing bank %x\r\n", rawAddress);
            IoOutputByte(XENIUM_REGISTER_BANKING, XENIUM_BANK_RECOVERY);
            bankSize = 0x40000;
        }

        const unsigned int chunk = 1024;
        unsigned int bankAddress = 0;
        while (bankAddress < bankSize) {

            for (unsigned int i = 0; i < chunk; i++) {
                flashProgramByte(bankAddress, buffer[rawAddress + bankAddress]);
                bankAddress++;
            }
            
            //Calculate and display a percent complete.
            char str[] = "Complete: XXX%";
            sprintf(str, "Complete: %3u%%",
                (unsigned char)((float)(rawAddress + bankAddress) / (float) XENIUM_FLASH_SIZE * 100.0));
            debugPrintXY(str, 25, 480 - 50 - 8 - 12); //Second from bottom row.
        }
    }
}