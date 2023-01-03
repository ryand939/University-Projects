import argparse
import signal
import sys
from urllib.parse import urlparse
from socket import *


# data structure for any clients that connect to server
class Client:
    def __init__(self, address: str, name: str):
        self.address = address
        self.name = name
        self.inv = []

    # delete an item from the client's inventory
    def deleteItem(self, item: str):
        if item in self.inv:
            self.inv.remove(item)
            return True
        else:
            return False

    # add an item to the client's inventory
    def addItem(self, item: str):
        self.inv.append(item)


# data structure for a room
class Room:
    def __init__(self, desc: str, name: str, items: list[str], roomServers):
        self.desc = desc
        self.name = name
        self.items = items
        self.roomServers = roomServers

    # delete an item from the room
    def deleteItem(self, item: str):
        if item in self.items:
            self.items.remove(item)
            return True
        else:
            return False

    # add an item to the room
    def addItem(self, item: str):
        self.items.append(item)

    # string representation of room and its items
    def toString(self):
        nameStr = self.name + "\n\n"
        descStr = self.desc + "\n"
        doorStr = ""
        for entry in self.roomServers:
            if self.roomServers[entry] != None:
                doorStr += f"A doorway leads away from the room, leading {entry}\n"
        doorStr += "\n"   
        if len(self.items) == 0:
            itemStr = "There are no items in this room."
        else:
            itemStr = "In this room, there is: "
            for x in self.items:
                itemStr += "\n  " + x
        return nameStr + descStr + doorStr + itemStr


# finds and returns a client based on their address. Returns false if client is not found.
def getClient(address: str, clients: list[Client]):
    for x in clients:
        if x.address == address:
            return x
    return False


# checks if the name is taken and recursively adds numbers to the 
# end of the name until an available name is found
def nameDupe(clientList, name, counter=0):
    counter += 1
    for client in clientList:
        if client.name == name:
            if counter != 1: newName = name[:-len(str(counter))] + str(counter)
            else: newName = name + str(counter)
            return nameDupe(clientList, newName, counter)
    return name


# gets a list of the players connected to the server excluding the calling client
def playerList(clientList, clientName):
    rtnStr = ""
    for client in clientList:
        #print("debug: ", client.name)
        if client.name != clientName:
            rtnStr += "\n  " + client.name
    return rtnStr


# broadcasts a message to all clients, optionally excluding one player
def broadcastMessage(clientList, message, serverSocket, excluding=None):
    for client in clientList:
        if client != excluding:
            serverSocket.sendto(message.encode(), client.address)


# switches the room of a client and returns the new clientlist for this server
def switchRoom(roomServers, serverSocket, clientList, currClient, direction, discovery):
    if roomServers[direction] != None:
        # contact discovery to get the server


        serverSocket.sendto(f"LOOKUP {roomServers[direction]}".encode(), discovery)
        message, addr = serverSocket.recvfrom(2048)
        sideRoom = message.decode().split()


        # get list of items for the client to take
        newServer = str(urlparse(sideRoom[1]).port) + " "
        if currClient.inv != []:
            for item in currClient.inv:
                newServer += item + " "
        else:
            newServer += "empty "
        serverSocket.sendto(str(newServer[:-1]).encode(), currClient.address)
        broadcastMessage(clientList, f"\n{currClient.name} left the room, heading {direction}.", serverSocket, currClient)
        print(f"User {currClient.name} left the room.")
        clientList.remove(currClient)
    else:
        serverSocket.sendto(f"There is no door in that direction.".encode(), currClient.address)

    return clientList


