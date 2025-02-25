import socket
import time

# Server IP (update based on WSL ifconfig output)
HOST = '127.0.0.1'  
PORT = 65431          

def get_network_details():
    hostname = socket.gethostname()
    IP_addr  = socket.gethostbyname(hostname)
    fqdn     = socket.getfqdn() 
    print("Host name:", hostname)
    print("IP address:", IP_addr)
    print("FQDN:", fqdn)

print("Lab2: Exp3: Client running on Windows: Welcome !!!")
get_network_details()

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    
    while True:
        # Send message
        s.sendall(b'Hello, server!')
        
        # Receive reply
        data = s.recv(1024)
        print(f"Received {data.decode()} from server")
        
        # Sleep for 2 seconds before sending the next message
        time.sleep(2)