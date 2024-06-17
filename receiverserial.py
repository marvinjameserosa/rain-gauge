import serial
import threading

PORT = None  
BAUDRATE = 9600
data_dict = {
    'rpm': '0.00 rpm',
    'flowrate':'0.00 m3/s',
    'mm':'0.00',
    'intensity':'No Rain',
    'exception': '', 
}

def get_port():
    global PORT
    if not PORT:
        PORT = input("What port is your receiver: ") 
    return PORT

def read_sensor_data():
    global data_dict
    PORT = get_port()
    
    try:
        ser = serial.Serial(port=PORT, baudrate=BAUDRATE)
        data_dict['exception'] = ''
        
        while True:
            raw_data = ser.readline().decode('utf-8').strip()
            data = raw_data.split(':')
            if len(data) == 2:
                data_dict[data[0]] = data[1]
            else:
                print("Unexpected data format:", raw_data)
                
    except serial.SerialException as e:
        data_dict['exception'] = str(e)

PORT = get_port()
threading.Thread(target=read_sensor_data, daemon=True).start()
