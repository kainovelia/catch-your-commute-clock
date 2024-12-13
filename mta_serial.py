import gtfs_realtime_pb2 as nyc_rt
import requests
#from pyduinobridge import Bridge_py
from datetime import datetime
import serial
import time

arduino = serial.Serial(port='COM7', baudrate=115200, timeout=.1)
url = "https://api-endpoint.mta.info/Dataservice/mtagtfsfeeds/nyct%2Fgtfs-nqrw"
response = requests.get(url)
response_bytes = response.content

msg = nyc_rt.FeedMessage()
msg.ParseFromString(response_bytes)
entity = msg.entity[2]
unix_times = []


while True:
    byte_in = arduino.read()
    string_in = byte_in.decode('utf-8')

    if(string_in == "R"):
        print("R recvd")
        for i in range(len(msg.entity)):
            if(i % 2 == 0):
                for j in range(len(msg.entity[i].trip_update.stop_time_update)):
                    id = msg.entity[i].id
                    stop = msg.entity[i].trip_update.stop_time_update[j].stop_id
                    if(id[6]== 'Q' and stop=="Q04S"):
                        arrTime = msg.entity[i].trip_update.stop_time_update[j].arrival.time
                        arduino.write(bytes(str(arrTime), "utf-8"))  
                        break
    else:
        continue
                        
    print(str(arrTime))