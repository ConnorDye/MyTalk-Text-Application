#MyTalk Text and Messaging Application
==============

This program allows a user on one machine to send and receive messages from another user on another machine. This program implements a server and client.

Created by Connor Dye as an California Polytechnic University Project.

# Features
--------
* program has two modes: client and server. In server mode, the program opens a network socket and listens for connections
from a client elsewhere in the world. In client mode, the program opens a socket and will attempt to connect to a server on a remote host
* supports arguments `-v`, `-a`, `-N` where -v increases verbosity, , and -N will not start ncurses windowing
* usage: mytalk [ -v ] [ -a ] [ -N ] [ hostname ] port. If a hostname is present then mytalk will act as a client.
If a hostname is not present, mytalk will act as a server, opening a listening socket on the local machine at the given port.
* The **-v** option increases verbosity. Used for debugging debugging purposes: allows user to turn on and off code that narrates what your program is doing.
* The **-a**option tells the server to accept all connections without asking. Again used for debugging purposes as -a allows for the server to accept a connection without asking
* The **-N** option tells mytalk not to start the ncurses windowing. Used for debugging purposes as it will interact better with gdb
* Client and server use TCP (SOCK_STREAM) for communications
* When the **server** starts it opens a socket on the given port and listens for connection attempts
from a client.
* When a client starts, it attempts to open a connection to the server on the given host at the given port. 
If the connection is established, the client sends a packet containing the user’s username and waits for a response.
.It will display a message like the following "Waiting for response from hostname."
* When the connection is established, the server displays a message of the form: "Mytalk requesst from user@hostname. Accept (y/n)?" If the answer is anything other than "ok", connection is closed by the server.
* If the connected succeeds, both server and client switch to a graphical mode where
text from the remote machine is displayed in the top half of the screen and local text is
displayed in the lower half using the ncurses library

# Pro










## Notes
- `list.c` is a custom built and error tested linked list data structure library that works on the same node struct as tree.c
- `tree.c` is a custom built binary tree data structure library that works on the same node struct as list.c
- together these allow for the huffman algorithm to be carried out:  **`1.)`** generate a histogram of all the characters in the file  **`2.)`** generate a linked list of the characters in ascending order of frequency  **`3.)`** the first two nodes of the list become a tree with the root being the sum of the two removed nodes and the left and right child being the removed nodes  **`4.)`** reinsert tree from the previous step at the head of the list and repeat until your list consists of only a single node  **`5.)`** trace the tree to get the huffman codes to encode
- `bit.c` is our bit library that allows us to compress based on the huffman codes; this library allows for an abstraction of the handling of bits to compress and decompress
- helper functions to create headers to recreate the tree are included in hdecode.c and hencode.c
- hencode.c and hdecode.c includes the parsing and main functionality to put all libraries together