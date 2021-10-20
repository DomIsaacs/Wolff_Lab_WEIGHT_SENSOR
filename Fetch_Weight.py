from socket import *
import time
import xlwt
from xlwt import Workbook

path = 'C:/Users/domis/projects/weight_sensor_test_folder/xlwt_example.xls'



# Workbook is created
wb = Workbook()
sheet1 = wb.add_sheet('Sheet 1')

# set global variable flag
address =('10.90.125.56', 1900) #define server IP and port, this may be a little tricky, each mega is assigned a different IP (but they can all talk to the same port, at least one at a time)
#address = ('10.90.125.43',1900)
client_socket=socket(AF_INET, SOCK_DGRAM) #Set up the 'Socket' which is just a dumb name for a communication chnnel
client_socket.settimeout(2) #Only wait 2 second for a response
 
while True:
    
    #we ask for temprature
    data_request = "req" #Set data request to weight
    client_socket.sendto(str.encode(data_request), address) #Send the data request
 
    try:
 
        rec_data, addr = client_socket.recvfrom(2048) #Read response from arduino
        weight = float(rec_data) #Convert string rec_data to float temp
        print("weight:",weight, "g.")# Print the result

 
    except:
        pass
    
    time.sleep(2) #delay before sending next command

    
    print("")

sheet1.write(1, 0, "wei")
sheet1.write(2, 0, weight)
wb.save(path)
