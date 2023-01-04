#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"

// -1 not reachable
// 0 is empty tile
// 1 is nothing
// 2 is blakc pawn
// 3 white pawn
// 4 black rook
// 5 white rook
// 6 black knight
// 7 white knight
// 8 black bishop
// 9 white bishop
// 10 black queen
// 11 white queen
// 12 black king
// 13 white king

// int board[12][12] = {
//     {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // ligne -1
//     {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // ligne -1
//     {-1, -1, 0, 0, 0, 0, 12, 0, 0, 0, -1, -1},        // ligne 0
//     {-1, -1, 0, 0, 0, 0, 3, 0, 0, 0, -1, -1},         // ligne 1
//     {-1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1},         // ligne 2
//     {-1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1},         // ligne 3
//     {-1, -1, 0, 0, 0, 0, 0, 11, 0, 0, -1, -1},        // ligne 4
//     {-1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1},         // ligne 5
//     {-1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1},         // ligne 6
//     {-1, -1, 0, 0, 0, 0, 13, 0, 0, 0, -1, -1},        // ligne 7
//     {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // ligne -1
//     {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}  // ligne -1
// };

int board[12][12] = {
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // ligne -1   // ligne0
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // ligne -1   // ligne1
    {-1, -1, 4, 6, 8, 10, 12, 8, 6, 4, -1, -1},       // ligne 0    // ligne2
    {-1, -1, 2, 2, 2, 2, 2, 2, 2, 2, -1, -1},         // ligne 1    // ligne3
    {-1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1},         // ligne 2    // ligne4
    {-1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1},         // ligne 3    // ligne5
    {-1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1},         // ligne 4    // ligne6
    {-1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1},         // ligne 5    // ligne7
    {-1, -1, 3, 3, 3, 3, 3, 3, 3, 3, -1, -1},         // ligne 6    // ligne8
    {-1, -1, 5, 7, 9, 11, 13, 9, 7, 5, -1, -1},       // ligne 7    // ligne9
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // ligne -1   // ligne10
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}  // ligne -1   // ligne11

};

int playable_moves[12][12];

bool white_to_play = true;

bool black_king_has_moved = false;
bool white_king_has_moved = false;
bool black_rook_has_moved_king_side = false;
bool white_rook_has_moved_king_side = false;
bool black_rook_has_moved_queen_side = false;
bool white_rook_has_moved_queen_side = false;

void print_piece(int i, int j)
{
    switch (board[i][j])
    {
    case 2:
        printf("p |");
        break;
    case 3:
        printf("P |");
        break;
    case 4:
        printf("r |");
        break;
    case 5:
        printf("R |");
        break;
    case 6:
        printf("n |");
        break;
    case 7:
        printf("N |");
        break;
    case 8:
        printf("b |");
        break;
    case 9:
        printf("B |");
        break;
    case 10:
        printf("q |");
        break;
    case 11:
        printf("Q |");
        break;
    case 12:
        printf("k |");
        break;
    case 13:
        printf("K |");
        break;
    case 0:
        printf("  |");
        break;

    default:
        break;
    }
}

void print_board()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            print_piece(i, j);
        }
        printf("\n");
        printf("------------------------\n");
    }
}

void next_move_black_pawn(int line, int column)
{

    if (line == 3 && board[line + 1][column] == 0 && board[line + 2][column] == 0)
    {
        playable_moves[line + 1][column] = 1;
        playable_moves[line + 2][column] = 1;
    }
    else if (line == 3 && board[line + 1][column] == 0 && board[line + 2][column] != 0)
    {
        playable_moves[line + 1][column] = 1;
    }
    else if (line > 3 && line != 9 && board[line + 1][column] == 0)
    {
        playable_moves[line + 1][column] = 1;
    }
    if (board[line + 1][column - 1] % 2 == 1)
    {
        playable_moves[line + 1][column - 1] = 2;
    }
    if (board[line + 1][column + 1] % 2 == 1)
    {
        playable_moves[line + 1][column + 1] = 2;
    }
}

void next_move_white_pawn(int line, int column)
{
    if (line == 8 && board[line - 1][column] == 0 && board[line - 2][column] == 0)
    {
        playable_moves[line - 1][column] = 1;
        playable_moves[line - 2][column] = 1;
    }
    else if (line == 8 && board[line - 1][column] == 0 && board[line - 2][column] != 0)
    {
        playable_moves[line - 1][column] = 1;
    }
    else if (line < 8 && line != 2 && board[line - 1][column] == 0)
    {
        playable_moves[line - 1][column] = 1;
    }
    if (board[line - 1][column - 1] % 2 == 0 && board[line - 1][column - 1] > 0)
    {
        playable_moves[line - 1][column - 1] = 2;
    }
    if (board[line - 1][column + 1] % 2 == 0 && board[line - 1][column + 1] > 0)
    {
        playable_moves[line - 1][column + 1] = 2;
    }
}

