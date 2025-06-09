import socket
import time

# Server address (replace with the server's IP)
HOST = '127.0.0.1'  # WSL: IP address assigned on Ubuntu
PORT = 65432         # Same port as the server


def getNetworkDetails():
    hostname = socket.gethostname()
    IP_addr  = socket.gethostbyname(hostname)
    # Get Fully Qualified Domain Name (FQDN)
    fqdn     = socket.getfqdn() 
    
    print("Host name: ", hostname)
    print("IP address: ", IP_addr)
    print("FQDN: ", fqdn)
    
        
print("Lab1: Exp2:Client running on WSL: Welcome !!!")
getNetworkDetails()

# Continuously run the client connection in a while loop
while True:
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((HOST, PORT))  # Connect to the server
            s.sendall(b'Hello, server!')  # Send message to the server
            data = s.recv(1024)  # Receive response from the server
            
        print(f"Received {data.decode()} from server")
        
        # Sleep for a while before the next connection attempt (optional)
        time.sleep(2)  # Adjust the delay as needed (e.g., 2 seconds)
        
    except ConnectionRefusedError:
        print("Server not available, retrying...")
        time.sleep(2)  # Wait before retrying if the server is not available
    except Exception as e:
        print(f"An error occurred: {e}")
        break  # Exit the loop on other errors