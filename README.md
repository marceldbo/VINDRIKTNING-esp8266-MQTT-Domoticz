## VINDRIKTNING-esp8266-MQTT-Domoticz

Since the IKEA Vindriktning dust sensor is supported in "Collection E" of the ESPEasy project for a while now, there is no real need for the original project as described below. Please refer to the ESPEasy website https://espeasy.readthedocs.io/en/latest/Plugin/P144.html for more details. 

There is a small difference in the wiring of the REST pin: instead of wiring the REST pin to D2 of the Wemos D1 mini pro, we now wire the REST pin to D9 (GPIO3 - RX). In the ESPEasy configuration, we need to define the controller e.g. Domoticz MQTT, and add the device using serial port: HW Serial0. It is also advisable to disable or change the Serial Port Console Port in the "Advanced" settings of ESPEasy.

Since the unit was open anyway, I also added a BME/BMP280 and connected SDA to D2 (GPIO4 - SDA), SCL to D1 (GPIO5 - SCL), VIN to 3.3V and GND to Ground. Adding the sensor in ESPEasy and creating a Temp + Humidity + Baro sensor in Domoticz is pretty straight forward.

## Original project description

For reference purposes, and for those who prefer to use the firmware method instead of ESPEasy, the following is still valid. It is based on the idea from Sören Beye, see reference below, to add an esp8266 e.g. Wemos D1 mini pro, to the IKEA VINDRIKTNING PM2.5 air quality sensor, this code will publish the calculated average via MQTT to Domoticz.

The first step is to create a virtual sensor in Domoticz to obtain a unique identifier (idx). Give the virtual sensor a name and use ug/m3 as the axis label. Save the device and take note of the idx value, since you will need this in the code later on.

## Hardware

Now it is time to open up the IKEA VINDRIKTNING by unscrewing the four screws in the back of the box. The front comes off and for ease of soldering, I recommend to disconnect the connectors and remove the three small screws. With the circuit board removed, solder wires to GND, 5V and the REST pin. The latter is actually the TX from the onboard microcontroller. Cut the wires to approximately 6cm and solder them to the GND, 5V and D2 respectively of a Wemos D1 Mini. 

![wiring](./img/vr_to_wemos_wiring.jpg)

The image above should give you an idea of the wiring. Alternatively, the round solder pads can also be used.

## Firmware

The firmware can be built and flashed using the Arduino IDE. To do this, you need to add ESP8266 support by [using the Boards Manager](https://github.com/esp8266/Arduino#installing-with-boards-manager). You will also need to install the following libraries using the Library Manager:

* ESP8266WiFi.h
* PubSubClient.h

Download the code from the repository to your system, unzip it and open the .ino file in the Arduino IDE. Once open, follow the comments in the code and enter the following information:

* ssid of your wireless network
* password of your wireless network
* ip address of your MQTT server (there is no need to add the 1883 listener port number as this is handled in the code)
* the unique identifier (idx) in the MQTT client connect section
* the unique identifier (idx) in the MQTT Payload section

After making these changes, do a Verify/Compile to check if everything is OK. If so, connect the Wemos via USB to your system and Upload the code. Open the Serial Monitor from the tools menu of the Arduino IDE. You should see the connectivity status, the data acquisition and every 60 seconds the MQTT Payload message.

Now go to Domoticz and check if the virtual sensor starts to dispay a value in ug/m3. In a healthy environment this should be a value between 2 and 50.

## Background information

The VINDRIKTNING consists of Cubic PM1006-like Sensor + another uC that controls the leds and also outputs the serial TX data from the sensor. With the code we simply listen to the TX output of the uC, take 5 samples and calculate the average of these samples, create a MQTT message and publish the result to Domoticz.

Because it is a generic sensor, the published values are in ug/m3 and not ppm. Ppm is typically used for specific sensors which measure the concentration of one type of gas with a specific weight, hence you can easily calculate the ppm. Converting the ug/m3 to ppm in case of the Ikea type of sensor is most likely going to provide unreliable (read: meaningless) values, as you would need to know the presence and percentage/concentration of each gas in your area and calculate the values accordingly.

## Some ideas

* Dynamically create the payload message using the ArduinoJson library, which is probably a more elegant way of creating the payload message. 
* The Wemos D1 Mini has plenty of pins available to add a BME280/BMP280 sensor board or a DS18B20 temperature sensor. The code can easily be adapted to accomodate these sensors. 
* Another suggestion from the domoticz forum is to use the Alert Sensor Device to visualize the levels in color from grey to red e.g. good, moderate, potentially unhealthy, unhealthy, hazardous etc.

## References and sources

- Thanks to [Sören Beye](https://github.com/Hypfer/esp8266-vindriktning-particle-sensor) for the inspirational work and parts of the code.
