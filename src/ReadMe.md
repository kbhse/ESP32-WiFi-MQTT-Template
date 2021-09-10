            YOU MIGHT NEED TO CHANGE THIS LINE, IF YOU'RE HAVING PROBLEMS WITH MQTT MULTIPLE CONNECTIONS
            To change the ESP device ID, you will have to give a new name to the ESP8266.
            Here's how it looks:
                if (client.connect("ESP8266Client")) {
            You can do it like this:
                if (client.connect("ESP1_Office")) {
            Then, for the other ESP:
                if (client.connect("ESP2_Garage")) {
            That should solve your MQTT multiple connections problem


if it starts without a wifi signal need code in loop() to periodically try to connect, simlar to MQTT server connection

change name from client to MQTTclient

setup in weatherstation

callback in energy monitor

MQTT code from: https://randomnerdtutorials.com/raspberry-pi-publishing-mqtt-messages-to-esp8266/
                https://github.com/knolleary/pubsubclient
https://savjee.be/2019/07/Home-Energy-Monitor-ESP32-CT-Sensor-Emonlib/

grafana mqtt buttons

reduce database entries. 
