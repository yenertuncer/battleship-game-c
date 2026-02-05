# Battleship Game in C

## Description
A console-based Battleship game written in C. The game features a 10x10 board,
random ship placement, file-based ship storage, move logging, and replay support.

## Features
- 10x10 Battleship board
- Random ship placement with collision detection
- File-based ship configuration (`ships.txt`)
- Move logging system (`battleship_log.txt`)
- Hit / miss / sunk ship detection
- Input validation with recursive retry
- Replay or exit option after game completion

## Ships
The game includes the following ships:
- 1 ship of size 4
- 2 ships of size 3
- 1 ship of size 2

## Technologies
- C
- Standard C libraries
- File I/O

## How to Compile and Run
```bash
gcc battleship.c -o battleship
./battleship