void next_move_black_rook(int line, int column)
{
    int tmp_line = line;
    int tmp_column = column;

    // right

    while (board[line][column + 1] == 0)
    {
        playable_moves[line][column + 1] = 1;
        column++;
    }
    // case found is an opposite piece
    if (board[line][column + 1] % 2 == 1)
    {
        playable_moves[line][column + 1] = 2;
    }

    // left
    line = tmp_line;
    column = tmp_column;
    while (board[line][column - 1] == 0)
    {
        playable_moves[line][column - 1] = 1;
        column--;
    }
    // case found is an opposite piece
    if (board[line][column - 1] % 2 == 1)
    {
        playable_moves[column][column - 1] = 2;
    }

    // up
    line = tmp_line;
    column = tmp_column;
    while (board[line - 1][column] == 0)
    {
        playable_moves[line - 1][column] = 1;
        line--;
    }
    // case found is an opposite piece
    if (board[line - 1][column] % 2 == 1)
    {
        playable_moves[line - 1][column] = 2;
    }

    // down
    line = tmp_line;
    column = tmp_column;
    while (board[line + 1][column] == 0)
    {
        playable_moves[line + 1][column] = 1;
        line++;
    }
    // case found is an opposite piece
    if (board[line + 1][column] % 2 == 1)
    {
        playable_moves[line + 1][column] = 2;
    }
}

void next_move_white_rook(int line, int column)
{
    int tmp_line = line;
    int tmp_column = column;

    // right
    while (board[line][column + 1] == 0)
    {
        playable_moves[line][column + 1] = 1;

        column++;
    }
    // case found is an opposite piece
    if (board[line][column + 1] % 2 == 0 && board[line][column + 1] > 0)
    {
        playable_moves[line][column + 1] = 2;
    }

    // left
    line = tmp_line;
    column = tmp_column;
    while (board[line][column - 1] == 0)
    {
        playable_moves[line][column - 1] = 1;
        column--;
    }
    // case found is an opposite piece
    if (board[line][column - 1] % 2 == 0 && board[line][column - 1] > 0)
    {
        playable_moves[column][column - 1] = 2;
    }

    // up
    line = tmp_line;
    column = tmp_column;
    while (board[line - 1][column] == 0)
    {
        playable_moves[line - 1][column] = 1;
        line--;
    }
    // case found is an opposite piece
    if (board[line - 1][column] % 2 == 0 && board[line - 1][column] > 0)
    {
        playable_moves[line - 1][column] = 2;
    }

    // down
    line = tmp_line;
    column = tmp_column;
    while (board[line + 1][column] == 0)
    {
        playable_moves[line + 1][column] = 1;
        line++;
    }
    // case found is an opposite piece
    if (board[line + 1][column] % 2 == 0 && board[line + 1][column] > 0)
    {
        playable_moves[line + 1][column] = 2;
    }
}

void next_move_black_bishop(int line, int column)
{
    int tmp_line = line;
    int tmp_column = column;

    // up & right
    while (board[line - 1][column + 1] == 0)
    {
        playable_moves[line - 1][column + 1] = 1;
        line--;
        column++;
    }
    if (board[line - 1][column + 1] % 2 == 1)
    {
        playable_moves[line - 1][column + 1] = 2;
    }
    // up & left
    line = tmp_line;
    column = tmp_column;
    while (board[line - 1][column - 1] == 0)
    {
        playable_moves[line - 1][column - 1] = 1;
        line--;
        column--;
    }
    if (board[line - 1][column - 1] % 2 == 1)
    {
        playable_moves[line - 1][column - 1] = 2;
    }
    // down & right
    line = tmp_line;
    column = tmp_column;
    while (board[line + 1][column + 1] == 0)
    {
        playable_moves[line + 1][column + 1] = 1;
        line++;
        column++;
    }
    if (board[line + 1][column + 1] % 2 == 1)
    {
        playable_moves[line + 1][column + 1] = 2;
    }
    // down and left
    line = tmp_line;
    column = tmp_column;
    while (board[line + 1][column - 1] == 0)
    {
        playable_moves[line + 1][column - 1] = 1;
        line++;
        column--;
    }
    if (board[line + 1][column - 1] % 2 == 1)
    {
        playable_moves[line + 1][column - 1] = 2;
    }
}

void next_move_white_bishop(int line, int column)
{
    int tmp_line = line;
    int tmp_column = column;

    // up & right
    while (board[line - 1][column + 1] == 0)
    {
        playable_moves[line - 1][column + 1] = 1;
        line--;
        column++;
    }
    if (board[line - 1][column + 1] % 2 == 0 && board[line - 1][column + 1] > 0)
    {
        playable_moves[line - 1][column + 1] = 2;
    }
    // up & left
    line = tmp_line;
    column = tmp_column;
    while (board[line - 1][column - 1] == 0)
    {
        playable_moves[line - 1][column - 1] = 1;
        line--;
        column--;
    }
    if (board[line - 1][column - 1] % 2 == 0 && board[line - 1][column - 1] > 0)
    {
        playable_moves[line - 1][column - 1] = 2;
    }
    // down & right
    line = tmp_line;
    column = tmp_column;
    while (board[line + 1][column + 1] == 0)
    {
        playable_moves[line + 1][column + 1] = 1;
        line++;
        column++;
    }
    if (board[line + 1][column + 1] % 2 == 0 && board[line + 1][column + 1] > 0)
    {
        playable_moves[line + 1][column + 1] = 2;
    }
    // down and left
    line = tmp_line;
    column = tmp_column;
    while (board[line + 1][column - 1] == 0)
    {
        playable_moves[line + 1][column - 1] = 1;
        line++;
        column--;
    }
    if (board[line + 1][column - 1] % 2 == 0 && board[line + 1][column - 1] > 0)
    {
        playable_moves[line + 1][column - 1] = 2;
    }
}

