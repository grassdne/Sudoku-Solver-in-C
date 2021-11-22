#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>


#define MAX_SOLUTIONS 1000000
#define LEN 9
#define BOX_LENGTH 3

typedef char small;

typedef small board_arr[LEN][LEN];

bool possible_at(board_arr board, int row, int col, small num);
bool solve_easy_tiles(board_arr board);
void board_pretty_str(board_arr board, char *result);
void solve(board_arr board);
void copy_board_to_solutions(board_arr board);
void realloc_solutions();

size_t test;
bool abort_solving = false;
int num_solutions;
int solutions_length = 32;
board_arr *solutions;

void copy_board_to_solutions(board_arr board) {
    for (int i = 0; i < LEN; ++i) {
        for (int j = 0; j < LEN; ++j) {
            solutions[num_solutions-1][i][j] = board[i][j];
        }
    }
}

bool solve_easy_tiles(board_arr board) {
    bool useful = false;
    for (int i = 0; i < LEN; ++i) {
        for (int j = 0; j < LEN; ++j) {
            if (board[i][j] == 0) {
                small choice = 0;
                for (small n = 1; n <= LEN; ++n) {
                    if (possible_at(board, i, j, n)) {
                        if (choice) {
                            goto SKIP;
                        }
                        else {
                            choice = n;
                        }
                    }
                }
                board[i][j] = choice;
                useful = true;
            }
            SKIP: ;
        }
    }
    return useful;
}


void board_pretty_str(board_arr board, char *result) {
    for (int i = 0; i < LEN; ++i) {
        if (i && i % BOX_LENGTH == 0) {
            for (int j = 0; j < LEN*3 + 3 * (BOX_LENGTH - 1); ++j) {
                *result++ = '~';
            }
            *result++ = '\n';
        }

        for (int j = 0; j < LEN; ++j) {
            small v = board[i][j];

            if (j && j % BOX_LENGTH == 0) {
                *result++ = ' ';
                *result++ = '|';
                *result++ = ' ';
            }

            if (v == 0) {
                *result++ = ' ';
                *result++ = '\'';
                *result++ = ' ';
            }
            else {
                *result++ = ' ';
                if (v < 10) {
                    *result++ = (char)('0' + v);
                }
                else {
                    *result++ = (char)('A' + v - 10);
                }
                *result++ = ' ';
            }

        }
        *result++ = '\n';
    }
    *result = '\0';
}


void solve(board_arr board) {
    for (int i = 0; i < LEN; ++i) {
        for (int j = 0; j < LEN; ++j) {

            if (board[i][j] == 0) {
                for (small n = 1; n <= LEN; ++n) {
                    if (possible_at(board, i, j, n)) {
                        board[i][j] = n;
                        solve(board);
                        board[i][j] = 0;
                        if (abort_solving) return;
                    }
                }
                return;
            }
        }
    }

    ++num_solutions;

    if (num_solutions >= MAX_SOLUTIONS)
        abort_solving = true;
    else if (num_solutions >= solutions_length)
        realloc_solutions();

    copy_board_to_solutions(board);
}

void realloc_solutions() {
    printf("Reallocating solutions array from length %d ", solutions_length);
    solutions_length *= 2;

    if (solutions_length > MAX_SOLUTIONS) solutions_length = MAX_SOLUTIONS;
    printf("to %d\n", solutions_length);
    board_arr *tmp_solutions = realloc(solutions, sizeof(board_arr) * solutions_length);
    if (!tmp_solutions) {
        printf("Unable to allocate memory for more solutions. Abortting solving.\n");
        abort_solving = true;
        return;
    }
    solutions = tmp_solutions;
}

bool possible_at(board_arr board, int row, int col, small num) {
    for (int i = 0; i < LEN; ++i) {
        if (num == board[i][col] ||
            num == board[row][i])
            return false;
    }

    int col_start = col / BOX_LENGTH * BOX_LENGTH;
    int row_start = row / BOX_LENGTH * BOX_LENGTH;
    for (int i = row_start; i < row_start + BOX_LENGTH; ++i) {
        for (int j = col_start; j < col_start + BOX_LENGTH; ++j) {
            if (board[i][j] == num) return false;
        }
    }

    return true;
}


