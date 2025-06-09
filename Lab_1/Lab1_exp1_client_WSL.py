import socket

# Server address (replace with the server's IP)
# Replace the below IP address based on what you see on WSL for ifconfig cmd
HOST = '172.17.4.27'  # WSL: IP address assigned on windows

PORT = 65432            # Same port as the server


def getNetworkDetails():
    hostname = socket.gethostname()
    IP_addr  = socket.gethostbyname(hostname)
    # Get Fully Qualified Domain Name (FQDN)
    fqdn     = socket.getfqdn() 
    
    print("Host name: ", hostname)
    print("IP address: ", IP_addr)
    print("FQDN: ", fqdn)
    
        
print("Lab1: Exp1:Client running on Windows: Welcome !!!")
getNetworkDetails()

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.sendall(b'Hello, server!')
    data = s.recv(1024)

print(f"Received {data.decode()} from server")
