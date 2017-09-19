# Smart LEGO Tracked-Racer

Control your LEGO Technic 42065 RC Tracked Racer via web application instead of using included RC.

## Open issues

* Enable TLS for ESP8266 to provide https
* Enable Service Worker to Polymer app (depends on https)
* Build a [Apache Cordova](https://cordova.apache.org/) app from WebApp

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

Technically motion control works perfect. - But unfortunately it was very unhandy for controlling the vehicle. Finally the least exciting design was chosen as the winner:

[JSFiddle - Latest Version](https://jsfiddle.net/hunsalz/1tgfpvgL/)

## Build and run

### Build Web-App with [Polymer 2.0](https://www.polymer-project.org/2.0/)

1. Install all dependencies via [Bower](https://bower.io/) in the _controller-app_ directory:

```
    bower install
```

2. Test app locally in your browser via the Polymer development server part of [Polymer CLI](https://www.npmjs.com/package/polymer-cli). Start typing from the _controller-app_ directory:

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

Load sketch _ESP8266.ino_ from _ESP8266_ folder in [Arduino IDE](https://www.arduino.cc/en/main/software).

Verify dependencies:

* Update _Additional boards Manager URLs_ in your Arduino IDE preferences page with [Arduino core for ESP8266 WiFi chip](https://github.com/esp8266/Arduino). Current version depends on: https://github.com/esp8266/Arduino/releases/download/2.4.0-rc1/package_esp8266com_index.json

Additional includes:

* [#include <ArduinoLog.h>](https://github.com/thijse/Arduino-Log)
* [#include <ArduinoJson.h>](https://github.com/bblanchon/ArduinoJson)
* [#include <ArduinoJson.h>](https://github.com/bblanchon/ArduinoJson)
* [#include <ESPAsyncTCP.h>](https://github.com/me-no-dev/ESPAsyncTCP/blob/master/src/ESPAsyncTCP.h)
* [#include <ESPAsyncWebServer.h>](https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/ESPAsyncWebServer.h)

You can manage your additional includes by Arduino IDE _Sketch > Include Library > Manage Libraries..._ or better by _git clone_ in your _library_ folder of your Arduino sketchbook location.

Using git allows you to easily update all libraries at once:

```
ls | xargs -I{} git -C {} pull
```

With all dependencies provided the code should compile and should be ready for upload.

5. Upload app to ESP8266

Finally upload app from _ESP8266/data/www_ folder to your ESP8266. Go to Arduino IDE _Tools > ESP8266 Sketch Data Upload_

*Note*: Close _Serial Monitor_ before uploading data. Otherwise upload will interrupt.

6. Testing

_TODO_