int main() {
    assert(BOX_LENGTH * BOX_LENGTH == LEN);
    num_solutions = 0;
    solutions = malloc(sizeof(board_arr) * solutions_length);

    /*
    assert(LEN == 9 && BOX_LENGTH == 3);
    board_arr board = {
            {5, 3, 0, 0, 7, 0, 0, 0, 0},
            {6, 0, 0, 1, 9, 5, 0, 0, 0},
            {0, 9, 8, 0, 0, 0, 0, 6, 0},
            {8, 0, 0, 0, 6, 0, 0, 0, 3},
            {4, 0, 0, 8, 0, 3, 0, 0, 1},
            {7, 0, 0, 0, 2, 0, 0, 0, 6},
            {0, 6, 0, 0, 0, 0, 2, 8, 0},
            {0, 0, 0, 4, 1, 9, 0, 0, 0},
            {0, 0, 0, 0, 8, 0, 0, 7, 0},
    };
    //*/

    //*
    assert(LEN == 9 && BOX_LENGTH == 3);
    board_arr board = {
            {0, 3, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 0, 5, 0, 0, 0},
            {0, 9, 8, 0, 0, 0, 0, 6, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 8, 0, 3, 0, 0, 1},
            {0, 0, 0, 0, 2, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 2, 8, 0},
            {0, 0, 0, 0, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
    };
    //*/

    /*
    assert(LEN == 16 && BOX_LENGTH == 4);
    board_arr board = {
            {0 , 6 , 0 , 0 ,   0 , 0 , 0 , 8 ,   11, 0 , 0 , 15,   14, 0 , 0 , 16},
            {15, 11, 0 , 0 ,   0 , 16, 14, 0 ,   0 , 0 , 12, 0 ,   0 , 6 , 0 , 0 },
            {13, 0 , 9 , 12,   0 , 0 , 0 , 0 ,   3 , 16, 14, 0 ,   15, 11, 10, 0 },
            {2 , 0 , 16, 0 ,   11, 0 , 15, 10,   1 , 0 , 0 , 0 ,   0 , 0 , 0 , 0 },

            {0 , 15, 11, 10,   0 , 0 , 16, 2 ,   13, 8 , 9 , 12,   0 , 0 , 0 , 0 },
            {12, 13, 0 , 0 ,   4 , 1 , 5 , 6 ,   2 , 3 , 0 , 0 ,   0 , 0 , 11, 10},
            {5 , 0 , 6 , 1 ,   12, 0 , 9 , 0 ,   15, 11, 10, 7 ,   16, 0 , 0 , 3 },
            {0 , 2 , 0 , 0 ,   0 , 10, 0 , 11,   6 , 0 , 5 , 0 ,   0 , 13, 0 , 9 },

            {10, 7 , 15, 11,   16, 0 , 0 , 0 ,   12, 13, 0 , 0 ,   0 , 0 , 0 , 6 },
            {9 , 0 , 0 , 0 ,   0 , 0 , 1 , 0 ,   0 , 2 , 0 , 16,   10, 0 , 0 , 11},
            {1 , 0 , 4 , 6 ,   9 , 13, 0 , 0 ,   7 , 0 , 11, 0 ,   3 , 16, 0 , 0 },
            {16, 14, 0 , 0 ,   7 , 0 , 10, 15,   4 , 6 , 1 , 0 ,   0 , 0 , 13, 8 },

            {11, 10, 0 , 15,   0 , 0 , 0 , 16,   9 , 12, 13, 0 ,   0 , 1 , 5 , 4 },
            {0 , 0 , 12, 0 ,   1 , 4 , 6 , 0 ,   16, 0 , 0 , 0 ,   11, 10, 0 , 0 },
            {0 , 0 , 5 , 0 ,   8 , 12, 13, 0 ,   10, 0 , 0 , 11,   2 , 0 , 0 , 14},
            {3 , 16, 0 , 0 ,   10, 0 , 0 , 7 ,   0 , 0 , 6 , 0 ,   0 , 0 , 12, 0 },
    };
    //*/

    while (solve_easy_tiles(board)) ;
    char result[LEN * LEN * 4 * 4];
    board_pretty_str(board, result);

    solve(board);

    printf("Found %d Solution%s%s...\n",
           num_solutions,
           num_solutions ? "s" : "",
           abort_solving ? " (Max Solutions)" : "");

    for (int i = 0; i < 2; ++i) {
        printf("\n    ----  Solution [%d] ---- \n", i + 1);
        board_pretty_str(solutions[i], result);
        printf("%s\n", result);
    }

    for (int i = num_solutions - 2; i >= 2 && i < num_solutions; ++i) {
        printf("\n    ----  Solution [%d] ---- \n", i+1);
        board_pretty_str(solutions[i], result);
        printf("%s\n", result);
    }

    for (int i = 2; i < num_solutions; ++i) {
        printf("Press Enter to show another solution. Or any other key to exit.\n");
        if (getc(stdin) != '\n') break;

        printf("\n    ----  Solution [%d] ---- \n", i+1);
        board_pretty_str(solutions[i], result);
        printf("%s\n", result);
    }

    free(solutions);
    return 0;
}
