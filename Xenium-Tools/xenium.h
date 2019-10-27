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
 
#define XENIUM_REGISTER_BANKING 0xEF
#define XENIUM_REGISTER_LED 0xEE
#define XENIUM_LED_R (1<<0)
#define XENIUM_LED_G (1<<1)
#define XENIUM_LED_B (1<<2)


#define XENIUM_BANK_TSOP        0
#define XENIUM_BANK_CROMWELL    1
#define XENIUM_BANK_XENIUMOS    2 //This is XeniumOS data only. To boot into XeniumOS, choose the cromwell bank.
#define XENIUM_BANK_1           3
#define XENIUM_BANK_2           4
#define XENIUM_BANK_3           5
#define XENIUM_BANK_4           6
#define XENIUM_BANK_1_512       7
#define XENIUM_BANK_2_512       8
#define XENIUM_BANK_1_1024      9
#define XENIUM_BANK_RECOVERY    10

#define XENIUM_FLASH_SIZE      2097152
#define XENIUM_SIZEOF_CROMWELL 262144
#define XENIUM_SIZEOF_XOS1     524288
#define XENIUM_SIZEOF_XOS2     131072
#define XENIUM_SIZEOF_UPDATE   1048576

#define XENIUM_MANUF_ID  0x01
#define XENIUM_DEVICE_ID 0xC4
#define XENIUM_LASTBLAST_MANUF_ID 0xBF
#define XENIUM_LASTBLAST_ID 0xC9