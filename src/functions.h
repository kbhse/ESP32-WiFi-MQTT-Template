/* functions.h */

#ifndef FUNCTIONS_H   /* include guards */
#define FUNCTIONS_H

    #include <Arduino.h>
    #include "WEMOS_SHT3X.h"                                                                            // Wemos Temperature and Humidity shield library
    #include "config.h"

    extern long updateFreq;
    extern PubSubClient client;
    extern EnergyMonitor emon1;

    // ------------------------------------------------------------------
    // functions:
    // float calcVPD()
    // void id()
    // void readSensors()
    // ------------------------------------------------------------------

                                                                                                        // https://stackoverflow.com/questions/47346133/how-to-use-a-define-inside-a-format-string
    #define _STRINGIFY(x) #x                                                                            // this converts to string
    #define STRINGIFY(x) _STRINGIFY(x)                                                                  // this makes sure the argument is expanded before converting to string
                                                                                                        // used to convert #define PUB_SUB_CLIENT esp30client to a string in id() function

    // ------------------------------------------------------------------
    // Create Instances
    
    #ifdef WEMOS_SHT30
        SHT3X sht30(0x45);                                                                              // SHT30 shield has two user selectable I2C addresses
    #endif

    // ------------------------------------------------------------------------------------------------
    float calcVPD(float T, float RH)
        {
        //float RH = sht30.humidity;
        //float T = sht30.cTemp;
        float VPsat = ((610.7 * pow(10, ((7.5*T)/(237.5+T))))/1000);
        float VPair = VPsat*(RH/100);
        float VPD = VPsat-VPair;
        return VPD;
        }

    // ------------------------------------------------------------------
    void id()                                                                                       
    // create Json object containing ID data and publish to mqtt broker
    // https://techtutorialsx.com/2017/04/29/esp32-sending-json-messages-over-mqtt/
    // https://github.com/bblanchon/ArduinoJson/blob/6.x/examples/JsonGeneratorExample/JsonGeneratorExample.ino
    // https://arduinojson.org/v6/doc/serialization/
    // PubSubClient: As part of minimising its footprint, it limits the size of any MQTT packet it can send or receive to 128 bytes.
    // If you want to send or receive messages larger than this, you must change the value of MQTT_MAX_PACKET_SIZE in PubSubClient.h
    // The library allocates this much memory in its internal buffer, which reduces the memory available to the sketch itself.
    // https://www.hivemq.com/blog/mqtt-client-library-encyclopedia-arduino-pubsubclient/

        {

        //StaticJsonBuffer<300> JSONbuffer;
        //JsonObject& JSONencoder = JSONbuffer.createObject();

        StaticJsonDocument<400> doc;

        doc["MQTT_DEVICE"] = MQTT_DEVICE;
        doc["MCU"] = MCU;
        doc["PROGNAM"] = PROGNAM;
        doc["VERSION"] = VERSION;
        Serial.println(WiFi.localIP());
        doc["IP"] = WiFi.localIP().toString();
        doc["ROUTER"] = ssid;
        doc["MQTT_LOCATION"] = MQTT_LOCATION;
        String pubSubClient = STRINGIFY(PUB_SUB_CLIENT);                                                        // used to convert PUB_SUB_CLIENT #define to a string  https://stackoverflow.com/questions/47346133/how-to-use-a-define-inside-a-format-string
        Serial.println(pubSubClient);
        doc["PUB_SUB_CLIENT"] = pubSubClient;
        doc["mqtt_server"] = mqtt_server;
        doc["UPDATE_FREQ"] = updateFreq;
        #ifdef LED_STARTS_OFF
            doc["LED_STARTS"] = "OFF";
        #endif
        #ifndef LED_STARTS_OFF
            doc["LED_STARTS"] = "ON";
        #endif

        String sensors;
        #ifdef WEMOS_SHT30
            sensors += WEMOS_SHT30;
        #endif
        #ifdef WEMOS_HP303
            //sensors += ", ";
            sensors += WEMOS_HP303;
        #endif
        #ifdef WEMOS_BH1750
            sensors += WEMOS_BH1750;
        #endif
        #ifdef WEMOS_BATTERY
            sensors += WEMOS_BATTERY;
        #endif
        #ifdef WIFI_RSSI
            sensors += WIFI_RSSI;
        #endif
        #ifdef RS485_WIND
            sensors += RS485_WIND;
        #endif
        #ifdef CO2
            sensors += CO2;
        #endif
        #ifdef ROBOTDYN_ACDIMMER
            sensors += ROBOTDYN_ACDIMMER;
        #endif
        #ifdef MLX90614_IR_SENSOR
            sensors += MLX90614_IR_SENSOR;
        #endif
        #ifdef DS18B20
            sensors += DS18B20;
        #endif
        doc["SENSORS"] = sensors;

        // Generate the minified JSON and send it to the Serial port
        serializeJson(doc, Serial);
        Serial.println();
        // Generate the prettified JSON and send it to the Serial port
        serializeJsonPretty(doc, Serial);
        Serial.println();

        // Declare a buffer to hold the result
        char jsonBuff[384];
        // Produce a minified JSON document
        serializeJson(doc, jsonBuff);
        Serial.println(jsonBuff);
        //publish to MQTT
        client.publish("ID", jsonBuff);

        }
        // end of id()

    // ------------------------------------------------------------------
    void readSensors()
    // read sensors and send data to MQTT

        {
        #ifdef WEMOS_SHT30
            #ifdef DEBUG_OUT
                Serial.println(F("Reading the SHT30 for MQTT: "));
            #endif
            static char temperatureTemp[7];         // client.publish() expects char array
            static char humidityTemp[7];
            static char vpdTemp[7];
            float temperature = 0;
            float humidity = 0;
            float VPD = 0;
            sht30.get();
            temperature = sht30.cTemp;              // float temperature = sht30.cTemp   !!!

            if (temperature > 100)                  // restart ESP8266 ON INSANE METRICS
                {
                #ifdef DEBUG_OUT
                    Serial.println("Insane metrics, Restarting ESP8266..");
                #endif
                static char restartMessage[40] = "Insane metrics, Restarting ESP8266..";
                client.publish(MQTT_LOCATION "/sysMessage", restartMessage);
                delay(3000);
                ESP.restart();
                }

            humidity = sht30.humidity;
            dtostrf(temperature, 6, 2, temperatureTemp);                            // convert float to char array
            dtostrf(humidity, 6, 2, humidityTemp);
            client.publish(MQTT_LOCATION "/temperature", temperatureTemp);
            client.publish(MQTT_LOCATION "/humidity", humidityTemp);
            VPD = calcVPD(temperature, humidity);
            dtostrf(VPD, 6, 2, vpdTemp);                                            // convert float to char array
            client.publish(MQTT_LOCATION "/mcuVPD", vpdTemp);
            #ifdef DEBUG_OUT
                Serial.print("Temperature: ");
                Serial.println(temperatureTemp);
                Serial.print("Humidity: ");
                Serial.println(humidityTemp);
                Serial.print("VPD: ");
                Serial.println(vpdTemp);
            #endif
        #endif

        #ifdef WEMOS_BATTERY
            static char voltageTemp[7];                                                                // client.publish() expects char array
            int adc = analogRead(0);                                                                   // read the ESP ADC (connected to battery)
            float voltage = (adc * 4.335) / 1024;                                                      // calculate the voltage (fsd 4.335v)
            dtostrf(voltage, 6, 2, voltageTemp);                                                       // convert float to char array
            client.publish(MQTT_LOCATION "/voltage", voltageTemp);                                     // publish to MQTT
        #endif

        #ifdef WIFI_RSSI            
            int rssi = WiFi.RSSI();                                                                    // get RSSI (received signal strength indicator) of WiFi connection
            static char rssiStr[5];
            itoa(rssi, rssiStr, 10);
            client.publish(MQTT_LOCATION "/rssi", rssiStr);
        #endif

        #ifdef CURRENT_SENSOR
            static char ampsTemp[7];                                                                // client.publish() expects char array
            static char wattsTemp[7];
            double amps = emon1.calcIrms(1480); // Calculate Irms only
            double watts = amps * HOME_VOLTAGE;
            itoa(amps, ampsTemp, 10);                                                               // convert integer to string (base 10)
            itoa(watts, wattsTemp, 10);
            client.publish(MQTT_LOCATION "/amps", ampsTemp);                                        // publish to MQTT
            client.publish(MQTT_LOCATION "/watts", wattsTemp);
            #ifdef DEBUG_OUT
                Serial.print("Amps: ");
                Serial.println(amps);
                Serial.print("Watts: ");
                Serial.println(watts);
            #endif
        #endif

        #ifdef ADC_TEST
            // ESP32 ADC test
            static char adcTemp[7];                                                                     // client.publish() expects char array
            int adc = analogRead(34);                                                                   // read the ESP ADC (connected to battery)
            itoa(adc, adcTemp, 10);
            client.publish(MQTT_LOCATION "/adc", adcTemp);
            Serial.print("ADC: ");
            Serial.println(adc);
        #endif

        }   // end of readSensors()

    // ------------------------------------------------------------------------------------------------

#endif // FUNCTIONS_H