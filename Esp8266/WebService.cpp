#include "WebService.h"

WebService::WebService() :
  webServer(HTTP_PORT) {}

WebService::~WebService() {
  stop();
}

bool WebService::isRunning() {
  return _running;
}

bool WebService::start() {

  if (!isRunning()) {
    // rewrite root context
    webServer.rewrite("/", ROOT_CONTEXT);
    // handle static web resources
    webServer.serveStatic("/", SPIFFS, "/www/", "max-age:600"); // cache-control 600 seconds
    // handle 404
    webServer.onNotFound([](AsyncWebServerRequest *request) {
      String method = F("UNKNOWN");
      if (request->method() == HTTP_GET)
        method = F("GET");
      else if (request->method() == HTTP_POST)
        method = F("POST");
      else if (request->method() == HTTP_DELETE)
        method = F("DELETE");
      else if (request->method() == HTTP_PUT)
        method = F("PUT");
      else if (request->method() == HTTP_PATCH)
        method = F("PATCH");
      else if (request->method() == HTTP_HEAD)
        method = F("HEAD");
      else if (request->method() == HTTP_OPTIONS)
        method = F("OPTIONS");
  
      Log.verbose(F("HTTP 404 : http://%s%s" CR), request->host().c_str(), request->url().c_str());
  
      request->send(404, F("Page not found."));
    });
    // start web server
    webServer.begin();
    _running = true;
  }
  Log.verbose("WebServer started.\n");

  return true;
}

bool WebService::stop() {

  if (isRunning()) {
    webServer.reset();
    _running = false;
  }

  return isRunning();
}

AsyncWebServer* WebService::getWebServer() {
  return &webServer;
}
