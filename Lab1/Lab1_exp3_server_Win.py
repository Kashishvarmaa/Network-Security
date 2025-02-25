import socket
import threading

# Server settings
HOST = '0.0.0.0'  # Listen on all interfaces
PORTS = [65431, 65432, 65433, 65434, 65435]  # Ports to listen on

print("Lab1: Exp3:Server running on Windows (Always Listening): Welcome !!!")

def getNetworkDetails():
    hostname = socket.gethostname()
    IP_addr  = socket.gethostbyname(hostname)
    # Get Fully Qualified Domain Name (FQDN)
    fqdn     = socket.getfqdn() 
    
    print("Host name: ", hostname)
    print("IP address: ", IP_addr)
    print("FQDN: ", fqdn)

def handle_client(conn, addr, serial_number):
    print(f'Connected by {addr} on serial number {serial_number}')
    while True:
        data = conn.recv(1024)
        if not data:
            break
        message = f"Serial {serial_number}: {data.decode()}"
        print(f"Received from {serial_number}: {message}")
        conn.sendall(message.encode())  # Echo back
    print(f'Connection closed for serial number {serial_number}')
    conn.close()

def start_server(port, serial_number):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST, port))
        s.listen()
        print(f"Server listening on {HOST}:{port} with serial number {serial_number}")
        while True:
            conn, addr = s.accept()
            threading.Thread(target=handle_client, args=(conn, addr, serial_number)).start()

getNetworkDetails()

# Create and start a thread for each port
for i, port in enumerate(PORTS):
    threading.Thread(target=start_server, args=(port, i+1)).start()