void next_move_black_knight(int line, int column)
{

    // 1, starting up right
    if (board[line - 2][column + 1] == 0)
    {
        playable_moves[line - 2][column + 1] = 1;
    }
    if (board[line - 2][column + 1] % 2 == 1)
    {
        playable_moves[line - 2][column + 1] = 2;
    }
    // 2
    if (board[line - 1][column + 2] == 0)
    {
        playable_moves[line - 1][column + 2] = 1;
    }
    if (board[line - 1][column + 2] % 2 == 1)
    {
        playable_moves[line - 1][column + 2] = 2;
    }
    // 3
    if (board[line + 1][column + 2] == 0)
    {
        playable_moves[line + 1][column + 2] = 1;
    }
    if (board[line + 1][column + 2] % 2 == 1)
    {
        playable_moves[line + 1][column + 2] = 2;
    }
    // 4
    if (board[line + 2][column + 1] == 0)
    {
        playable_moves[line + 2][column + 1] = 1;
    }
    if (board[line + 2][column + 1] % 2 == 1)
    {
        playable_moves[line + 2][column + 1] = 2;
    }
    // 5

    if (board[line + 2][column - 1] == 0)
    {
        playable_moves[line + 2][column - 1] = 1;
    }
    if (board[line + 2][column - 1] % 2 == 1)
    {
        playable_moves[line + 2][column - 1] = 2;
    }
    // 6
    if (board[line + 1][column - 2] == 0)
    {
        playable_moves[line + 1][column - 2] = 1;
    }
    if (board[line + 1][column - 2] % 2 == 1)
    {
        playable_moves[line + 1][column - 2] = 2;
    }

    // 7
    if (board[line - 1][column - 2] == 0)
    {
        playable_moves[line - 1][column - 2] = 1;
    }
    if (board[line - 1][column - 2] % 2 == 1)
    {
        playable_moves[line - 1][column - 2] = 2;
    }
    // 8
    if (board[line - 2][column - 1] == 0)
    {
        playable_moves[line - 2][column - 1] = 1;
    }
    if (board[line - 2][column - 1] % 2 == 1)
    {
        playable_moves[line - 2][column - 1] = 2;
    }
}

void next_move_white_knight(int line, int column)
{

    // 1, starting up right
    if (board[line - 2][column + 1] == 0)
    {
        playable_moves[line - 2][column + 1] = 1;
    }
    if (board[line - 2][column + 1] % 2 == 0 && board[line - 2][column + 1] > 0)
    {
        playable_moves[line - 2][column + 1] = 2;
    }
    // 2
    if (board[line - 1][column + 2] == 0)
    {
        playable_moves[line - 1][column + 2] = 1;
    }
    if (board[line - 1][column + 2] % 2 == 0 && board[line - 1][column + 2] > 0)
    {
        playable_moves[line - 1][column + 2] = 2;
    }
    // 3
    if (board[line + 1][column + 2] == 0)
    {
        playable_moves[line + 1][column + 2] = 1;
    }
    if (board[line + 1][column + 2] % 2 == 0 && board[line + 1][column + 2] > 0)
    {
        playable_moves[line + 1][column + 2] = 2;
    }
    // 4
    if (board[line + 2][column + 1] == 0)
    {
        playable_moves[line + 2][column + 1] = 1;
    }
    if (board[line + 2][column + 1] % 2 == 0 && board[line + 2][column + 1] > 0)
    {
        playable_moves[line + 2][column + 1] = 2;
    }
    // 5

    if (board[line + 2][column - 1] == 0)
    {
        playable_moves[line + 2][column - 1] = 1;
    }
    if (board[line + 2][column - 1] % 2 == 0 && board[line + 2][column - 1] > 0)
    {
        playable_moves[line + 2][column - 1] = 2;
    }
    // 6
    if (board[line + 1][column - 2] == 0)
    {
        playable_moves[line + 1][column - 2] = 1;
    }
    if (board[line + 1][column - 2] % 2 == 0 && board[line + 1][column - 2] > 0)
    {
        playable_moves[line + 1][column - 2] = 2;
    }

    // 7
    if (board[line - 1][column - 2] == 0)
    {
        playable_moves[line - 1][column - 2] = 1;
    }
    if (board[line - 1][column - 2] % 2 == 0 && board[line - 1][column - 2] > 0)
    {
        playable_moves[line - 1][column - 2] = 2;
    }
    // 8
    if (board[line - 2][column - 1] == 0)
    {
        playable_moves[line - 2][column - 1] = 1;
    }
    if (board[line - 2][column - 1] % 2 == 0 && board[line - 2][column - 1] > 0)
    {

        playable_moves[line - 2][column - 1] = 2;
    }
}

