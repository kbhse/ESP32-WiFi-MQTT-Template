
#include <Arduino.h>
#include "SimpleTimer.h"                                                                            // https://playground.arduino.cc/Code/SimpleTimer/ https://github.com/marcelloromani/arduino/tree/master/SimpleTimer
#include "EmonLib.h"                                                                                // https://github.com/openenergymonitor/EmonLib
//#include <driver/adc.h>                                                                           // for ESP32

#include "config.h"
#include "WiFiMQTT.h"

// ------------------------------------------------------------------
// Create Instances
        
WiFiClient PUB_SUB_CLIENT;
PubSubClient client(PUB_SUB_CLIENT);
SimpleTimer timer;
EnergyMonitor emon1;

long updateFreq = 60000;                                                    // DEFAULT update frequency for sensors and publish to MQTT (milliseconds)
int timerID;
bool restartFlg;                                                            // used to flag restarts so reconnect() will publish notification of restart to MQTT topic /sysMessage
long lastMqttReconnectAttempt = 0;

// ------------------------------------------------------------------
void setup()
  {
    
  Serial.begin(9600);
  //#ifdef DEBUG_OUT
      Serial.printf("\n\n%s_%s, %s, %s, %s, %s\n\n", PROGNAM, VERSION, AUTHOR, CREATION_DATE, MQTT_DEVICE, MQTT_LOCATION);
  //#endif
  pinMode(lamp, OUTPUT);                                                       // onboard LED
  #ifdef LED_STARTS_OFF
    digitalWrite(lamp, HIGH);                                                  // turn onboard LED OFF
  #endif

  connect_wifi();                             // NB as non-blocking, do I need a delay here?  test it !!!!!!!!!

  client.setServer(mqtt_server, mqttPort);
  client.setCallback(callback);

  // start the timer
  timerID = timer.setInterval(updateFreq, readSensors);                        // every x milliseconds, send data to MQTT

  id();

  restartFlg = true;

  }

// ------------------------------------------------------------------
void loop()
    {

    // non-blocking MQTT connection/reconnection                                 https://github.com/knolleary/pubsubclient/blob/master/examples/mqtt_reconnect_nonblocking/mqtt_reconnect_nonblocking.ino
    if (!client.connected())
        {
        // MQTT client is not connected
        long now = millis();
        if (now - lastMqttReconnectAttempt > 10000)                                // every 10 seconds
            {
            lastMqttReconnectAttempt = now;
            if (reconnect())                                                       // Attempt to reconnect
                {
                // 
                lastMqttReconnectAttempt = 0;
                }
            }
        }
    else
        {
        // MQTT client is connected
        client.loop();
        }

    timer.run();

    }