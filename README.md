# ESPLedController
ESP8266 project to control a pair of led strips from the PC through a WebSocket.

## Motivation
So, you probably have already seen some neat gaming setups on the interwebs, decorated with bright and shinny RGB led strips which you can control using some remote to choose the color or make it pulse. That looks fancy allright, but with a bit more of creativity, ingenuity and some simple diy work you can take it to the next level.

This is part of my personal fun project to turn my office into a home made disco. For something like 5 bucks, you can get yourself an ESP8266 chip and a couple of RGB led amplifiers and build a double led strip controller commanded by Wifi. 

With this project you can control the colors and intensity of a couple of led strips from the computer. Aditionally, you can blink the led strips according to music. The ESP hosts a WebSocket server and the PC, using Rainmeter, provides an interface to interact with the controller, sending commands and streaming audio levels through the WebSocket for  music visualization. 

## Features

### General
 - Choose colors
 - Set led brightness
 - Turn leds On/Off

### Color modes
 - Equal - Both strips have the same color
 - Oposite - Strips have oposite colors
 - Unrelated - Strips have unrelated colors
 
### Color transition
 - None - Allways use same color (when applicable)
 - Rand - Transition to random colors (when applicable)

### Light modes
 - Constant - Constant color and intensity
 - Fade - Fade from one color to the next
 - Pulse - Fade brightness from full to none and back
 - MusiVis - Blink leds to music rythm, one strip reacts to bass and the other to treble
 - MusicBeat - Keep brigthness constant but switch color on bass beat
 - Gaming - Dim brightness and blink when reacting to sound (explosions and gun firing), uses reds and yellows as colors
 
Note: The reaction to sound is provided by scripts running in the PC. These can be tweaked without needing to change any of the code in the ESP. Gaming mode was developed for use with BF1.


## Links
 - I used Visual Studio Community with the excelent Visual Micro plugin to develop this project:
https://www.visualmicro.com/
 - Rainmeter Websocket plugin to communicate with the ESP8266: 
https://github.com/ILikon/WebSocketPlugins
 - Rainmeter skin to process audio and provide an interface:
https://github.com/ILikon/AudioReporter
 - The final project which includes all this functionallity and even more:
https://github.com/ILikon/TabletCompanion

Video preview:
 
[![](https://img.youtube.com/vi/je1WfeeDfdk/0.jpg)](https://www.youtube.com/watch?v=je1WfeeDfdk)

### Building the Led controller
Unfortunately I currently have no tutorial on how to build the led controller. Maybe in the future I will be able to dedicate some time to write it. Meanwhile, I leave here the components I used and some tutorials from where I learned to help to get you going.

Components:
 - [D1 mini by WeMos](https://www.aliexpress.com/item/D1-mini-V2-Mini-NodeMcu-4M-bytes-Lua-WIFI-Internet-of-Things-development-board-based-ESP8266/32681374223.html?spm=a2g0s.9042311.0.0.76dc4c4dtn1FUd) - easy to use ESP8266 circuit
 - [DC Power Shield V1.1.0 for WEMOS D1 mini](https://www.aliexpress.com/item/DC-Power-Shield-V1-1-0-for-WEMOS-D1-mini/32837758572.html?spm=a2g0s.9042311.0.0.76dc4c4dtn1FUd) - for easy pluging to power
 - [ProtoBoard Shield for WeMos D1 mini](https://www.aliexpress.com/item/ProtoBoard-Shield-for-WeMos-D1-mini-double-sided-perf-board-Compatible/32823336161.html?spm=a2g0s.9042311.0.0.76dc4c4dtn1FUd) - to allow me to assemble the amplifiers in a small volume
 - [12v led amplifiers](https://www.aliexpress.com/item/Free-shipping-Hot-20pcs-lot-led-amplifier-for-rgb-strip-light-mini-signal-amplifier-12V/729234432.html?spm=a2g0s.9042311.0.0.76dc4c4dtn1FUd) - requires some reverse engeneering but provides a cheap and compact way to drive the led strips
 - In my case I also needed some resistors to interface with the drivers, it will depend on the model

Tutorials:
 - https://www.youtube.com/watch?v=7Dv70ci-MOw
 - https://www.instructables.com/id/Esp8266Websockets-RGB-LED-controller/
 - https://perso.aquilenet.fr/~sven337/english/2015/01/19/Tearing-down-a-RGB-LED-strip-amplifier.html

