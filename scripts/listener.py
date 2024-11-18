import socket

if __name__ == "__main__":
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.SOL_UDP)
    # sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
    sock.bind(("127.0.0.1", 5555))
    while True:
        data, addr = sock.recvfrom(256)
        print(addr)
        print(data)
        print("=================================")


