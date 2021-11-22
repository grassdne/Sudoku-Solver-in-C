#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>

#define MAX_SOLUTIONS 10000
#define LEN 9
#define BOX_LENGTH 3

typedef char small;

typedef small board_arr[LEN][LEN];

bool possible_at(board_arr board, int row, int col, small num);
bool solve_easy_tiles(board_arr board);
void board_pretty_str(board_arr board, char *result);
void solve(board_arr board);
void copy_board_to_solutions(board_arr board);
void add_link();


bool abort_solving = false;
int num_solutions;

int num_mallocs;
int num_frees;


typedef struct board_link {
    board_arr value;
    struct board_link *next;
} board_link;

board_link solutions_start;
board_link *solutions_current;


void copy_board_to_solutions(board_arr board) {
    for (int i = 0; i < LEN; ++i) {
        for (int j = 0; j < LEN; ++j) {
            solutions_current->value[i][j] = board[i][j];
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
    if (num_solutions >= MAX_SOLUTIONS) abort_solving = true;
    copy_board_to_solutions(board);
    add_link();
}

void add_link() {
    solutions_current->next = (board_link *) (board_link *) malloc(sizeof(board_link));
    if (!solutions_current->next) {
        --num_solutions;
        abort_solving = true;
        return;
    }
    ++num_mallocs;
    solutions_current = solutions_current->next;
}

void free_links() {
    board_link *link = solutions_start.next;
    board_link *temp;

    while (link != NULL) {
        temp = link->next;
        free(link);
        ++num_frees;
        link = temp;
    }
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
    solutions_current = &solutions_start;
    num_solutions = 0;
    assert(BOX_LENGTH * BOX_LENGTH == LEN);

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
            {5 , 0 , 6 ≤, 1 ,   12, 0 , 9 , 0 ,   15, 11, 10, 7 ,   16, 0 , 0 , 3 },
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
           num_solutions >= MAX_SOLUTIONS ? " (Max Solutions)" : "");

    solutions_current = &solutions_start;
    for (int i = 0; i < num_solutions && i < MAX_SOLUTIONS; ++i) {
        if (i > 3) {
            printf("Press Enter to show another solution. Or any other key to exit.\n");
            if (getc(stdin) != '\n') break;
        }
        printf("\n    ----  Solution [%d] ---- \n", i+1);
        board_pretty_str(solutions_current->value, result);
        printf("%s\n", result);
        solutions_current = solutions_current->next;
    }

    free_links();
    printf("Dynamic allocations: %d\nFreed pointers: %d\n", num_mallocs, num_frees);
    assert(num_frees == num_mallocs);

    return 0;
}
