import socket

if __name__ == "__main__":

    HOST = "10.0.1.1"   # Server-Adresse
    PORT = 36769        # Server-Port

    # Daten für den Echo-Befehl
    data = None

    # Opcode 0 (Echo)
    opcode = 1

    length = 2

    # Paket zusammenbauen
    message = bytes([opcode]) + bytes([length]) 

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))

        print("Sende:", message)
        s.sendall(message)

        response = s.recv(1024)

        print("Antwort:", response)