#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_bomb_percent(char *level) {
    if (!level) return -1;
    if (strcmp(level, "easy") == 0) return 5;
    if (strcmp(level, "medium") == 0) return 20;
    if (strcmp(level, "hard") == 0) return 35;
    return -1;
}

int get_num_bomb(int rows, int cols, int bomb_percent) {
    int total = rows * cols;
    int num = (bomb_percent * total + 50) / 100;
    if (num < 0) num = 0;
    if (num > total) num = total;
    return num;
}

char **setup(int rows, int cols) {
    char **board = (char**)malloc(sizeof(char*) * rows);
    if (!board) return NULL;
    for (int r = 0; r < rows; ++r) {
        board[r] = (char*)malloc(sizeof(char) * cols);
        if (!board[r]) {
            for (int i = 0; i < r; ++i) free(board[i]);
            free(board);
            return NULL;
        }
        for (int c = 0; c < cols; ++c) board[r][c] = '.';
    }
    return board;
}

static int in_bounds(int rows, int cols, int r, int c) {
    return (r >= 0 && r < rows && c >= 0 && c < cols);
}

void init(char **gboard, int rows, int cols, int num_bomb) {
    int placed = 0;
    while (placed < num_bomb) {
        int r = rand() % rows;
        int c = rand() % cols;
        if (gboard[r][c] != '*') {
            gboard[r][c] = '*';
            placed++;
        }
    }
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (gboard[r][c] == '*') continue;
            int count = 0;
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    if (dr == 0 && dc == 0) continue;
                    int nr = r + dr, nc = c + dc;
                    if (in_bounds(rows, cols, nr, nc) && gboard[nr][nc] == '*') count++;
                }
            }
            gboard[r][c] = (char)('0' + count);
        }
    }
}

void print_board(char **board, int rows, int cols) {
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) putchar(board[r][c]);
        putchar('\n');
    }
}

void walker(char **pboard, char **gboard, int rows, int cols, int click_r, int click_c) {
    if (!in_bounds(rows, cols, click_r, click_c)) return;
    if (pboard[click_r][click_c] != '.') return;
    int cap = rows * cols;
    int *qr = (int*)malloc(sizeof(int) * cap);
    int *qc = (int*)malloc(sizeof(int) * cap);
    if (!qr || !qc) { free(qr); free(qc); return; }
    int head = 0, tail = 0;
    pboard[click_r][click_c] = gboard[click_r][click_c];
    if (pboard[click_r][click_c] == '0') {
        qr[tail] = click_r; qc[tail] = click_c; tail++;
    }
    while (head < tail) {
        int r = qr[head], c = qc[head];
        head++;
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                if (dr == 0 && dc == 0) continue;
                int nr = r + dr, nc = c + dc;
                if (!in_bounds(rows, cols, nr, nc)) continue;
                if (pboard[nr][nc] != '.') continue;
                pboard[nr][nc] = gboard[nr][nc];
                if (pboard[nr][nc] == '0') {
                    qr[tail] = nr; qc[tail] = nc; tail++;
                }
            }
        }
    }
    free(qr);
    free(qc);
}

static int count_revealed(char **pboard, int rows, int cols) {
    int cnt = 0;
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c)
            if (pboard[r][c] != '.') cnt++;
    return cnt;
}

void play(char **pboard, char **gboard, int rows, int cols, int num_bomb) {
    int total = rows * cols;
    int safe_tiles = total - num_bomb;
    while (1) {
        int r = rand() % rows;
        int c = rand() % cols;
        if (gboard[r][c] == '*') {
            printf("Click at (%d, %d). Bomb Exploded! Game Over!\n", r, c);
            pboard[r][c] = '!';
            printf("This is the play board after explosion\n");
            print_board(pboard, rows, cols);
            return;
        }
        if (pboard[r][c] == '.') {
            printf("Click at (%d, %d)\n", r, c);
            walker(pboard, gboard, rows, cols, r, c);
            print_board(pboard, rows, cols);
        }
        if (count_revealed(pboard, rows, cols) >= safe_tiles) {
            printf("Game Completed!\n");
            print_board(pboard, rows, cols);
            return;
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 5) {
        printf("invalid inputs\n");
        return 1;
    }
    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);
    char *level = argv[3];
    int seed = atoi(argv[4]);
    int percent = get_bomb_percent(level);
    if (rows <= 0 || cols <= 0 || percent < 0) {
        printf("invalid inputs\n");
        return 1;
    }
    srand((unsigned int)seed);
    int num_bomb = get_num_bomb(rows, cols, percent);
    char **gboard = setup(rows, cols);
    char **pboard = setup(rows, cols);
    if (!gboard || !pboard) {
        if (gboard) { for (int r = 0; r < rows; ++r) free(gboard[r]); free(gboard); }
        if (pboard) { for (int r = 0; r < rows; ++r) free(pboard[r]); free(pboard); }
        printf("invalid inputs\n");
        return 1;
    }
    init(gboard, rows, cols, num_bomb);
    printf("This is the game board after initialization\n");
    print_board(gboard, rows, cols);
    printf("This is the play board after setup\n");
    print_board(pboard, rows, cols);
    printf("The game will start\n");
    play(pboard, gboard, rows, cols, num_bomb);
    for (int r = 0; r < rows; ++r) free(gboard[r]);
    for (int r = 0; r < rows; ++r) free(pboard[r]);
    free(gboard);
    free(pboard);
    return 0;
}

