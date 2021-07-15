# TCP-socket-in-C
A C program to create a TCP socket between two devices in the same network.
The server has a simple function of a calculator. It receives two arguments and an operation and retrives a solution to the client. The server can attend multiple pettitions of a client and multiple clients.
Right now the socket is create between two proccess in the same machine. To change that, the client file must be changed, specifying the server IP insteand of using loopback.
