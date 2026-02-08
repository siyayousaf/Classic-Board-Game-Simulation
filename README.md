Classic Board Game Simulation
A classic board game simulation implemented in C, focusing on systems programming fundamentals, memory management, and command-line execution.
Overview
This project is a terminal-based classic board game simulation written in C. The program emphasizes low-level programming concepts such as memory allocation, structured program design, and compilation using a Makefile. The game logic is executed entirely through the command line.
Features
Classic board game mechanics implemented in C
Dynamic memory allocation and pointer usage
Command-line based gameplay
Build automation using a Makefile
Clear separation of game logic and utility functions
How the Program Works
The program simulates a board-based game using C:
The game board is created and managed using dynamic memory
Game state is updated based on player actions
Memory is allocated and freed appropriately to avoid leaks
The program is compiled using GCC via a Makefile and run from the terminal
Technologies Used
C
GCC
Make
Unix/Linux Command Line
Dynamic Memory Management
How to Run
To compile the project:
make
To run the program:
./executable_name
To clean build files:
make clean