void next_move_black_king(int line, int column)
{
    if (board[line + 1][column] == 0)
    {
        playable_moves[line + 1][column] = 1;
    }
    else if (board[line + 1][column] % 2 == 1)
    {
        playable_moves[line + 1][column] = 2;
    }
    if (board[line - 1][column] == 0)
    {
        playable_moves[line - 1][column] = 1;
    }
    else if (board[line - 1][column + 1] % 2 == 1)
    {
        playable_moves[line - 1][column + 1] = 2;
    }
    if (board[line][column + 1] == 0)
    {
        playable_moves[line][column + 1] = 1;
    }
    else if (board[line][column + 1] % 2 == 1)
    {
        playable_moves[line][column + 1] = 2;
    }
    if (board[line][column - 1] == 0)
    {
        playable_moves[line][column - 1] = 1;
    }
    else if (board[line][column - 1] % 2 == 1)
    {
        playable_moves[line][column - 1] = 2;
    }

    if (board[line - 1][column - 1] == 0)
    {
        playable_moves[line - 1][column - 1] = 1;
    }
    else if (board[line - 1][column - 1] % 2 == 1)
    {
        playable_moves[line - 1][column - 1] = 2;
    }
    if (board[line - 1][column + 1] == 0)
    {
        playable_moves[line - 1][column + 1] = 1;
    }
    else if (board[line - 1][column + 1] % 2 == 1)
    {
        playable_moves[line - 1][column + 1] = 2;
    }

    if (board[line + 1][column - 1] == 0)
    {
        playable_moves[line + 1][column - 1] = 1;
    }
    else if (board[line + 1][column - 1] % 2 == 1)
    {
        playable_moves[line + 1][column - 1] = 2;
    }
    if (board[line + 1][column + 1] == 0)
    {
        playable_moves[line + 1][column + 1] = 1;
    }
    else if (board[line + 1][column + 1] % 2 == 1)
    {
        playable_moves[line + 1][column + 1] = 2;
    }
}

void next_move_white_king(int line, int column)
{
    if (board[line + 1][column] == 0)
    {
        playable_moves[line + 1][column] = 1;
    }
    else if (board[line + 1][column] % 2 == 0 && board[line + 1][column] > 0)
    {
        playable_moves[line + 1][column] = 2;
    }
    if (board[line - 1][column] == 0)
    {
        playable_moves[line - 1][column] = 1;
    }
    else if (board[line - 1][column] % 2 == 0 && board[line - 1][column] > 0)
    {
        playable_moves[line - 1][column] = 2;
    }

    else if (board[line - 1][column + 1] % 2 == 0 && board[line - 1][column + 1] > 0)
    {
        playable_moves[line - 1][column + 1] = 2;
    }
    if (board[line][column + 1] == 0)
    {
        playable_moves[line][column + 1] = 1;
    }
    else if (board[line][column + 1] % 2 == 0 && board[line][column + 1] > 0)
    {
        playable_moves[line][column + 1] = 2;
    }
    if (board[line][column - 1] == 0)
    {
        playable_moves[line][column - 1] = 1;
    }
    else if (board[line][column - 1] % 2 == 0 && board[line][column - 1] > 0)
    {
        playable_moves[line][column - 1] = 2;
    }

    if (board[line - 1][column - 1] == 0)
    {
        playable_moves[line - 1][column - 1] = 1;
    }
    else if (board[line - 1][column - 1] % 2 == 0 && board[line - 1][column - 1] > 0)
    {
        playable_moves[line - 1][column - 1] = 2;
    }
    if (board[line - 1][column + 1] == 0)
    {
        playable_moves[line - 1][column + 1] = 1;
    }
    else if (board[line - 1][column + 1] % 2 == 0 && board[line - 1][column + 1] > 0)
    {
        playable_moves[line - 1][column + 1] = 2;
    }

    if (board[line + 1][column - 1] == 0)
    {
        playable_moves[line + 1][column - 1] = 1;
    }
    else if (board[line + 1][column - 1] % 2 == 0 && board[line + 1][column - 1] > 0)
    {
        playable_moves[line + 1][column - 1] = 2;
    }
    if (board[line + 1][column + 1] == 0)
    {
        playable_moves[line + 1][column + 1] = 1;
    }
    else if (board[line + 1][column + 1] % 2 == 0 && board[line + 1][column + 1] > 0)
    {
        playable_moves[line + 1][column + 1] = 2;
    }
}

