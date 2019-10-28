
# OpenXenium CPLD
 This is the heart of the OpenXenium.  
 The main reason you cant just copy a Genine Xenium is because the CPLD is read protected so I reverse engineered the whole things from scratch. 
 This was a significant effort so I ask that you read and respect the [open source license](https://github.com/Ryzee119/OpenXenium/blob/master/Firmware/LICENSE) that this is released under.
 
 * Download and install [ISE Design Suite 14.7](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/design-tools.html).
 * Check this for a Windows 10 fix [https://www.xilinx.com/support/answers/62380.html](https://www.xilinx.com/support/answers/62380.html).
 * And maybe this if you encounter other issues [https://www.xilinx.com/support/answers/68433.html](https://www.xilinx.com/support/answers/68433.html).
 * Open `OpenXenium.xise` with ISE Design Suite then *Process>Implement Top Module* to synthesize the VHDL. 
 * This will generate a JEDEC file for programming. Then go to *Tools>iMPACT...* to program the CPLD using a compatible programming cable. I used a Xilinx Model DLC9LP Programming cable. The JTAG points on the OpenXenium are easily accessible and marked.

This is technically compatible with a Genuine Xenium device, however you should only consider overwriting the CPLD if you are 100% certain you *need* to (i.e. the legacy CPLD is fried and you replace it with a new one). Once the original CPLD is erased it cannot ever be recovered and I'm certain this remake is not 100% identical.
 <br>
By Ryzee119