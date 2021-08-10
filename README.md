# VINDRIKTNING-esp8266-MQTT-Domoticz

Based on the idea from Soren Beye (https://github.com/Hypfer/esp8266-vindriktning-particle-sensor), to add an esp8266 e.g. Wemos D1 mini pro, to the IKEA VINDRIKTNING PM2.5 air quality sensor, this code will publish the calculated average via MQTT to Domoticz. 

The first step is to create a virtual sensor in Domoticz to obtain a unique identifier (idx). Give the virtual sensor a name and use ug/m3 as the axis label
