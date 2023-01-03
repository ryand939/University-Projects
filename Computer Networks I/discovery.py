import signal
import sys
from urllib.parse import urlparse
from socket import *
from datetime import datetime

# data structure for server
class Server:
    def __init__(self, host, roomName, port):
        self.host = host
        self.roomName = roomName
        self.port = port
    def toString(self):
        return f"room://{self.host}:{self.port}"


def responseMessage(status, discoverySocket, recipient, message=""):
    if status: discoverySocket.sendto(f"OK {message}".encode(), recipient)
    else:       discoverySocket.sendto(f"NOTOK {message}".encode(), recipient)


# checks if there is a server with the given name
def lookupServer(serverList, roomName):
    for server in serverList:
        if server.roomName == roomName:
            return server
    return None
    
def getServerList(serverList):
    rtnStr = "The discovery service recognizes the following room servers: "
    for server in serverList:
        rtnStr += server.roomName + ", "
    return rtnStr[:-2]


def main():
    def signalHandler(sig, frame):
        discoverySocket.close()
        print("Interrupt received, shutting down discovery service...")
        sys.exit(0)
    
    discoveryPort = 3357
    discoverySocket = socket(AF_INET, SOCK_DGRAM)
    discoverySocket.bind(('', discoveryPort))

    serverList = []

    signal.signal(signal.SIGINT, signalHandler)

    while True:
        # receive message
        message, clientAddress = discoverySocket.recvfrom(2048)
        
        # get current time to log action
        currTime = datetime.now().strftime("%H:%M:%S")

        # decode message and split for interpretation
        cmd = message.decode().lower().split()

        # cmd[0]: register, 
        # cmd[1]: room://host:port, 
        # cmd[2]: name
        if len(cmd) > 1 and cmd[0] == "register": 
            if lookupServer(serverList, cmd[2]) == None:
                newServer = Server(urlparse(cmd[1]).hostname, cmd[2], urlparse(cmd[1]).port)
                serverList.append(newServer)
                responseMessage(True, discoverySocket, clientAddress, f"Successfully registed server room \"{cmd[2]}\" on discovery service")
                print(f"[{currTime}] Registered new room server: \"{newServer.roomName}\" at {newServer.toString()}\n\t   {getServerList(serverList)}")
            else:
                responseMessage(False, discoverySocket, clientAddress, f"Server room \"{cmd[2]}\" already exists!")

            
        # cmd[0]: deregister, 
        # cmd[1]: name
        elif len(cmd) > 1 and cmd[0] == "deregister":
            server = lookupServer(serverList, cmd[1])
            if server == None:
                responseMessage(False, discoverySocket, clientAddress, f"The discovery service does not recognize server room \"{cmd[1]}\", can't remove!")
            else:
                serverList.remove(server)
                responseMessage(True, discoverySocket, clientAddress, f"The discovery service successfully deregisted server room: \"{cmd[1]}\"")
                print(f"[{currTime}] Deregistered room server: \"{server.roomName}\" at {server.toString()}\n\t   {getServerList(serverList)}")
        
        # cmd[0]: lookup, 
        # cmd[1]: name
        elif len(cmd) > 1 and cmd[0] == "lookup":
            server = lookupServer(serverList, cmd[1])
            if server != None: # case server was found
                responseMessage(True, discoverySocket, clientAddress, server.toString())
                print(f"[{currTime}] Responding to lookup request: Found room \"{server.roomName}\" at {server.toString()}")
            else: # case server DNE
                responseMessage(False, discoverySocket, clientAddress, f"The discovery service does not recognize server room \"{cmd[1]}\"")
                print(f"[{currTime}] Responding to lookup request: Could not find room \"{cmd[1]}\"\n\t   {getServerList(serverList)}")




# main entry point
if __name__ == "__main__":
    main()