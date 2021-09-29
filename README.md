# Wolff_Lab_WEIGHT_SENSOR
The arduino and python code used with a strain gauge and HX711 for weighing rats in the automatic skinner boxes in the Wolff lab

udp_sensor.ino sets up the arduino to talk to an HX711 breakout board (read sensor data) and then write that sensor data over an ethernet connection (using an ip and MAC address). the python script 'Fetch_Weight.py' does exactly that; it writes request over the network to the arduino for the sensor data. When the arduino recieves a request, it sends the data over the ethernet connection to the requesting entity (aka our computer sending the request). Technically, the arduino is called a 'server' and our computer is the 'client' and we are pinging the server for data. 
