# BadAppleESP32
The Bad Apple animation in a Microcontroller with 2Mb (with audio)

To set it up, firstly you will need the following parts requirement
  - A ESP32-WROOM
  - A LCD with a SSD1306 I2C Driver (Commonly just called a SSD1306 LCD)
  - A Common Passive Buzzer, you can get one from a PC Motherboard usually.
  - A USB Cable to Mini-USB, for connecting the ESP32 to your computer.

The hardware setup is as follows...
-Black lines are ground
-Green lines are Signal lines
-Red lines are positive voltage lines (and in the buzzer case, also signal)

![image](https://github.com/katyushapolye/BadAppleESP32/assets/61098580/f54a0f7a-2b4c-4c54-83a9-54c55014b045)


-For software...well, its a bit more complicated. You will have to setup the IDF SDK in your computer
I used a Ubunto laptop, at version 22.04 LTS, using VSCODE, the instalation was pretty straight foward, using the
proper IDF extension from ESPRESSIF. If you run into any troubles in it, I suggest you take a look at these videos/Documents

On how to install the Espressif enviroment in Linux, by their own channel.
https://www.youtube.com/watch?v=Hj3okDU-CIY

The official documentation of the company.
https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html

If you can run any of the examples provided in the IDE, such as the simple "Hello world" and flash your ESP32, you can assume it is working correctly.

Now, onto the important thing Bad Apple.

The code for the ESP32, is provided, obviously, in the ESP32 Folder, there you will find all of the code for it, no need for any alterations, simply copying and pasting should work in a clean project, or even in the simple examples. Be mindful of the CMAKE file and the idf menuconfig, *some chips may require aditional configuration ,such as enabling interrupts,if you run into trouble, you can open a issue request here* and remember to run the
`idf.py clear`
command to clean the build files.

For sake of clarification, the External folder contains external scripts that were run prior, to extract frames from the original video, resize them, compress them using the Huffman compression algorithm, extract the MIDI, the list goes on...
You don't need to run them nor should you put them in your ESP project folder.

