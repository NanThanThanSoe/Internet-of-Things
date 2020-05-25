# IOTProject Tasks

IoT project idea: Temperature and humidity data service on ESP8266
Brief description
Record the temperature and humidity measurements using ESP8266 and serve it upon request
by another device.

Tasks to be carried out by the ESP8266
1. Take raw temperature and humidity measurements from the DHT11 sensor, then
smoothen the measurements, e.g. with a low-pass filter. At least 2 measurements (both
temperature and humidity) should be taken every minute.
2. Give the timestamp (format “yyyy-mm-dd hh:mm:ss”, e.g. 2020-04-13 21:07:09) to each
measurement. The timestamp is to be associated to the actual time in Bangkok
(GMT+7). The MCU must acquire the time information from a reasonably accurate NTP
server running on the same network (the MCU is to be connected to a network using its
built-in WIFI circuit).
3. Store the filtered measurement with timestamp onto the non-volatile memory.
4. The MCU should be able to serve following inquiries from another device on the
network:
a. Asking for the current (most recent) temperature and humidity
b. Asking for the min, max, and average temperature and humidity within the past
hour
c. Asking for the min, max and average temperature and humidity within a given
period of time, e.g. between 2020-04-13 20:00:00 and 2020-04-13 21:05:45


Tasks to be implemented on another device, e.g. a computer, a smartphone
Inquire the temperature and humidity measurement from the MCU in three different ways as
explained in Item 4 above.

Task to be implemented with AWS IoT core
1. Create a MQTT topic in AWS IoT Core
2. User AWS IoT Analytics, create a channel, a pipeline, a data store, and a notebook
3. Use AWS QuickSight to create visualized dashboard
4. Use AWS IoT Analytics to filter data anomaly
