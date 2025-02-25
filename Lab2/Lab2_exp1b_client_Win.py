import socket

# Server address (replace with the server's IP)
# Replace the below IP address based on what you see on WSL for ifconfig cmd
HOST = '127.0.0.1'  # WSL: IP address assigned on Ubuntu

PORT = 65431            # Same port as the server


def getNetworkDetails():
    hostname = socket.gethostname()
    IP_addr  = socket.gethostbyname(hostname)
    # Get Fully Qualified Domain Name (FQDN)
    fqdn     = socket.getfqdn() 
    
    print("Host name: ", hostname)
    print("IP address: ", IP_addr)
    print("FQDN: ", fqdn)
    
        
print("Lab2: Exp1b:Client running on Windows: Welcome !!!")
getNetworkDetails()

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.sendall(b'Hello, server!')
    data = s.recv(1024)
    print(f"Received '{data.decode()}' from server")

    # Added the below code to do half-close and wait for data from server
    s.shutdown(socket.SHUT_WR)
    # After this step DO NOT TRY SENDING any data to the server
    # But, you are allowed to receive data until server closes from its end
    while True:
        data = s.recv(1024)
        if not data:
            break
        print(f"Received '{data.decode()}' from server")



