# Smart LEGO Tracked Racer

Control your LEGO Technic 42065 RC Tracked Racer via web application instead of using included RC.

## Open issues

* Enable TLS for ESP8266 to provide https
* Enable Service Worker to Polymer app (depends on https)
* Build an [Apache Cordova](https://cordova.apache.org/) app from WebApp

## Hardware listing

* [42065 RC Tracked Racer](https://www.lego.com/en-us/technic/products/rc-tracked-racer-42065)
* Amica NodeMCU ESP8266 - *Important note: Not all ESP8266 development boards are compatible with the offered motor shields. Especially the sizing of the LoLin NodeMCU V3 doesn't match.*
* ESP8266 NodeMcu V2 Motor Shield Board
* a bunch of wires

## Pictures of hardware assembling

_TODO_

## Controller in Html5 & Css3

Various controller versions with the idea of using motion control of my smartphone to control the vehicle:

[JSFiddle - First version inspired by Android Remote Controller](https://jsfiddle.net/hunsalz/eg8L16uk/)

[JSFiddle - Adapted second version](https://jsfiddle.net/hunsalz/xh6ny11p/)

Technically motion control works perfectly. - But unfortunately it was very unhandy for controlling the vehicle. 

Finally the least exciting design became the winner:

![Controller](https://user-images.githubusercontent.com/16960855/30973828-5d66fd42-a46e-11e7-98a2-cb32a9309094.png)

[JSFiddle - Latest JSFiddle Version](https://jsfiddle.net/hunsalz/1tgfpvgL/)

## Build and run

### Build Web-App with [Polymer 2.0](https://www.polymer-project.org/2.0/)

1. Install all dependencies via [Bower](https://bower.io/) in the __controller-app__ directory:

```
    bower install
```

2. Test app locally in your browser via the Polymer development server part of [Polymer CLI](https://www.npmjs.com/package/polymer-cli). Start typing from the __controller-app__ directory:

```
    polymer serve --open
```

_Note: By design this app runs locally on ESP8266. That's why the WebSocket connection url listens on host=**window.location.hostname** by default. For testing purposes simply change the url to any other WebSocket server location._

3. Minify & bundle app

[polymer-bundler](https://github.com/Polymer/polymer-bundler) does this job well:

```
  polymer-bundler index.html --inline-scripts --inline-css --strip-comments > ../Esp8266/data/www/index.build.html
```

4. Compile & upload C++ code to ESP8266

Load sketch __ESP8266.ino__ from __ESP8266__ folder in [Arduino IDE](https://www.arduino.cc/en/main/software).

Verify dependencies:

* Update __Additional boards Manager URLs__ in your Arduino IDE preferences page with [Arduino core for ESP8266 WiFi chip](https://github.com/esp8266/Arduino). Current version depends on: https://github.com/esp8266/Arduino/releases/download/2.4.0-rc1/package_esp8266com_index.json

Additional includes:

* [#include <ArduinoLog.h>](https://github.com/thijse/Arduino-Log)
* [#include <ArduinoJson.h>](https://github.com/bblanchon/ArduinoJson)
* [#include <ArduinoJson.h>](https://github.com/bblanchon/ArduinoJson)
* [#include <ESPAsyncTCP.h>](https://github.com/me-no-dev/ESPAsyncTCP/blob/master/src/ESPAsyncTCP.h)
* [#include <ESPAsyncWebServer.h>](https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/ESPAsyncWebServer.h)

You can manage your additional includes by Arduino IDE __Sketch > Include Library > Manage Libraries...__ or better by __git clone__ in your __library__ folder of your Arduino sketchbook location.

Using git allows you to easily update all libraries at once:

```
ls | xargs -I{} git -C {} pull
```

With all dependencies provided the code should compile and is ready for upload.

5. Upload app to ESP8266

Finally upload app from __ESP8266/data/www__ folder to your ESP8266. Go to Arduino IDE __Tools > ESP8266 Sketch Data Upload__

*Note*: Close _Serial Monitor_ of Arduino IDE before uploading data. Otherwise upload will interrupt.

6. Testing

_TODO_