def main():
    # signal handler for Ctrl+C
    def signalHandler(sig, frame):
        broadcastMessage(clientList, "server_disconnect", serverSocket)
        serverSocket.sendto(f"DEREGISTER {startingRoom.name}".encode(), discovery)
        print("Interrupt received, shutting down...")
        sys.exit(0)
    signal.signal(signal.SIGINT, signalHandler)


    # handle command line arguments
    parser = argparse.ArgumentParser(description="Room Starting Description:")
    parser.add_argument('-u', type=str, nargs='?', help="Optional server on an upper level")
    parser.add_argument('-d', type=str, nargs='?', help="Optional server on a lower level")
    parser.add_argument('-n', type=str, nargs='?', help="Optional server to the north")
    parser.add_argument('-e', type=str, nargs='?', help="Optional server to the east")
    parser.add_argument('-s', type=str, nargs='?', help="Optional server to the south")
    parser.add_argument('-w', type=str, nargs='?', help="Optional server to the west")
    parser.add_argument('name', metavar='N', type=str, help="The name of the room running on the server.")
    parser.add_argument('desc', metavar='D', type=str, help="A textual description of the room.")
    parser.add_argument('items', metavar='I', type=str, nargs='*', help="List of items found in the room initially.")
    args = parser.parse_args()
    

    # setup server socket
    serverSocket = socket(AF_INET, SOCK_DGRAM)
    serverSocket.bind(('', 0))  # port 0 will automatically get an available port
    timeoutValue = 5
    serverSocket.settimeout(timeoutValue) # incase discovery service isnt running we want a timeout
    serverPort = serverSocket.getsockname()[1] # get the randomly assigned port

    # create new room
    startingRoom = Room(args.desc, args.name, args.items, {
        "up":args.u, "down":args.d,
        "north":args.n, "east":args.e,
        "south":args.s, "west":args.w })

    # hardcoded hostname and port for discovery service
    discovery = ("localhost", 3357)

    # list to store all clients/players connected to this room server
    clientList = []

    # register room server on discovery service if it is running
    serverSocket.sendto(f"REGISTER room://localhost:{serverPort} {startingRoom.name}".encode(), discovery)
    try: message, addr = serverSocket.recvfrom(2048)
    except timeout: 
        print(f"Failed to reach discovery service, is it running? Exiting...")
        serverSocket.close()
        sys.exit(0)

    # parse discovery service reply
    discoveryReply = message.decode().split(" ", 1)

    # case where room already exists
    if(discoveryReply[0] == "NOTOK"): 
        print(discoveryReply[1])
        serverSocket.close()
        sys.exit(0)

    # room successfully registered on discovery service, 
    # print some info and enter main loop
    print(discoveryReply[1] + "\n")
    print(startingRoom.toString())
    print("\nRoom will wait for players at port:", serverPort)

    # disable timeout now that we are registered in the discovery service,
    # we want to be able to wait extended periods of time
    serverSocket.settimeout(None)

    # main loop
    while True:
        # receive message from client
        message, clientAddress = serverSocket.recvfrom(2048)

        #determine what known (or unknown) client is sending request
        currClient = getClient(clientAddress,clientList)

        # decode client message and split for interpretation
        cmd = message.decode().split()

        # check if prefix is valid cmd
        if len(cmd) != 0 and cmd[0].lower() in ["join", "take", "drop", "inventory", "say"] and len(cmd) > 1:
            if cmd[0].lower() == "join":
                # check if the client already exists
                if not currClient:
                    client = Client(clientAddress, nameDupe(clientList, cmd[1]))
                    if len(cmd) > 2 and cmd[2] != "empty":
                        for item in cmd[2:]:
                            client.addItem(item)

                    clientList.append(client)
                    roomStr = startingRoom.toString() + playerList(clientList, client.name)
                    serverSocket.sendto(roomStr.encode(), clientAddress)
                    broadcastMessage(clientList, f"\n{client.name} entered the room.", serverSocket, client)
                    print("User", client.name, "joined from address", clientAddress)
                    
                else: # client already connected
                    serverSocket.sendto("You're already in the server!".encode(), clientAddress)


            elif cmd[0].lower() == "take":
                if startingRoom.deleteItem(cmd[1].lower()): # confirm item is found and deleted
                    currClient.addItem(cmd[1].lower()) # add item to player inv
                    serverSocket.sendto((cmd[1].lower() + " taken.").encode(), clientAddress) # update player on game info
                else:
                    serverSocket.sendto(("No " + cmd[1].lower() + " found.").encode(), clientAddress) # inform player item wasnt found


            elif cmd[0].lower() == "drop":
                if currClient.deleteItem(cmd[1].lower()): # confirm item is found and deleted
                    startingRoom.addItem(cmd[1].lower()) # add item to room
                    serverSocket.sendto((cmd[1].lower() + " dropped.").encode(), clientAddress) # update player on game info
                else:
                    serverSocket.sendto(("You are not holding " + cmd[1].lower() + ".").encode(), clientAddress) # inform player item wasnt found

            elif cmd[0].lower() == "say":
                buildMsg = ""
                for word in cmd[1:]:
                    buildMsg += word + " "
                serverSocket.sendto(f"You said \"{buildMsg[:-1]}\"".encode(), clientAddress)
                broadcastMsg = "\n" + currClient.name + f" said \"{buildMsg[:-1]}\"."
                broadcastMessage(clientList, broadcastMsg, serverSocket, currClient)


        elif len(cmd) != 0 and cmd[0].lower() == "inventory":
            itemList = currClient.inv
            if len(itemList) == 0:
                serverSocket.sendto("You are holding nothing!".encode(), clientAddress)
            else:
                buildStr = "You are holding:"
                for x in itemList:
                    buildStr += "\n  " + x
                serverSocket.sendto(buildStr.encode(), clientAddress)

        elif len(cmd) != 0 and cmd[0].lower() == "look":
            roomStr = startingRoom.toString() + playerList(clientList, currClient.name)
            serverSocket.sendto(roomStr.encode(), clientAddress)

        elif len(cmd) != 0 and cmd[0].lower() == "exit":
            # copy list because im going to be deleting elements while iterating through
            tempInv = currClient.inv.copy()

            # delete each item in the client inv, and at the same time add it to room
            for x in tempInv:
                if currClient.deleteItem(x):
                    startingRoom.addItem(x)

            # announce player exiting server
            broadcastMsg = "\n" + currClient.name + " left the game."
            broadcastMessage(clientList, broadcastMsg, serverSocket, currClient)

            # remove from clientlist
            clientList.remove(currClient)
        
        # player entered command "say" but didnt provide anything to say
        elif len(cmd) != 0 and cmd[0].lower() == "say":
            serverSocket.sendto(("What did you want to say?").encode(), clientAddress)

        # player wants to switch rooms
        elif len(cmd) != 0 and cmd[0].lower() in ["north", "east", "south", "west", "up", "down", ]:
            clientList = switchRoom(startingRoom.roomServers, serverSocket, clientList, currClient, cmd[0].lower(), discovery)

        else:
            serverSocket.sendto("Invalid input.".encode(), clientAddress)


# main entry point
if __name__ == "__main__":
    main()



