#include "FSService.h"

FSService::FSService() {}

FSService::~FSService() {
  stop();
}

bool FSService::isRunning() {
  return _running;
}

bool FSService::start() {

  if (!isRunning()) {
    if (SPIFFS.begin()) {
      Log.verbose(F("File system mounted." CR));
      _running = true;
    } else {
      Log.warning(F("Mounting file system failed." CR));
    }
  }

  return isRunning();
}

bool FSService::stop() {

  if (isRunning()) {
    SPIFFS.end();
    _running = false;
  }

  return isRunning();
}

ArRequestHandlerFunction FSService::getInfoFunction() {

    return [](AsyncWebServerRequest *request) {

    AsyncJsonResponse *response = new AsyncJsonResponse();
    JsonObject& json = response->getRoot();

    FSInfo fs_info;
    SPIFFS.info(fs_info);

    json[F("totalBytes")] = fs_info.totalBytes;
    json[F("usedBytes")] = fs_info.usedBytes;
    json[F("blockSize")] = fs_info.blockSize;
    json[F("pageSize")] = fs_info.pageSize;
    json[F("maxOpenFiles")] = fs_info.maxOpenFiles;
    json[F("maxPathLength")] = fs_info.maxPathLength;

    response->setLength();
    request->send(response);
  };
}

ArRequestHandlerFunction FSService::getListFunction() {
  
  return [](AsyncWebServerRequest *request) {

    AsyncJsonResponse *response = new AsyncJsonResponse();
    JsonObject& json = response->getRoot();
    JsonArray& files = json.createNestedArray("files");
    // enumerate files
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {
      String name = String(dir.fileName());
      String size = FSService::formatBytes(dir.fileSize());
      JsonObject& entry = files.createNestedObject();
      entry[F("name")] = name;
      entry[F("size")] = size;
      Log.verbose(F("File: name=%s, size=%s" CR), name.c_str(), size.c_str());
    }
    response->setLength();
    request->send(response);
  };
}

String FSService::formatBytes(size_t bytes) {

  if (bytes < 1024){
    return String(bytes) + "B";
  } else if (bytes < (1024 * 1024)){
    return String(bytes/1024.0) + "KB";
  } else if (bytes < (1024 * 1024 * 1024)){
    return String(bytes/1024.0/1024.0) + "MB";
  } else {
    return String(bytes/1024.0/1024.0/1024.0) + "GB";
  }
}
