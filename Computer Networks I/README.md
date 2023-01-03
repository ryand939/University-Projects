# Client-Server Command Line Game

This game uses the python socket API to simulate rooms (servers) and players (clients). 

## How to run

To use this program, open a few terminal windows and navigate
to the directory which the discovery.py, player.py and room.py files are stored.

### discovery
Command format: python3 discovery.py


### room (server): 
Command format: python3 room.py -u/d/n/e/s/w SIDE_ROOM_NAME ROOM_NAME "The description for the room" "sample-item-1" "sample-item-2" ... "sample-item-N"

A room may only have one door/side room leading in any single direction (UP/DOWN/N/E/S/W).
The side room parameters are optional, and must be provided in the order UP, DOWN, N, E, S, W.



### player (client):
Command format: python3 player.py PLAYER_NAME ROOM_NAME



Sample discovery command:
python3 discovery.py

Sample room commands:
python3 room.py -n Theater Foyer "The entry way to an old house. Weathered, but still tidy and clean." "vase" "rug"
python3 room.py -d Theater Study "An old Study. Some manner of business was apparently run from here." "desk" "lamp" "book" "paper"
python3 room.py -u Study -s Foyer Theater "An empty theater. Comforting but Sad." "unopened-pepsi" "ticket"


Sample player commands:
python3 player.py Bob Foyer
python3 player.py Richard Foyer
python3 player.py Alice Theater


