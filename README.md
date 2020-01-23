# ChessyChess
C++ Chess Client Model using Qt Creator.
Presented with a minimal server in C, supporting parallel matches. Includes a database containing personal history. Running under localhost, PORT 3490.

Usage: 
gcc ChessServer.c -o server.exe
./server.exe
compiled&run using Qt

Note: Due to the sensitivity of the interface in Qt, the synchronisation of the moves is delayed. However, the logical scheme behind the process does work, as presented by the server.