void next_move(int line, int column)
{

    line = line + 2;
    column = column + 2;

    // now, first line up = 2
    // now, black pawns line = 3
    // now, white pawns line = 8
    // now, last line = 9

    memset(playable_moves, 0, 144 * sizeof(int));
    // displaying current piece
    playable_moves[line][column] = 8;

    switch (board[line][column])
    {

    // black pawn
    case 2:
        next_move_black_pawn(line, column);
        break;

    // white pawn
    case 3:
        next_move_white_pawn(line, column);
        break;

    // black rook
    case 4:
        next_move_black_rook(line, column);
        break;

    // white rook
    case 5:
        next_move_white_rook(line, column);
        break;

    // black knight
    case 6:
        next_move_black_knight(line, column);
        break;

    // white knigth
    case 7:
        next_move_white_knight(line, column);
        break;

    // black bishop
    case 8:
        next_move_black_bishop(line, column);
        break;

    // white bishop
    case 9:
        next_move_white_bishop(line, column);
        break;

    // black queen
    case 10:
        next_move_black_bishop(line, column);
        next_move_black_rook(line, column);
        break;

    // white queen
    case 11:
        next_move_white_bishop(line, column);
        next_move_white_rook(line, column);
        break;

    // black king
    case 12:
        next_move_black_king(line, column);
        break;

    case 13:
        next_move_white_king(line, column);
        break;

    default:
        break;
    }
}

