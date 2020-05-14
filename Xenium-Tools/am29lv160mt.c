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
 
#include "am29lv160mt.h"
#include "xenium.h"

//The flash device used on Xenium and OpenXenium is a AM29LV160MT by Spansion (or equivalent).
//These functions should be read in confunction with the relevant datasheet
//https://www.dataman.com/media/datasheet/Spansion/AM29LV160M.pdf 

//Memory calls in the range 0xFF000000 to 0xFFFFFFFF will be read from the LPC port flash rom
//if enabled during boot. (D0 held low) Otherwise from the TSOP flash memory on the motherboard.
//The CPLD on the Xenium will see these commands and read/write the flash chip as required.
//http://xboxdevwiki.net/Memory
static unsigned char LPCmemoryReadByte(unsigned long address){
    unsigned char* lpc_mem_map = (unsigned char *)LPCFlashAddress;
    return lpc_mem_map[address];
}

//Two successive LPC reads. DQ6 will toggle if still in progress
//Returns 1 if busy, 0 is not.
static unsigned char flashBusy(){
    return (LPCmemoryReadByte(0x00)!=LPCmemoryReadByte(0x00));
}

static void flashReset() {
    LPCmemoryWrite(0x000, 0xF0);
}

void LPCmemoryRead(unsigned char* rxbuff, unsigned long address, unsigned long len){
    memcpy(rxbuff,(void*)(LPCFlashAddress + address), len);
}

void LPCmemoryWrite(unsigned long address, unsigned char data) {
    memcpy((void*)(LPCFlashAddress + address),&data, 1);
}

//Sector address should fall in the sector you want to erase.
void flashSectorErase(unsigned long sector_address) {
    LPCmemoryWrite(0xAAA, 0xAA);
    LPCmemoryWrite(0x555, 0x55);
    LPCmemoryWrite(0xAAA, 0x80);
    LPCmemoryWrite(0xAAA, 0xAA);
    LPCmemoryWrite(0x555, 0x55);
    LPCmemoryWrite(sector_address, 0x30);
    while(flashBusy());
}

void flashFullErase() {
    LPCmemoryWrite(0xAAA, 0xAA);
    LPCmemoryWrite(0x555, 0x55);
    LPCmemoryWrite(0xAAA, 0x80);
    LPCmemoryWrite(0xAAA, 0xAA);
    LPCmemoryWrite(0x555, 0x55);
    LPCmemoryWrite(0xAAA, 0x10);
    while(flashBusy()){
        Sleep(500);
    }
}

//Writes a single byte to a previously erased section.
//This function can be sent in isolated provided the byte as already been erased.
void flashProgramByte(unsigned long address, unsigned char data) {
    LPCmemoryWrite(0xAAA, 0xAA);
    LPCmemoryWrite(0x555, 0x55);
    LPCmemoryWrite(0xAAA, 0xA0);
    LPCmemoryWrite(address, data);
    while(flashBusy());
}

unsigned char getManufID() {
    unsigned char manuf;
    LPCmemoryWrite(0xAAA, 0xAA);
    LPCmemoryWrite(0x555, 0x55);
    LPCmemoryWrite(0xAAA, 0x90);
    manuf = LPCmemoryReadByte(0x00);
    flashReset();
    return manuf;
}

//Returns the deviceID of the flash memory chip
//Some memory devices has the deviceID located
//at address 1. So if the read fails I try at address 1 instead
unsigned char getDevID() {
    unsigned char devID;
    LPCmemoryWrite(0xAAA, 0xAA);
    LPCmemoryWrite(0x555, 0x55);
    LPCmemoryWrite(0xAAA, 0x90);
    devID = LPCmemoryReadByte(0x02);
    flashReset();
    return devID;
}





