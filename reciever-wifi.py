import socket

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

host = '0.0.0.0'  
port = 'XXXX'     # Port number 

server_socket.bind((host, port))

server_socket.listen(5)

print(f"Server listening on {host}:{port}")

client_socket = None
addr = None

while True:
    if client_socket is None:
        client_socket, addr = server_socket.accept()
        client_socket.settimeout(3.0) 
        print(f"Connection from {addr}")

    try:
        data = client_socket.recv(1024)
        if not data:
            print("Client disconnected")
            client_socket.close()
            client_socket = None
        else:
            print(f"Received: {data.decode().strip()}")
    except socket.timeout:
        continue
    except socket.error as e:
        print(f"Socket error: {e}")
        if client_socket:
            client_socket.close()
        client_socket = None
    except Exception as e:
        print(f"Error: {e}")
        if client_socket:
            client_socket.close()
        client_socket = None
