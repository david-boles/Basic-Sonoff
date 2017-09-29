#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <NTPClient.h>

const char* ssid = "WIFI_SSID"
const char* password = "WIFI_PASSWORD";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void setup() {
  setupSonoff();
  setLED(HIGH);
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    setLED(LOW);
    delay(5000);
    setLED(HIGH);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  timeClient.begin();
  
  setLED(LOW);
  setRelay(HIGH);
}

bool last = getButton();
int state = 0;

void loop() {
  ArduinoOTA.handle();
  timeClient.update();
  switch(state) {
    case 0:
      if(timeClient.getHours() > 4 && timeClient.getHours() < 15) {
        setRelay(false);
      }else {
        setRelay(true);
      }
      break;
    case 1:
      setRelay(true);
      break;
    case 2:
      setRelay(false);
      break;
  }

  if(getButton() && !last) {
    state = (state + 1)%3;
    flash(state + 1);
  }

  last = getButton();
  delay(100);
}

void flash(int num) {
  for(int i = 0; i < num; i++) {
    setLED(HIGH);
    delay(200);
    setLED(LOW);
    delay(200);
  }
}
