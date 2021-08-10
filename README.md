# VINDRIKTNING-esp8266-MQTT-Domoticz
#
# Based on the idea from Soren Beye https://github.com/Hypfer/esp8266-vindriktning-particle-sensor, to add an esp8266 e.g. Wemos D1 mini pro, to the IKEA VINDRIKTNING PM2.5 air quality sensor, this code will publish the calculated average via MQTT to Domoticz. First we need to create a virtual sensor in Domoticz to obtain an unique identifier (idx). Give the virtual sensor a name and provide and axis label e.g. ug/m3.
