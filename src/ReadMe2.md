Notes:
1. use a different library for SHT30 (with crc error checking)
2. make wifi and mqtt non-blocking
    WiFi:
        https://www.esp8266.com/viewtopic.php?p=71077
        https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/generic-examples.html
    MQTT:
        https://github.com/knolleary/pubsubclient/blob/master/examples/mqtt_reconnect_nonblocking/mqtt_reconnect_nonblocking.ino
3. need remote reboot ESP89266
4. PID control
5. sht30 sample  rate & I2C speed s length https://github.com/espressif/arduino-esp32/issues/1395
6. pubsub qos http://www.steves-internet-guide.com/using-arduino-pubsub-mqtt-client/
7. 06/01/21 19:00 v01.04 running
8. I'm sometimes getting -54 on ssrState! try boolean data type?
9. if it restarts it will go back to default low and high setPoints !!! need NodeRED globals?
10. do I need the wemos relay??
11. add remote restart via subscribe to topic ssrHeater1/restart
12. perhaps it should check for setPoints regularly (if mqtt not reliable and it doesn't update from nodered)

NEED TO PREVENT BLOCKING THE CRITICAL THERMOSTAT FUNCTIONALITY !!

non-blocking WiFi:
if connection is established, and then lost for some reason, ESP will automatically reconnect to the last used access point once it is again back on-line. This will be done automatically by Wi-Fi library, without any user intervention.
so the old code would only be blocking on startup or restart (in setup() call to setup_wifi()) if there is no WiFi because code is waiting to report that the connection has been established.
this code: *while (WiFi.status() != WL_CONNECTED)* could loop forever if no WiFi connection established.
don't need to report connection established so just get rid of that code.
NB test it works if ESP8266 starts up with no WiFi available. does a blink in loop work??
for an event driven solution that reports connection/disconnection without blocking critical functionality
see : https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html#quick-start

non-blocking mqtt  https://github.com/knolleary/pubsubclient/blob/master/examples/mqtt_reconnect_nonblocking/mqtt_reconnect_nonblocking.ino

ChangeLog
01.00 
01.05 change default setPoints
01.06 added publish (re)start message (to track restarts) in setup()
01.07 on (re)startup (in setup()) publish request for the current setPoints but don't wait for them (non-blocking).
      they will overwrite the defaults in callback() if/when messages arrive. take care in NodeRED that the values won't fail the sanity check (in callback()) which would revert to defaults!