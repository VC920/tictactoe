#include <raylib.h>

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 900

#define BOARD_ROW 3
#define BOARD_COL 3
#define BOARD_CELL_SIZE (SCREEN_WIDTH / BOARD_ROW)

typedef enum {
    GAME_RUN,
    GAME_WIN,
} GameState;

typedef enum {
    PLAYER_1,
    PLAYER_2,
    PLAYER_NONE
} Player;

void check_win(GameState *state, char board[BOARD_ROW][BOARD_COL], Player *winner)
{
    if ((board[0][0] == 'X' && board[0][1] == 'X' && board[0][2] == 'X') ||
        (board[1][0] == 'X' && board[1][1] == 'X' && board[1][2] == 'X') ||
        (board[2][0] == 'X' && board[2][1] == 'X' && board[2][2] == 'X') ||

        (board[0][0] == 'X' && board[1][0] == 'X' && board[2][0] == 'X') ||
        (board[0][1] == 'X' && board[1][1] == 'X' && board[2][1] == 'X') ||
        (board[0][2] == 'X' && board[1][2] == 'X' && board[2][2] == 'X') ||

        (board[0][0] == 'X' && board[1][1] == 'X' && board[2][2] == 'X') ||
        (board[0][2] == 'X' && board[1][1] == 'X' && board[2][0] == 'X')
    ) {
        *state = GAME_WIN;
        *winner = PLAYER_1;
    }

    else if ((board[0][0] == 'O' && board[0][1] == 'O' && board[0][2] == 'O') ||
             (board[1][0] == 'O' && board[1][1] == 'O' && board[1][2] == 'O') ||
             (board[2][0] == 'O' && board[2][1] == 'O' && board[2][2] == 'O') ||

             (board[0][0] == 'O' && board[1][0] == 'O' && board[2][0] == 'O') ||
             (board[0][1] == 'O' && board[1][1] == 'O' && board[2][1] == 'O') ||
             (board[0][2] == 'O' && board[1][2] == 'O' && board[2][2] == 'O') ||

             (board[0][0] == 'O' && board[1][1] == 'O' && board[2][2] == 'O') ||
             (board[0][2] == 'O' && board[1][1] == 'O' && board[2][0] == 'O')
    ) {
        *state = GAME_WIN;
        *winner = PLAYER_2;
    }

    else if ((board[0][0] != '0' && board[0][1] != '0' && board[0][2] != '0') &&
             (board[1][0] != '0' && board[1][1] != '0' && board[1][2] != '0') &&
             (board[2][0] != '0' && board[2][1] != '0' && board[2][2] != '0')
    ) {
        *state = GAME_WIN;
        *winner = PLAYER_NONE;
    }
}

char* player_str(Player player)
{
    if (player == PLAYER_1) return "player 1";
    if (player == PLAYER_2) return "player 2";
    if (player == PLAYER_NONE) return "player none";
    return "unknown";
}

void player_control(char board[BOARD_ROW][BOARD_COL], Player *player)
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        int click_col = GetMouseX() / BOARD_CELL_SIZE;
        int click_row = GetMouseY() / BOARD_CELL_SIZE;
        if (board[click_row][click_col] == '0') {
            if (*player == PLAYER_1) {
                board[click_row][click_col] = 'X';  // put chess
                *player = PLAYER_2; // switch player
            }
            else if (*player == PLAYER_2) {
                board[click_row][click_col] = 'O';  // put chess
                *player = PLAYER_1; // switch player
            }
        }
    }
}

void clear_board(char board[BOARD_ROW][BOARD_COL])
{
    for (int i = 0; i < BOARD_ROW; i++) {
        for (int j = 0; j < BOARD_COL; j++) {
            board[i][j] = '0';
        }
    }
}

void draw_board(char board[BOARD_ROW][BOARD_COL])
{
    for (int i = 0; i < BOARD_ROW; i++) {
        for (int j = 0; j < BOARD_COL; j++) {
            // back
            DrawRectangle(j * BOARD_CELL_SIZE,
                          i * BOARD_CELL_SIZE,
                          BOARD_CELL_SIZE,
                          BOARD_CELL_SIZE,
                          (i + j) & 1 ? DARKGRAY : LIGHTGRAY);
            // front
            switch (board[i][j]) {
                case 'O': {
                    Vector2 center = (Vector2){ (j * BOARD_CELL_SIZE) + (BOARD_CELL_SIZE / 2),
                                                (i * BOARD_CELL_SIZE) + (BOARD_CELL_SIZE / 2) };
                    float inner_radius = (BOARD_CELL_SIZE / 2.0f) - 20;
                    float outer_radius = (BOARD_CELL_SIZE / 2.0f) - 10;

                    DrawRing(center, inner_radius, outer_radius, 0, 360, 0, BLACK);
                    break;
                }
                case 'X': {
                    Vector2 left_top     = (Vector2){ j * BOARD_CELL_SIZE + 10, i * BOARD_CELL_SIZE + 10};
                    Vector2 right_bottom = (Vector2){ (j + 1) * BOARD_CELL_SIZE - 10, (i + 1) * BOARD_CELL_SIZE - 10};
                    Vector2 left_right   = (Vector2){ (j + 1) * BOARD_CELL_SIZE - 10, i * BOARD_CELL_SIZE + 10};
                    Vector2 right_top    = (Vector2){ j * BOARD_CELL_SIZE + 10, (i + 1) * BOARD_CELL_SIZE - 10};

                    DrawLineEx(left_top  , right_bottom, 10, BLACK);
                    DrawLineEx(left_right, right_top   , 10, BLACK);
                    break;
                }
                default:
                    break;
            }
        }
    }
}

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "tictactoe");

    GameState game_state = GAME_RUN;

    char board[BOARD_ROW][BOARD_COL];
    clear_board(board);

    Player player = PLAYER_1;
    Player winner;

    while (!WindowShouldClose()) {
        // Update
        if (game_state == GAME_RUN) {
            check_win(&game_state, board, &winner);
            player_control(board, &player);
        }
        if (game_state == GAME_WIN) {
            if (IsKeyPressed(KEY_ENTER)) {
                clear_board(board);
                player = PLAYER_1;
                game_state = GAME_RUN;
            }
        }

        // Render
        BeginDrawing();
            ClearBackground(GRAY);

            draw_board(board);
            if (game_state == GAME_RUN) {
                DrawText(TextFormat("player: %s", player_str(player)), 10, 10, 32, RED);
            }

            if (game_state == GAME_WIN) {
                DrawText("WIN", SCREEN_WIDTH / 2 - 32, SCREEN_HEIGHT / 2 - 32, 64, RED);
                DrawText(TextFormat("winner: %s", player_str(winner)), 10, 10,64, RED);
            }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
