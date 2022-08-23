MyTalk Text and Messaging Application
==============

This program allows a user on one machine to send and receive messages from another user on another machine. This program program has two modes: client and server. In server mode, the program opens a network socket and listens for connections from a client elsewhere in the world. In client mode, the program opens a socket and will attempt to connect to a server on a remote host

Created by Connor Dye as an California Polytechnic University Project.

Features
--------
* usage: mytalk [ -v ] [ -a ] [ -N ] [ hostname ] port. If a hostname is present then mytalk will act as a client. If a hostname is not present, mytalk will act as a server, opening a listening socket on the local machine at the given port.
* supports arguments `-v`, `-a`, `-N` where **-v** increases verbosity, **-a** will accept all connections without asking, and **-N** will not start ncurses windowing
* Client and server use TCP (SOCK_STREAM) for communications
* Message passing stops when either user types either the EOF character (ˆD) or generates a SIGINT (^C) causing that end of the connection to terminate
* **Server** side does the following: **`1.)`** utilizes `socket(2)` to set up for a connection, **`2.)`** attaches an address to the socket with `bind(2)`, **`3.)`** waits for a connection with `listen(2)`, **`4.)`** accepts a connection with `accept(2)`, **`5.)`** sends and receives with `send(2)` and `recv(2)` until done, **`6.)`** closes any remaining sockets with `close(2)`
* **Client** side does the following: **`1.)`** looks up peer address with getaddrinfo(3), **`2.)`** opens up a `socket(2)` for connection, **`3.)`** connect to the server using `connect(2)`, **`4.)`** sends and receives with `send(2)` and `recv(2)` until done, **`5.)`** closes any remaining sockets with `close(2)`





# Notes:
* The **-v** option increases verbosity. Used for debugging debugging purposes: allows user to turn on and off code that narrates what your program is doing.
* The **-a**option tells the server to accept all connections without asking. Again used for debugging purposes as -a allows for the server to accept a connection without asking
* The **-N** option tells mytalk not to start the ncurses windowing. Used for debugging purposes as it will interact better with gdb
* When the **server** starts it opens a socket on the given port and listens for connection attempts
from a client. When a **client** starts, it attempts to open a connection to the server on the given host at the given port. 
* If the connection is established, the client sends a packet containing the user’s username and waits for a response.It will display a message like the following "Waiting for response from *hostname*." When the connection is established, the server displays a message of the form: "Mytalk requesst from user@hostname. Accept (y/n)?" If the answer is anything other than "ok", connection is closed by the server.
* If the connected succeeds, both server and client switch to a graphical mode where
text from the remote machine is displayed in the top half of the screen and local text is
displayed in the lower half using the ncurses library
* Utilizes htonl(3), htons(3), ntohl(3), and ntohs(3) as appropriate
* utilizes ncurses abstraction I/O library
