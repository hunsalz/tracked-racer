#include "WSHandler.h"

void WSHandler::onEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {

  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  switch (type) {
    case WS_EVT_CONNECT:
      Log.verbose(F("ws[%s][%u] connected" CR), ws->url(), client->id());
      notify(ws, client, "connect", false);
      break;
    case WS_EVT_DISCONNECT:
      Log.verbose(F("ws[%s][%u] disconnected: %u" CR), ws->url(), client->id());
      break;
    case WS_EVT_ERROR:
      Log.error(F("ws[%s][%u] error(%u): %s" CR), ws->url(), client->id(), *((uint16_t*)arg), (char*)data);
      break;
    case WS_EVT_PONG:
      Log.verbose(F("ws[%s][%u] pong[%u]: %s" CR), ws->url(), client->id(), len, (len)?(char*)data:"");
      break;
    case WS_EVT_DATA:
      // process data if it's a single frame and all payload is available and data contains only text
      if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        // convert data into char array
        char message[info->len + 1];
        strncpy(message, (char*)data, info->len);
        // make certain that the message ends with a \0 terminator
        message[info->len] = '\0';

        // TODO: workaround - replace by Log.verbose(...)
        Serial.printf("V: ws[%s][%u] received : %s\n", ws->url(), client->id(), String(message).c_str());

        // try to interpret message as JSON
        DynamicJsonBuffer jsonBuffer;
        JsonObject &json = jsonBuffer.parseObject(message);
        // interrupt if message contains no valid JSON
        if (json.success()) {
          processMessage(ws, client, json);
        } else {
          Log.error(F("Parsing message into JSON failed." CR));
        }
      } else {
        // TODO send a response to the client
      }
      break;
    default:
      Log.verbose(F("ws[%s][%u] frame[%u] %s[%llu - %llu]" CR), ws->url(), client->id(), info->num, (info->message_opcode == WS_TEXT)?"text":"binary", info->index, info->index + len);
      // TODO send a response to the client
      break;
    }
}

void WSHandler::send(AsyncWebSocket *ws, AsyncWebSocketClient *client, JsonObject *json, bool broadcast = false) {

  int length = json->measureLength() + 1;
  char payload[length];
  json->printTo(payload, length);
  if (broadcast) {
    ws->textAll(payload);
  } else {
    client->text(payload);
  }
}
