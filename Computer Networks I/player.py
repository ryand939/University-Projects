import argparse
import signal
import sys
from urllib.parse import urlparse
from socket import *
import select



def main():
    # signal handler for ctrl+c interrupt 
    def signalHandler(sig, frame):
        clientSocket.sendto("exit".encode(), gameServer)
        clientSocket.close()
        sys.exit(0)
    signal.signal(signal.SIGINT, signalHandler)

    # parse args
    parser = argparse.ArgumentParser(description="Server Information")
    parser.add_argument('playerName', metavar='P', type=str, help="Player name to be recognized by server.")
    parser.add_argument('serverName', metavar='N', type=str, help="Name of server.")
    args = parser.parse_args()

    # hardcoded hostname and port for discovery service
    discovery = ("localhost", 3357)

    # setup client socket
    clientSocket = socket(AF_INET, SOCK_DGRAM)
    timeoutValue = 5
    clientSocket.settimeout(timeoutValue)


    # try/except to catch socket.timeout error
    try:
        # lookup server/room name to get address
        clientSocket.sendto(f"LOOKUP {args.serverName}".encode(), discovery)    # send lookup request to discovery service
        message, addr = clientSocket.recvfrom(2048)                             # recieve discovery service response


        discoveryReply = message.decode().split(" ", 1)

        # if failure to find room, print error message and exit
        if(discoveryReply[0] == "NOTOK"): 
            print(discoveryReply[1])
            clientSocket.close()
            sys.exit(0)

        # OK from discovery service, server info will be in reply
        serverInfo = urlparse(discoveryReply[1])
        gameServer = (serverInfo.hostname, serverInfo.port)

        # now "join" the server and enter main loop
        clientSocket.sendto(("join " + args.playerName).encode(), gameServer)
        reply, serverAddress = clientSocket.recvfrom(2048)
        print(reply.decode() + "\n>", end=" ")

        nextCmd = "none"

        while True:
            readables, ignored, ignored = select.select([clientSocket, sys.stdin], [], [])
            for msg in readables:
                # see if message is from socket or stdin
                if msg == clientSocket:
                    reply, serverAddress = clientSocket.recvfrom(2048)
                    # check if the last request was a room switch, we are expecting a reply
                    if nextCmd == "north\n" or nextCmd == "east\n" or nextCmd == "south\n" or nextCmd == "west\n" or nextCmd == "up\n" or nextCmd == "down\n":
                        newServer = reply.decode().split()
                        # if the door exists, the reply will be a port, otherwise will be error message
                        try: 
                            # update the port of the gameServer
                            gameServer = (serverInfo.hostname, int(newServer[0]))
                            itemListStr = ""
                            if len(newServer) > 1:
                                for item in newServer[1:]:
                                    itemListStr += item + " "
                            clientSocket.sendto(("join " + args.playerName + " " + itemListStr).encode(), gameServer)
                            reply, serverAddress = clientSocket.recvfrom(2048)
                        # error message, do nothing but print it
                        except: pass
                        nextCmd = "none"
                    # server shutting down message
                    elif reply.decode() == "server_disconnect":
                        print("\nDisconnected from server ... exiting!")
                        clientSocket.close()
                        sys.exit(0)

                    print(reply.decode() + "\n>", end=" ")
                        
                
                elif msg == sys.stdin:
                    nextCmd = sys.stdin.readline() 
                    clientSocket.sendto(nextCmd.encode(), gameServer)
                    nextCmd = nextCmd.lower()
                    if nextCmd == "exit\n":
                        clientSocket.close()
                        sys.exit(0)
                

    # no reply in 5 seconds, timeout and exit
    except timeout: 
        print(f"No reply recieved in {timeoutValue} seconds ... exiting!")
        clientSocket.close()



# main entry point
if __name__ == "__main__":
    main()
