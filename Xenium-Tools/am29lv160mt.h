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
 
#include <windows.h>
#include <string.h>
#include <hal/io.h>
#include <hal/debug.h>

#define LPCFlashAddress 0xFF000000u

void LPCmemoryRead(unsigned char* rxbuff, unsigned long address, unsigned long len);
void LPCmemoryWrite(unsigned long address, unsigned char data);
void flashSectorErase(unsigned long sector_address);
void flashFullErase(void);
void flashProgramByte(unsigned long address, unsigned char data);
unsigned char getManufID(void);
unsigned char getDevID(void);