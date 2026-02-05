#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define BOARD_SIZE 10
#define MAX_SHIPS 4


const int shipSizes[] = {4, 3, 3, 2};

void initializeBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = '-';
        }
    }
}

void printBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
    printf("   ");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%d ", i);
    }
    printf("\n");

    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%d  ", i);
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

void placeShipsToFile() {
    FILE *file = fopen("ships.txt", "w");
    if (file == NULL) {
        printf("Ships couldn't be saved in the file!\n");
        return;
    }

    char tempBoard[BOARD_SIZE][BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            tempBoard[i][j] = '-';
        }
    }

    srand(time(0));

    for (int s = 0; s < MAX_SHIPS; s++) {
        int shipSize = shipSizes[s];
        int placed = 0;

        while (!placed) {
            int row = rand() % BOARD_SIZE;
            int col = rand() % BOARD_SIZE;
            int direction = rand() % 2; 
            int valid = 1;

            
            for (int i = 0; i < shipSize; i++) {
                int r = row + (direction == 1 ? i : 0);
                int c = col + (direction == 0 ? i : 0);

                if (r >= BOARD_SIZE || c >= BOARD_SIZE || tempBoard[r][c] != '-') {
                    valid = 0;
                    break;
                }
            }

            if (valid) {
                
                for (int i = 0; i < shipSize; i++) {
                    int r = row + (direction == 1 ? i : 0);
                    int c = col + (direction == 0 ? i : 0);
                    tempBoard[r][c] = 'S'; 
                }

                fprintf(file, "%d %d %d %d\n", row, col, direction, shipSize);
                placed = 1;
            }
        }
    }

    fclose(file);
}


void resetLogFile() {
    FILE *file = fopen("battleship_log.txt", "w");
    if (file == NULL) {
        printf("Move log file couldn't be reset!\n");
        return;
    }
    fprintf(file, "Battleship Game Log\n");
    fclose(file);
}

int getPlayerInput(int *row, int *col) {
    char input[10];
    printf("Enter coordinates (e.g., 4 6, or X to exit): ");
    fgets(input, sizeof(input), stdin);

    
    if (input[0] == 'X' || input[0] == 'x') {
        return 0;
    }

    
    if (sscanf(input, "%d %d", row, col) != 2) {
        printf("Invalid input! Please enter two numbers between 0 and 9 , not characters.\n");
        return getPlayerInput(row, col); 
    }

    
    if (*row < 0 || *row >= BOARD_SIZE || *col < 0 || *col >= BOARD_SIZE) {
        printf("Invalid coordinates! Please enter values between 0 and 9.\n");
        return getPlayerInput(row, col); 
    }

    return 1;
}


void logMove(int row, int col, int hit, int totalShots) {
    FILE *file = fopen("battleship_log.txt", "a");
    if (file == NULL) {
        printf("Move couldn't be written in the file!\n");
        return;
    }

    fprintf(file, "Coordinate: (%d, %d) - %s\n", row, col, hit ? "Hit" : "Miss");
    fprintf(file, "Total shots: %d\n", totalShots);

    fclose(file);
}

int processMove(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, int *totalShots, int *sunkShips) {
    FILE *file = fopen("ships.txt", "r");
    if (file == NULL) {
        printf("File of ships couldn't be read!\n");
        return 0;
    }

    int hit = 0;
    int shipRow, shipCol, direction, shipSize;

    while (fscanf(file, "%d %d %d %d", &shipRow, &shipCol, &direction, &shipSize) == 4) {
        int currentHit = 0;
        int shipHitCount = 0;

        for (int i = 0; i < shipSize; i++) {
            int r = shipRow + (direction == 1 ? i : 0);
            int c = shipCol + (direction == 0 ? i : 0);

            if (r == row && c == col) {
                hit = 1;
                currentHit = 1;
            }
            if (board[r][c] == 'X') {
                shipHitCount++;
            }
        }

        if (currentHit) {
            shipHitCount++;
        }

        if (shipHitCount == shipSize && currentHit) {
            printf("Congratulations! You have sunk a %d-cell ship!\n", shipSize);
            (*sunkShips)++;
        }

        if (hit) break;
    }

    fclose(file);

    (*totalShots)++;
    logMove(row, col, hit, *totalShots);

    if (hit) {
        printf("Hit! (%d, %d)\n", row, col);
        board[row][col] = 'X';
    } else {
        printf("Miss! (%d, %d)\n", row, col);
        board[row][col] = 'O';
    }

    return *sunkShips == MAX_SHIPS;
}

int promptReplay() {
    char input[10];
    printf("Press `N` to start a new game or `X` to exit: ");
    fgets(input, sizeof(input), stdin);

    if (input[0] == 'N' || input[0] == 'n') {
        return 1;
    }

    return 0;
}

int main() {
    char board[BOARD_SIZE][BOARD_SIZE];
    int row, col;
    int totalShots;
    int sunkShips;

    while (1) {
        totalShots = 0;
        sunkShips = 0;

        resetLogFile();
        initializeBoard(board);
        placeShipsToFile();

        int allShipsSunk = 0;

        while (!allShipsSunk) {
            printBoard(board);

            if (!getPlayerInput(&row, &col)) {
                printf("Game finished.\n");
                return 0;
            }

            allShipsSunk = processMove(board, row, col, &totalShots, &sunkShips);
        }

        printBoard(board);
        printf("All ships are sunk! Total shots: %d\n", totalShots);

        if (!promptReplay()) {
            printf("Thank you for playing!\n");
            break;
        }
    }

    return 0;
}

