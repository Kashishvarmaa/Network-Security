import socket

# Server settings
HOST = '0.0.0.0'  # Listen on all interfaces
PORT = 65432       # Port to listen on

print("Lab1: Exp2:Server running on windows: Welcome !!!")

def getNetworkDetails():
    hostname = socket.gethostname()
    IP_addr  = socket.gethostbyname(hostname)
    # Get Fully Qualified Domain Name (FQDN)
    fqdn     = socket.getfqdn() 
    
    print("Host name: ", hostname)
    print("IP address: ", IP_addr)
    print("FQDN: ", fqdn)
    

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    getNetworkDetails()
    s.bind((HOST, PORT))
    s.listen()
    print(f"Server listening on {HOST}:{PORT}")
    while True:
        conn, addr = s.accept()
        with conn:
            print('Connected by', addr)
            while True:
                data = conn.recv(1024)
                if not data:
                    break
                print(f"Received: {data.decode()}")
                conn.sendall(data)  # Echo back
            
            