void print_next_move()
{
    for (int i = 2; i < 10; i++)
    {
        for (int j = 2; j < 10; j++)
        {
            printf("%d ", playable_moves[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

int *next_move_char_to_num(char *params)
{

    static int result[4] = {0, 0, 0, 0};
    char d[] = ",";
    char e[] = " ";

    char *from = strtok(params, d);
    char *to = strtok(NULL, e);
    // printf("pos1 = %s, pos2 = %s\n", from, to);

    int line_from, column_from;
    int line_to, column_to;

    switch ((char)from[0])
    {
    case 'a':
        column_from = 0;
        break;

    case 'b':
        column_from = 1;
        break;

    case 'c':
        column_from = 2;
        break;

    case 'd':
        column_from = 3;
        break;

    case 'e':
        column_from = 4;
        break;

    case 'f':
        column_from = 5;
        break;

    case 'g':
        column_from = 6;
        break;

    case 'h':
        column_from = 7;
        break;

    default:
        break;
    }

    switch ((char)from[1])
    {
    case '1':
        line_from = 7;
        break;

    case '2':
        line_from = 6;
        break;

    case '3':
        line_from = 5;
        break;

    case '4':
        line_from = 4;
        break;

    case '5':
        line_from = 3;
        break;

    case '6':
        line_from = 2;
        break;

    case '7':
        line_from = 1;
        break;

    case '8':
        line_from = 0;
        break;

    default:
        break;
    }

    switch ((char)to[0])
    {
    case 'a':
        column_to = 0;
        break;

    case 'b':
        column_to = 1;
        break;

    case 'c':
        column_to = 2;
        break;

    case 'd':
        column_to = 3;
        break;

    case 'e':
        column_to = 4;
        break;

    case 'f':
        column_to = 5;
        break;

    case 'g':
        column_to = 6;
        break;

    case 'h':
        column_to = 7;
        break;

    default:
        break;
    }

    switch ((char)to[1])
    {
    case '1':
        line_to = 7;
        break;

    case '2':
        line_to = 6;
        break;

    case '3':
        line_to = 5;
        break;

    case '4':
        line_to = 4;
        break;

    case '5':
        line_to = 3;
        break;

    case '6':
        line_to = 2;
        break;

    case '7':
        line_to = 1;
        break;

    case '8':
        line_to = 0;
        break;

    default:
        break;
    }

    printf("line from = %d column from = %d line to = %d column to = %d \n", line_from, column_from, line_to, column_to);
    result[0] = line_from;
    result[1] = column_from;
    result[2] = line_to;
    result[3] = column_to;
    return result;
}

// check wether the player's king is in check once the move is done
// returns true if the move is legal ie the king is not in check after the move is done
bool is_legal_check(int line_to, int col_to)
{

    int adv_king_pos[2] = {0, 0};
    int i = 2, j = 2;

    if (white_to_play)
    {
        printf("white\n");
        // checking where the opposite king is
        while (board[i][j] != 13)
        {
            j++;
            if (j % 10 == 0)
            {
                j = 2;
                i++;
            }
        }
        adv_king_pos[0] = i;
        adv_king_pos[1] = j;
        printf("\n\n");

        for (int l = 2; l < 10; l++)
        {
            for (int c = 2; c < 10; c++)
            {
                // check adverse pieces
                if (board[l][c] % 2 == 0 && board[l][c] != 0)
                {
                    next_move(l - 2, c - 2);
                    // check if king is in ennemy's scope
                    if (playable_moves[adv_king_pos[0]][adv_king_pos[1]] == 2)
                    {
                        printf("king is in ennemy scope, %d\n", playable_moves[adv_king_pos[0]][adv_king_pos[1]]);
                        return false;
                    }
                }
            }
        }
        return true;
    }
    else
    {
        printf("black\n");
        // checking where the opposite king is
        while (board[i][j] != 12)
        {
            j++;
            if (j % 10 == 0)
            {
                j = 2;
                i++;
            }
        }
        adv_king_pos[0] = i;
        adv_king_pos[1] = j;
        printf("\n\n");

        for (int l = 2; l < 10; l++)
        {
            for (int c = 2; c < 10; c++)
            {
                // check adverse pieces
                if (board[l][c] % 2 == 1 && board[l][c] != -1)
                {
                    printf("piece is : \n");
                    print_piece(l, c);
                    printf("\n\n");
                    next_move(l - 2, c - 2);
                    print_next_move();
                    printf("playable move : %d\n", playable_moves[adv_king_pos[0]][adv_king_pos[1]]);
                    printf("\n\n");
                    // check if king is in ennemy's scope

                    if (playable_moves[adv_king_pos[0]][adv_king_pos[1]] == 2)
                    {
                        printf("king is in ennemy scope, %d\n", playable_moves[adv_king_pos[0]][adv_king_pos[1]]);
                        return false;
                    }
                }
            }
        }
        return true;
    }
}

bool path_is_clear_between_r_and_k(int *pos)
{
    // castle queen side (POS[3] = 1, pos[1] = 5)
    if ((pos[1] - pos[3]) == 4)
    {
        if (white_to_play)
        {
            for (int i = pos[3] + 1; i < pos[1]; i++)
            {

                // first line of white is line 9 of board
                if (board[9][i + 2] != 0)
                {
                    return false;
                }
            }
        }
        // if black to play
        else
        {
            for (int i = pos[3] + 1; i < pos[1]; i++)
            {
                // first line of black is line 2 of board
                if (board[2][i + 2] != 0)
                {
                    return false;
                }
            }
        }
        return true;
    }
    // castle king side
    if ((pos[3] - pos[1]) == 3)
    {
        if (white_to_play)
        {
            for (int i = pos[1] + 1; i < pos[3]; i++)
            {
                // first line of white is line 9 of board
                if (board[9][i + 2] != 0)
                {
                    return false;
                }
            }
        }
        else
        {
            for (int i = pos[1] + 1; i < pos[3]; i++)
            {
                // first line of black is line 2 of board
                if (board[2][i + 2] != 0)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

bool no_checks_on_the_path(int *pos)
{

    // king side
    if ((pos[3] - pos[1]) == 3)
    {
        if (white_to_play)
        {
            for (int i = 2; i < 10; i++)
            {
                for (int j = 2; j < 10; j++)
                {
                    if (board[i][j] % 2 == 0 && board[i][j] != 0)
                    {
                        next_move(i - 2, j - 2);

                        // king side for white
                        for (int k = pos[1]; k < pos[3]; k++)
                        {
                            // first line for whites is 2nd line for playable_moves
                            if (playable_moves[9][k + 2] == 1 || playable_moves[9][k + 2] == 2)
                            {
                                return false;
                            }
                        }
                    }
                }
            }
            return true;
        }
        // black to play
        else
        {
            for (int i = 2; i < 10; i++)
            {
                for (int j = 2; j < 10; j++)
                {
                    if (board[i][j] % 2 == 1)
                    {
                        next_move(i - 2, j - 2);

                        // king side for black
                        for (int k = pos[1]; k < pos[3]; k++)
                        {
                            // first line for blacks is 2nd line for playable_moves
                            if (playable_moves[2][k + 2] == 1 || playable_moves[2][k + 2] == 2)
                            {
                                return false;
                            }
                        }
                    }
                }
            }
            return true;
        }
    }
    // queen side
    else if ((pos[1] - pos[3]) == 4)
    {

        if (white_to_play)
        {
            for (int i = 2; i < 10; i++)
            {
                for (int j = 2; j < 10; j++)
                {
                    if (board[i][j] % 2 == 0 && board[i][j] != 0)
                    {
                        next_move(i - 2, j - 2);

                        // queen side for white
                        // only two cases next to  king need to be checked
                        for (int k = pos[3] + 2; k < pos[1] + 1; k++)
                        {
                            // first line for whites is 2nd line for playable_moves
                            if (playable_moves[9][k + 2] == 1 || playable_moves[9][k + 2] == 2)
                            {
                                return false;
                            }
                        }
                    }
                }
            }
            return true;
        }
        // black to play
        else
        {
            for (int i = 2; i < 10; i++)
            {
                for (int j = 2; j < 10; j++)
                {
                    if (board[i][j] % 2 == 1)
                    {
                        next_move(i - 2, j - 2);

                        // king side for black
                        for (int k = pos[3] + 2; k < pos[1] + 1; k++)
                        {
                            // first line for blacks is 2nd line for playable_moves
                            if (playable_moves[2][k + 2] == 1 || playable_moves[2][k + 2] == 2)
                            {
                                return false;
                            }
                        }
                    }
                }
            }
        }
        return true;
    }
    return false;
}

bool move_is_castling(int *pos)
{
    return ((board[pos[0] + 2][pos[1] + 2] == 13 && board[pos[2] + 2][pos[3] + 2] == 5) || (board[pos[0] + 2][pos[1] + 2] == 12 && board[pos[2] + 2][pos[3] + 2] == 4));
}

bool move_is_castling_and_is_legal(int *pos)
{
    // check wether we're actually taking king and rook as targets
    // check wether we're at the first line
    if (pos[0] == 0 || pos[0] == 7)
    {
        // check wether the path is clear between king and rook
        if (path_is_clear_between_r_and_k(pos))
        {
            // check wether there's no check on the path between rook and king
            if (no_checks_on_the_path(pos))
            {
                // queen side
                if ((pos[1] - pos[3]) == 4)
                {
                    if (white_to_play)
                    {
                        // these booleans are set by default at false
                        // if they're both false, !false && !false == true && true == true
                        // otherwise, it fails
                        return !white_king_has_moved && !white_rook_has_moved_queen_side;
                    }
                    else
                        return !black_king_has_moved && !black_rook_has_moved_queen_side;
                }
                else
                {
                    if (white_to_play)
                    {
                        return !white_king_has_moved && !white_rook_has_moved_king_side;
                    }
                    else
                    {
                        return !black_king_has_moved && !black_rook_has_moved_king_side;
                    }
                }
            }
            else
                return false;
        }
        else
            return false;
    }
    else
        return false;
}

bool check_color(int line, int col)
{
    return ((board[line + 2][col + 2] % 2 == 1 && white_to_play) || (board[line + 2][col + 2] % 2 == 0 && !white_to_play));
}

void store_if_rook_or_king_has_moved(int *pos)
{
    // white
    // queen side
    if (board[pos[2] + 2][pos[3] + 2] == 5 && pos[1] == 0)
        white_rook_has_moved_queen_side = true;
    // king side
    if (board[pos[2] + 2][pos[3] + 2] == 5 && pos[1] == 7)
        white_rook_has_moved_king_side = true;

    if (board[pos[2] + 2][pos[3] + 2] == 13)
        white_king_has_moved = true;

    // black

    // queen side
    if (board[pos[2] + 2][pos[3] + 2] == 4 && pos[1] == 0)
        black_rook_has_moved_queen_side = true;
    // king side
    if (board[pos[2] + 2][pos[3] + 2] == 4 && pos[1] == 7)
        black_rook_has_moved_king_side = true;

    if (board[pos[2] + 2][pos[3] + 2] == 12)
        black_king_has_moved = true;

    // printf("w-qs:%d w-ks:%d, w-k:%d, b-qs:%d, b-ks:%d, b-k:%d\n", white_rook_has_moved_queen_side ? 1 : 0, white_rook_has_moved_king_side ? 1 : 0, white_king_has_moved ? 1 : 0, black_rook_has_moved_queen_side ? 1 : 0, black_rook_has_moved_king_side ? 1 : 0, black_king_has_moved ? 1 : 0);
}

bool move_is_promotion(int *pos)
{
    if (white_to_play)
    {
        if (pos[2] == 0 && board[pos[0] + 2][pos[1] + 2] == 3 && (board[pos[2] + 2][pos[3] + 2] == 0 || (board[pos[2] + 2][pos[3] + 2] % 2 == 0 && board[pos[2] + 2][pos[3] + 2] != 0)))
            return true;
        return false;
    }
    else
    {
        if (pos[2] == 7 && board[pos[0] + 2][pos[1] + 2] == 2 && (board[pos[2] + 2][pos[3] + 2] == 0 || (board[pos[2] + 2][pos[3] + 2] % 2 == 1)))
            return true;
        return false;
    }
}

bool check_if_checkmate(bool turn)
{
    int tmp_1, tmp_2;

    int index = turn ? 1 : 0;

    for (int i = 2; i < 10; i++)
    {
        for (int j = 2; j < 10; j++)
        {
            if (board[i][j] % 2 == index && board[i][j] != 0)
            {
                next_move(i, j);
                for (int k = 2; k < 10; k++)
                {
                    for (int l = 2; l < 10; l++)
                    {
                        print_next_move();

                        if (playable_moves[k][l] == 1 || playable_moves[k][l] == 2)
                        {
                            printf("hey\n");
                            tmp_1 = board[k][l];
                            tmp_2 = board[i][j];
                            board[k][l] = board[i][j];
                            board[i][j] = 0;
                            if (is_legal_check(k, l))
                            {
                                // no checkmate
                                return false;
                            }
                        }
                        board[i][j] = tmp_2;
                        board[k][l] = tmp_1;
                    }
                }
            }
        }
    }
    return true;
}

void play_next_move_local()
{
    int *pos;
    char *params = malloc(4 * sizeof(char));

    // saving temp position when checking if king is in check
    int tmp_1, tmp_2;

    while (1)
    {
        print_board();
        // if (check_if_checkmate(white_to_play))
        // {
        //     printf("%s has won by checkmate !\n", white_to_play ? "White" : "Black");
        //     exit(1);
        // };

    choix:
        // choosing what to play
        printf("%s", white_to_play ? "Please enter White's move: (format: <a3,g6>)\n" : " Please enter Black's move: (format: <a3,g6>)\n");
        scanf("%s", params);
        // receiving positions
        pos = next_move_char_to_num(params);
        // calculating all moves for the chosen piece
        next_move(pos[0], pos[1]);
        if (check_color(pos[0], pos[1]))
        {
            // castle move part

            if (move_is_castling(pos))
            {
                if (move_is_castling_and_is_legal(pos))
                {
                    if (white_to_play)
                    {
                        // queen side
                        if ((pos[1] - pos[3]) == 4)
                        {
                            board[pos[0] + 2][pos[1] + 2] = 0;
                            board[pos[2] + 2][pos[3] + 2] = 0;
                            board[pos[0] + 2][pos[1]] = 13;
                            board[pos[0] + 2][pos[1] + 1] = 5;
                        }
                        else
                        {
                            board[pos[0] + 2][pos[1] + 2] = 0;
                            board[pos[2] + 2][pos[3] + 2] = 0;
                            board[pos[0] + 2][pos[1] + 4] = 13;
                            board[pos[0] + 2][pos[1] + 3] = 5;
                        }
                    }
                    else
                    {
                        // queen side
                        if ((pos[1] - pos[3]) == 4)
                        {
                            board[pos[0] + 2][pos[1] + 2] = 0;
                            board[pos[2] + 2][pos[3] + 2] = 0;
                            board[pos[0] + 2][pos[1]] = 12;
                            board[pos[0] + 2][pos[1] + 1] = 4;
                        }
                        else
                        {
                            board[pos[0] + 2][pos[1] + 2] = 0;
                            board[pos[2] + 2][pos[3] + 2] = 0;
                            board[pos[0] + 2][pos[1] + 4] = 12;
                            board[pos[0] + 2][pos[1] + 3] = 4;
                        }
                    }
                    goto legal;
                }
                else
                {
                    goto not_legal;
                }
            }
            else
            {
                goto other_move;
            }

        // other moves part
        other_move:
            if ((playable_moves[pos[2] + 2][pos[3] + 2] == 1 || playable_moves[pos[2] + 2][pos[3] + 2] == 2))
            {
                system("clear");
                if (move_is_promotion(pos))
                {
                    tmp_1 = board[pos[2] + 2][pos[3] + 2];
                    tmp_2 = board[pos[0] + 2][pos[1] + 2];
                    if (white_to_play)
                    {

                        board[pos[2] + 2][pos[3] + 2] = 11;
                        board[pos[0] + 2][pos[1] + 2] = 0;

                        if (!is_legal_check(pos[2], pos[3]))
                        {
                            // entering this if means that the move wasnt legal
                            // we reset the positions since it is not legal
                            board[pos[2] + 2][pos[3] + 2] = tmp_1;
                            board[pos[0] + 2][pos[1] + 2] = tmp_2;
                            goto not_legal;
                        }
                    }
                    else
                    {

                        board[pos[2] + 2][pos[3] + 2] = 10;
                        board[pos[0] + 2][pos[1] + 2] = 0;

                        if (!is_legal_check(pos[2], pos[3]))
                        {
                            // entering this if means that the move wasnt legal
                            // we reset the positions since it is not legal
                            board[pos[2] + 2][pos[3] + 2] = tmp_1;
                            board[pos[0] + 2][pos[1] + 2] = tmp_2;
                            goto not_legal;
                        }
                    }
                    goto legal;
                }

                tmp_1 = board[pos[2] + 2][pos[3] + 2];
                tmp_2 = board[pos[0] + 2][pos[1] + 2];
                board[pos[2] + 2][pos[3] + 2] = board[pos[0] + 2][pos[1] + 2];
                board[pos[0] + 2][pos[1] + 2] = 0;
                // here we check if this new position is okay, ie it doesnt let or put or king in check;
                if (!is_legal_check(pos[2], pos[3]))
                {
                    // entering this if means that the move wasnt legal
                    // we reset the positions since it is not legal
                    board[pos[2] + 2][pos[3] + 2] = tmp_1;
                    board[pos[0] + 2][pos[1] + 2] = tmp_2;
                    goto not_legal;
                }
                // legal part

                // here we check if either any rook or king has moved
                store_if_rook_or_king_has_moved(pos);
                // we switch to the other color since the move has been accepted
            legal:
                white_to_play = !white_to_play;
                print_board();
            }
            else
            {
            not_legal:
                system("clear");
                print_board();
                printf("Not a legal move, try again\n");
                goto choix;
            }
        }
        else
        {
            // entering here means that either color has played twice in a row
            system("clear");
            print_board();
            printf("%s\n", white_to_play ? "Sorry, White to play\n" : "Sorry, Black to play\n");
            goto choix;
        }
    }
}

int main()
{
    play_next_move_local();
}