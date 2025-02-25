# Lab 3: Exp2 Asymmetric Key RSA
# The below package should have been installed prior to running this program
# This program is to be run on Windows after running the Server first on WSL
# Install the below library before running this python program
# pip install cryptography
# python Lab3_Exp2_Asym_Client_Win.py

import socket
from cryptography.hazmat.primitives import serialization, hashes
from cryptography.hazmat.primitives.asymmetric import padding

HOST = '127.0.0.1'  # Change it to WSL IP address by using ifconfig cmd
PORT = 65432

# Load the server's public key from public.pem
with open("public.pem", "rb") as key_file:
    public_key = serialization.load_pem_public_key(key_file.read())

# Encrypt the message with the server's public key
message = b"Hello from asymmetric client"
ciphertext = public_key.encrypt(
    message,
    padding.PKCS1v15()
)

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.sendall(ciphertext)
    data = s.recv(512)
    # In this simple lab, we assume that the client is not decrypting the server’s reply.
    # In a full implementation, the client would use its own private key to decrypt.
    print("Received encrypted data from server:", data.hex())
