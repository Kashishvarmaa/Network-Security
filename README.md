# Network Security Labs

This repository contains a series of network security lab experiments covering various topics such as socket programming, symmetric and asymmetric encryption, and network simulation.

## Project Structure

- **Lab_1**: Basic socket programming experiments with client-server communication.
  - Files include client and server implementations for Windows and WSL environments.
  - Experiments demonstrate basic TCP socket communication.

- **Lab_2**: Advanced socket programming with features like half-close and multi-threading.
  - Includes C-based server implementations and Python clients.
  - Demonstrates more complex socket interactions and error handling.

- **Lab_3**: Cryptography experiments focusing on symmetric and asymmetric encryption.
  - Symmetric encryption using AES-128-CBC.
  - Asymmetric encryption using RSA.
  - Includes both client and server implementations for Windows and WSL.

- **Lab_4**: (Directory exists but appears to be empty)

- **Lab_5**: Contains a network simulation file (Lab5.pkt).

- **Lab_6**: Contains network simulation files (Lab6_Exp1.pkt and Lab6_Exp2.pkt).

- **Lab_7**: Contains VLAN-related network simulation files and a PDF assignment.

- **Lab_8**: Contains VPN-related network simulation files.

## Getting Started

### Prerequisites

- Python 3.x
- OpenSSL development libraries (for C-based servers)
- Cryptography library for Python (`pip install cryptography`)
- Network simulation software (for .pkt files)

### Running the Experiments

1. **Lab 1**: Basic socket communication
   - Run the server on WSL: `python Lab1_exp0_server_WSL.py`
   - Run the client on Windows: `python Lab1_exp0_client_Win.py`

2. **Lab 2**: Advanced socket programming
   - Compile the C server: `gcc Lab2_exp1b_server_WSL.c -o server`
   - Run the server on WSL: `./server`
   - Run the client on Windows: `python Lab2_exp1b_client_Win.py`

3. **Lab 3**: Cryptography experiments
   - For symmetric encryption:
     - Compile the server: `gcc Lab3_Exp1_Sym_Server_WSL.c -lssl -lcrypto -o sym_server`
     - Run the server on WSL: `./sym_server`
     - Run the client on Windows: `python Lab3_Exp1_Sym_Client_Win.py`
   - For asymmetric encryption:
     - Ensure public.pem and private.pem are present
     - Compile the server: `gcc Lab3_Exp3c_Asym_Server_WSL.c -lssl -lcrypto -o asym_server`
     - Run the server on WSL: `./asym_server`
     - Run the client on Windows: `python Lab3_Exp3c_Asym_Client_Win.py`

## Notes

- Some files are binary (.pkt) and require specific network simulation software to open.
- The .gitignore file indicates that certain build artifacts and sensitive files (like private keys) are not tracked in the repository.

## License

This project is part of a network security course and is intended for educational purposes. 