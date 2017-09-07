#include "WebService.h"

WebService::WebService() :
  webServer(80),
  webSocket("/racer"),
  racerHandler(true) {}

WebService::~WebService() {
  stop();
}

bool WebService::isRunning() {
  return _running;
}

bool WebService::start() {

  if (!isRunning()) {
    // handle web sockets
    webSocket.onEvent(std::bind(&TrackedRacerHandler::onEvent, racerHandler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
    webServer.addHandler(&webSocket);

    // rewrite root to default index file
    webServer.rewrite("/", DEFAULT_INDEX);
    // handle static web resources
    webServer.serveStatic("/", SPIFFS, "/www/", "max-age:600"); // cache-control 600 seconds
    // handle 404
    webServer.onNotFound(notFoundFunction());
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

AsyncCallbackWebHandler& WebService::on(const char* uri, ArRequestHandlerFunction onRequest) {
  return webServer.on(uri, onRequest);
}

AsyncCallbackWebHandler& WebService::on(const char* uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest) {
  return webServer.on(uri, method, onRequest);
}

AsyncCallbackWebHandler& WebService::on(const char* uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest, ArUploadHandlerFunction onUpload) {
  return webServer.on(uri, method, onRequest, onUpload);
}

AsyncCallbackWebHandler& WebService::on(const char* uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest, ArUploadHandlerFunction onUpload, ArBodyHandlerFunction onBody) {
  return webServer.on(uri, method, onRequest, onUpload, onBody);
}

bool WebService::remove(AsyncWebHandler* handler) {
  return webServer.removeHandler(handler);
}

ArRequestHandlerFunction WebService::notFoundFunction() {

  return [](AsyncWebServerRequest *request) {
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
  };
}
