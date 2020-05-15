# XeniumOS Skins and LED Patterns
XeniumOS supports custom skins and LED PWM patterns. The PWM test patterns are only persistent within XeniumOS so aren't particularly useful, however are documented here anyway.
 * Skins should be placed in their own subdirectories at `\E\XeniumOS\Skins\`
 * LED scripts should be placed in `\E\XeniumOS\LEDs\`

## Skins
Skins require a 640x480 jpg file named `backdrop.jpg` with an associated `Skin.cfg`.  
`Skin.cfg` configures the colour of the text, and menu objects etc. Each line represends a 6 digit HEX colour value. Lines marked with ? are unknown to me.  
The comments in this example are provided for clarify only and **must** be removed before attempting to load the skin in XeniumOS.<br><br>
An example might look like this:


```
9abcde //Text Box - Text Colour
9abcde //Text Box - Background Colour
9abcde //Text Box - Border Colour 
9abcde //Text Box - Text Selection Highlight Colour
9abcde //Menus - Text Colour
9abcde //Menus - Text Selection Highlight Colour
9abcde //File Explorer - Background Colour
80     //Top Menu - X Position
153    //Top Menu - Y Position
300    //Top Menu - Width
256    //Top Menu - Height
8      //Top Menu - No. Items to Display

```
 See the `releases` section for an example skin I put together:
 ![enter image description here](https://github.com/Ryzee119/OpenXenium/blob/master/Images/skin_sample.jpg?raw=true)  
If you would like me to add your custom skin, please ensure it uses royalty free art work with appropriate credits as required by the author.
## LED PWM Patterns
LED test patterns have the file extension `led`.
A [quote from TeamXodus on the format](https://www.xbox-hq.com/html/article1577.html)

> The format of these files is straightforward; each line in the file represents one set of the leds. The first number is the duration for this set, the second number represents the color to set on the chip LED (0-7), and the third number is a standard hexadecimal representation of the front-panel LED.

An example of a single line might be  
`40 1 0f` where:  
`40 = duration (milliseconds?), 1 = RED, 0x0F = green eject led (0xF0 = red eject led)`

You can find a program called [XeniumOS LED Designer 1.1](https://www.xbox-hq.com/html/downloadview-details-953-XeniumOS_LED_Designer_v1.1.html) online which can assist in creating these patterns.
