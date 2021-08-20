/* wifiMQTT.h */

#ifndef WIFI_MQTT_H   /* include guards */
#define WIFI_MQTT_H

    #include <Arduino.h>
    //#include <ESP8266WiFi.h>
    #include "WiFi.h"
    #include "PubSubClient.h"                   // https://github.com/knolleary/pubsubclient   NB modified MQTT_MAX_PACKET_SIZE in PubSubClient.h from 128 to 384
    #include "ArduinoJson.h"                    // https://github.com/bblanchon/ArduinoJson
    #include "SimpleTimer.h"                    // https://playground.arduino.cc/Code/SimpleTimer/ https://github.com/marcelloromani/arduino/tree/master/SimpleTimer

    #include "config.h"
    #include "functions.h"

    extern PubSubClient client;
    extern bool restartFlg;
    extern long updateFreq;
    extern SimpleTimer timer;
    extern int timerID;

    // ------------------------------------------------------------------
    // functions:
    // boolean reconnect()
    // void connect_wifi()
    // void callback()
    // void readSensors()
    // ------------------------------------------------------------------


    // ------------------------------------------------------------------
    boolean reconnect()
    // (re)connect to the MQTT server
    // use this version with non-blocking code in loop()
        {
        if (client.connect(MQTT_DEVICE, mqttUser, mqttPassword))
            {
            Serial.print("Connected to MQTT Server: ");
            Serial.println(mqtt_server);
            
            // Subscribe to MQTT Topics
            client.subscribe("update/Freq/" MQTT_LOCATION);
            //client.subscribe(MQTT_LOCATION "/changeLowSetPoint");
            //client.subscribe(MQTT_LOCATION "/changeHighSetPoint");
            client.subscribe(MQTT_LOCATION "/restart");

            if (restartFlg)
                {
                restartFlg = false;
                static char sysMessageStr1[22] = "ESP8266 (re)started";                 // to track restarts
                client.publish(MQTT_LOCATION "/sysMessage", sysMessageStr1);
                static char sysMessageStr2[10] = VERSION;
                client.publish(MQTT_LOCATION "/sysMessage", sysMessageStr2);
                // request the update frequency (from Node RED) to override the default updateFreq
                // but don't wait for it (non-blocking). it will update updateFreq in the callback if/when message arrives
                //client.publish( "update/getFreq", MQTT_LOCATION);
                client.publish( MQTT_LOCATION "/getFreq", "");
                // request the current setPoints but don't wait for them (non-blocking).
                // they will overwrite the defaults in the callback if/when messages arrive
                //client.publish( MQTT_LOCATION "/getCurrentSetPoints", "");
                }
            static char sysMessageStr3[23] = "(re)connected to MQTT";                   // to track MQTT connections
            client.publish(MQTT_LOCATION "/sysMessage", sysMessageStr3);
            }
        return client.connected();

        }   // end of reconnect()

    // ------------------------------------------------------------------
    void connect_wifi()
    // this code is BLOCKING if no WiFi !!!
        {
        delay(10);
        // connect to a WiFi network
        Serial.println();
        Serial.print("Connecting to ");
        Serial.println(ssid);

        WiFi.begin(ssid, password);
        // the next line to fix esp8266 broadcasting unwanted open wifi network
        // it is placed AFTER WiFi.begin()
        // see: https://forum.arduino.cc/index.php?topic=501263.0 and https://github.com/esp8266/Arduino/issues/549
        WiFi.mode(WIFI_STA);
        while (WiFi.status() != WL_CONNECTED)
            {
            delay(500);
            Serial.print(".");
            }
        Serial.println("");
        Serial.print("WiFi connected - ESP IP address: ");
        Serial.println(WiFi.localIP());

        }   // end of connect_wifi

    // ------------------------------------------------------------------
    void callback(String topic, byte* message, unsigned int length)
    // This functions is executed when a device publishes a message to a topic that the ESP8266 is subscribed to
    // (subscriptions are specified in reconnect() function)
    // Change the function below to add logic to the program, so when a device publishes a message to a topic that 
    // the ESP8266 is subscribed to it can do something

        // handle message arrived
        {
        String messageTemp;
        for (unsigned int i = 0; i < length; i++)
            {
            Serial.print((char)message[i]);
            messageTemp += (char)message[i];
            }

        #ifdef DEBUG_OUT
            Serial.print("Message arrived on topic: ");
            Serial.print(topic);
            Serial.print(". Message: ");
            Serial.println();
        #endif

        // if a message is received on a topic this device is subscribed to, take appropriate action
        // (subscriptions are specified in the reconnect() function)

        // update frequency
        if(topic == "update/Freq/" MQTT_LOCATION)
            {
            updateFreq = messageTemp.toInt();
            #ifdef DEBUG_OUT
                Serial.print("freq: ");
                Serial.println(updateFreq);
            #endif
            if(timer.isEnabled(timerID))                        // if timer has already been started (in setup())
                {
                timer.deleteTimer(timerID);
                timerID = timer.setInterval(updateFreq, readSensors);       // start timer with new updateFreq
                }
            }
        // onboard LED
        if(topic == MQTT_LOCATION "/lamp")
            {
            Serial.print("Changing Room lamp to ");
            if(messageTemp == "on")
                {
                digitalWrite(lamp, LOW);                       // active LOW
                Serial.print("On");
                }
            else if(messageTemp == "off")
                {
                digitalWrite(lamp, HIGH);
                Serial.print("Off");
                }
            }

        // restart
        if(topic == MQTT_LOCATION "/restart")               // manual restart from NodeRED
            {
            static char sysMessageStr[40] = "manual restart from NodeRED";
            client.publish(MQTT_LOCATION "/sysMessage", sysMessageStr);
            delay(3000);
            ESP.restart();
            }

        // respond with ID data
        if(topic == "IDRequest")
            {
            id();
            }
        
        //Serial.println();

        }   // end of callback()

    // ------------------------------------------------------------------

#endif // WIFI_MQTT_H
