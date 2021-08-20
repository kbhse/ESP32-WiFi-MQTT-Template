/* config.h */

#ifndef CONFIG_H   /* include guards */
#define CONFIG_H

    #include <Arduino.h>

    #define PROGNAM "ESP32_WiFi_MQTT_Template"                                                          // program name
    #define VERSION "v01.04"                                                                            // program version (nb lowercase 'version' is keyword)
    #define PGMFUNCT ""                                                                                 // what the program does
    #define AUTHOR "J Manson"                                                                           // created by
    #define CREATION_DATE "17 August 2021"                                                              // date
    #define MODIFIED_DATE "20 August 2021"
    #define DEBUG_OUT



    #define ADC_INPUT 34                            // the pin where the CT sensor is connected
    #define HOME_VOLTAGE 244                        // the house voltage

    const int lamp = LED_BUILTIN;
    //long MQTTupdateFreq = 10000;                    // 1 minute (the update frequency to publish data to MQTT (milliseconds))
    //#define UPDATE_FREQ 60000L                    // 60 seconds

    // ------------------------------------------------------------------
    // MCU Hardware

    //#define MCU "ESP8266 D1 Mini"
    //#define MCU "ESP8266 D1 Mini Pro V2"
    #define MCU "ESP32 Dev board"
    //#define MCU "ESP32 D1 Mini"

    // ------------------------------------------------------------------
    // Configure MQTT
    // (unique number for each ESP8266 device and edit the next 2 #defines accordingly)

        #define MQTT_DEVICE "esp031"                                                                       // MQTT requires unique device ID (see reconnect() function)
        #define PUB_SUB_CLIENT esp031client                                                                // and unique client ?
        #define MQTT_LOCATION "roaming31"                                                                    // location for MQTT topic

    // ------------------------------------------------------------------
    // specify which MQTT Server to connect to

        const char* mqtt_server = "192.168.1.100";              // raspi03-01
        const char* mqttUser = "mqttUser2";
        const char* mqttPassword = "jR5b73Wklx";

        //const char* mqtt_server = "192.168.1.138";                // raspi4-03
        //const char* mqttUser = "mqttUser";
        //const char* mqttPassword = "kP7fWB5gVA";

        const int mqttPort = 1883;

    // ------------------------------------------------------------------
    // specify which WiFi network to connect to (only 1 should be active)

        #define BTHUB6                      // BTHub in Weights Room
        //#define EXT_BTHUB6                // WiFi Extender in Gym
        //#define BTHUBKBHSE                // NB not useable with this code because no MQTT Server on KBHSE Network
        //#define LINKSYS                   // Linksys Router in Garage

        #ifdef BTHUB6                                                                                       // BTHub in Weights Room
            const char* ssid = "BTHub6-7N5K";
            const char* password = "QeC3RCJGeUvx";
        #endif

        #ifdef EXT_BTHUB6                                                                                   // WiFi Extender in Gym
            const char* ssid = "EXT-BTHub6-7N5K";          
            const char* password = "QeC3RCJGeUvx";
        #endif

        #ifdef LINKSYS                                                                                      // Linksys Router in Garage
            const char* ssid = "linksys";                                                                   // BTHub WiFi credentials
            const char* password = "rhenigidale";
        #endif

        #ifdef BTHUBKBHSE
            const char* ssid = "BT-HZA8HJ";                                                                 // KBHSE BTHub WiFi credentials
            const char* password = "fyurGVup6qru3H";
        #endif

    // ------------------------------------------------------------------
    // Comment out sensors not in use

        //#define WEMOS_SHT30 "SHT30 "                                                                              // Wemos Temperature and Humidity shield
        //#define WEMOS_HP303 "HP303 "                                                                              // Wemos Barometric Pressure Shield
        //#define WEMOS_BH1750 "BH1750 "                                                                            // Wemos Ambient Light Shield
        //#define WEMOS_BATTERY "BATTERY "                                                                          // Wemos Battery Shield
        //#define WIFI_RSSI "RSSI "                                                                                 // ESP8266 WiFi RSSI
        //#define RS485_WIND "WIND "                                                                                // Anemometer and Wind Direction
        //#define CO2 "CO2 "                                                                                        // sandbox electronics CO2 sensor - NB recalibrate in fresh air regularly
        //#define ROBOTDYN_ACDIMMER "ACDIMMER "                                                                     // Robotdyn zero-crossing detector and AC dimmer module
        //#define MLX90614_IR_SENSOR "IR "                                                                          // IR temp sensor
        //#define DS18B20 "DS18B20"                                                                                 // 1-wire temperature sensor
        #define CURRENT_SENSOR                                                                                      // YHDC SCT-013 CT Sensor

/*
    // ------------------------------------------------------------------------------------------------
    // TFT pins - see README.md

        #define TFT_CS D0  //for D1 mini or TFT I2C Connector Shield (V1.1.0 or later)
        #define TFT_DC D8  //for D1 mini or TFT I2C Connector Shield (V1.1.0 or later)
        #define TFT_RST -1 //for D1 mini or TFT I2C Connector Shield (V1.1.0 or later)
        #define TS_CS D3   //for D1 mini or TFT I2C Connector Shield (V1.1.0 or later)

*/
   
#endif // CONFIG_H