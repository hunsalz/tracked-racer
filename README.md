# Smart LEGO® Tracked Racer

[![Build Status](https://travis-ci.org/hunsalz/tracked-racer.svg?branch=master)](https://travis-ci.org/hunsalz/weather-station)
[![License](https://img.shields.io/badge/license-MIT%20License-blue.svg)](http://doge.mit-license.org)

## Watch [Tracked-Racer-App ↗](https://hunsalz.github.io/tracked-racer/) live.

Control your LEGO® Technic 42065 RC Tracked Racer with your Smartphone instead of using the included RC.

## Features

* Use a [Progressive Web App](https://en.wikipedia.org/wiki/Progressive_web_applications) as controller. Add it without further installation to your homescreen.
* Use Web standard communication with [WebSockets](https://developer.mozilla.org/de/docs/WebSockets)
* Use cheap hardware components

## Hardware listing

* [42065 RC Tracked Racer](https://www.lego.com/en-us/technic/products/rc-tracked-racer-42065)
* A LEGO® Power Functions extension wire [8886]
* Amica NodeMCU ESP8266 - *Important note: Not all ESP8266 development boards are compatible with the offered motor shields. Especially the sizing of the LoLin NodeMCU V3 doesn't fit together.*
* ESP8266 NodeMcu V2 Motor Shield Board
* An elastic band to tie up NodeMCU and MotorShield the easy way with LEGO® bricks
* A bunch of wires

## Pictures of hardware assembling

Plain LEGO® Technic 42065 chassis:

![LEGO® Technic 42065 chassis](https://user-images.githubusercontent.com/16960855/31990202-b3c58120-b974-11e7-9a83-be3f9cbbaa06.jpg)

Assembled ESP8266 + Motorshield + LEGO® parts:

![Assembled ESP8266 + Motorshield + LEGO® motors & battery pack](https://user-images.githubusercontent.com/16960855/31990417-5f07fff4-b975-11e7-85a2-bf42f36d5443.jpg)

Details of ESP8266 + Motorshield + LEGO® parts:

![Details of ESP8266 + Motorshield + LEGO® parts](https://user-images.githubusercontent.com/16960855/31990570-c7881758-b975-11e7-878a-a9024992d66d.jpg)

You find further details of wiring LEGO® Power Functions for example [here](https://scuttlebots.com/2014/03/02/lego-pf-hacking-wiring/).

TODO further details

## Controller in Html5 & Css3

I tried various controller versions with the idea of using motion control abilities of my smartphone to control the vehicle:

   * [JSFiddle - First version inspired by Android Remote Controller](https://jsfiddle.net/hunsalz/eg8L16uk/)

   * [JSFiddle - Adapted second version](https://jsfiddle.net/hunsalz/xh6ny11p/)

Technically motion control works perfectly. - But in practise it was very unhandy for controlling the vehicle.

Finally the winner looks plain simpler:

![controller](https://user-images.githubusercontent.com/16960855/30988069-68acddb8-a499-11e7-84b7-44836a44ae3a.png)

   * [JSFiddle - Latest JSFiddle Version](https://jsfiddle.net/hunsalz/1tgfpvgL/)

## Build and run

### Build Web-App with [Polymer 3.0](https://polymer-library.polymer-project.org/3.0/docs/devguide/feature-overview)

*Prerequisition: [Polymer CLI](https://www.npmjs.com/package/polymer-cli)*

1. Install all dependencies - Go to folder *tracked-racer-app*:

```
$ polymer install
```

2. Test app locally in your browser. 

```
$ polymer serve --open
```

_Note: By design this app runs locally on ESP8266 and depends on a WebSocket served by the WebServer of the ESP. For production adjust the WS endpoint._

3. Prepare for production

```
$ polymer build
```

4. Upload app to ESP8266

Finally upload app from __ESP8266/data/www__ folder to your ESP8266. Go to Arduino IDE __Tools > [ESP8266 Sketch Data Upload](https://github.com/esp8266/arduino-esp8266fs-plugin)__

*Note*: Close _Serial Monitor_ of Arduino IDE before uploading data. Otherwise upload will interrupt.

### *Compile & upload C++ code to ESP8266*

Load sketch __ESP8266.ino__ from __ESP8266__ folder in [Arduino IDE](https://www.arduino.cc/en/main/software).

Verify dependencies:

* [#include <Esp8266Utils.h>](https://github.com/hunsalz/esp8266utils)
* [#include <Log4Esp.h>](https://github.com/hunsalz/log4Esp)

You can manage your additional includes by Arduino IDE __Sketch > Include Library > Manage Libraries__ or you checkout dependencies yourself by __git clone__ in your __library__ folder of your Arduino sketchbook location.

Using git allows you to easily update all libraries at once:

```
ls | xargs -I{} git -C {} pull
```

With all dependencies provided the code should compile and is ready for upload.

5. Browse to http://esp8266.local/ to load the app.