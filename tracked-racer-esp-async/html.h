#pragma once

#include <Arduino.h>

static const char WS_TEST_HTML[] PROGMEM = R"KEY(
<html>
<head>
    <script>
        var connection = new WebSocket('ws://'+location.hostname+'/ws');
        connection.onopen = function() {
            console.log('WebSocket connection established.', connection);
        };
        connection.onclose = function() {
            console.log('WebSocket connection closed.', connection);
        };
        connection.onerror = function(error) {
            console.log('WebSocket error ', error);
        };
        connection.onmessage = function(e) {
            console.log('Server: ', e.data);
        };
        function sendValue() {  
            var valueA = parseInt(document.getElementById('inputA').value, 10);
            var valueB = parseInt(document.getElementById('inputB').value, 10);

            console.log(valueA, valueB);

            connection.send('{\"motorA\":' + valueA + ',\"motorB\":' + valueB + ',\"mode\":\"absolute\"}');
        }
    </script>
</head>
<body>
    <div>
        Motor A:<input id="inputA" type="range" min="-25" max="25" step="5" oninput="sendValue();"/>
    </div>
    <div>
        Motor B:<input id="inputB" type="range" min="-25" max="25" step="5" oninput="sendValue();"/>
    </div>
</body>
</html>
)KEY";