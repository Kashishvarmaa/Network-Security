# Lab 3: Exp1 Client Symmetric Key AES
# Prior to running this program make sure the below package is installed
# Client program should be run only after the Server is running on WSL
# On Windows command prompt give the below command.
# pip install cryptography
# python Lab3_Exp1_Sym_Client_Win.py

import socket
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.backends import default_backend

HOST = '127.0.0.1'
PORT = 65431

# Same key and IV as in the Server written in C
key = b"0123456789abcdef"  # 16 bit key
iv  = b"abcdef9876543210" # 16 bit initialising vector 

def encrypt(plaintext):
    backend = default_backend()
    cipher = Cipher(algorithms.AES(key), modes.CBC(iv), backend=backend)
    encryptor = cipher.encryptor()
    # Pad plaintext to a multiple of 16 bytes
    pad_len = 16 - (len(plaintext) % 16)
    plaintext += bytes([pad_len]) * pad_len
    return encryptor.update(plaintext) + encryptor.finalize()

def decrypt(ciphertext):
    backend = default_backend()
    cipher = Cipher(algorithms.AES(key), modes.CBC(iv), backend=backend)
    decryptor = cipher.decryptor()
    plaintext = decryptor.update(ciphertext) + decryptor.finalize()
    pad_len = plaintext[-1]
    return plaintext[:-pad_len]

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    message = b"Hello from symmetric client"
    s.sendall(encrypt(message))
    data = s.recv(1024)
    print("Received from server:", decrypt(data).decode())
