import socket
from cryptography.hazmat.primitives import serialization, hashes
from cryptography.hazmat.primitives.asymmetric import padding

HOST = '127.0.0.1'  # Change to your WSL IP if needed
PORT = 65432

# Load the server's public key
with open("public.pem", "rb") as key_file:
    public_key = serialization.load_pem_public_key(key_file.read())

# Load the private key (for decrypting responses)
with open("private.pem", "rb") as key_file:
    private_key = serialization.load_pem_private_key(key_file.read(), password=None)

# Encrypt the message using the server's public key
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

    # Receive first encrypted response from the server
    encrypted_response = s.recv(512)
    print("\n📩 Encrypted response from server (HEX):", encrypted_response.hex())

    # Decrypt the response
    decrypted_message = private_key.decrypt(
        encrypted_response,
        padding.PKCS1v15()
    )
    print("\n🔓 Decrypted response from server:", decrypted_message.decode())

    # Receive additional encrypted message from the server
    encrypted_additional_response = s.recv(512)
    print("\n📩 Encrypted additional message from server (HEX):", encrypted_additional_response.hex())

    # Decrypt the additional message
    decrypted_additional_message = private_key.decrypt(
        encrypted_additional_response,
        padding.PKCS1v15()
    )
    print("\n📩 Additional message from the Server received is:\n", decrypted_additional_message.decode())