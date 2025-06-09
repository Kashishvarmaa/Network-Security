import socket
import time

def getNetworkDetails():
    hostname = socket.gethostname()
    IP_addr  = socket.gethostbyname(hostname)
    fqdn     = socket.getfqdn() 
    
    print("Host name: ", hostname)
    print("IP address: ", IP_addr)
    print("FQDN: ", fqdn)
    
print("Lab1: Exp3:Client running on WSL (Always Sending): Welcome !!!")
getNetworkDetails()

# Get port number from user input
PORT = int(input("Enter the port number (65431 to 65435): "))

# Server address (replace with the server's IP)
HOST = '127.0.0.1'  # WSL: IP address assigned on Ubuntu

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    count = 1
    while True:
        message = f"{count}. Hello, server!"
        s.sendall(message.encode())
        # data = s.recv(1024)
        # print(f"Received {data.decode()} from server")
        count += 1
        time.sleep(2)