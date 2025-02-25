# Lab 3: Exp2 Asymmetric Key RSA Client
# Install dependencies using: pip install cryptography
# Run this client on Windows AFTER starting the server on WSL
# python Lab3_Exp2_Asym_Client_Win.py

import socket
from cryptography.hazmat.primitives import serialization, hashes
from cryptography.hazmat.primitives.asymmetric import padding

HOST = '127.0.0.1'  # Change this to your WSL IP (use `ifconfig` on WSL)
PORT = 65432

# Load the server's public key from public.pem
with open("public.pem", "rb") as key_file:
    public_key = serialization.load_pem_public_key(key_file.read())

# Load the private key from private.pem (for decrypting server response)
with open("private.pem", "rb") as key_file:
    private_key = serialization.load_pem_private_key(key_file.read(), password=None)

# Encrypt the message with the server's public key
message = b"Hello from asymmetric client"
ciphertext = public_key.encrypt(
    message,
    padding.PKCS1v15()
)

print("🔒 Encrypted message sent (HEX):", ciphertext.hex())

# Create a socket connection
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.sendall(ciphertext)  # Send encrypted message
    encrypted_response = s.recv(512)  # Receive encrypted response

    print("\n📩 Encrypted response from server (HEX):", encrypted_response.hex())

    # Decrypt the response using the private key
    decrypted_message = private_key.decrypt(
        encrypted_response,
        padding.PKCS1v15()
    )

    print("\n🔓 Decrypted response from server:", decrypted_message.decode())