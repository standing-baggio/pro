#include <raylib.h>
#include "ping.h"
#include "snake.h"
#include "conway.h"  // Include the header file for the Game of Life

int menu() {

    const int numOptions = 4;  // Update to 4 options
    const char *options[numOptions] = {"Ping", "Snake", "Game of Life", "Exit"};
    int selectedOption = 0;

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        if (IsKeyPressed(KEY_DOWN)) {
            selectedOption = (selectedOption + 1) % numOptions;
        } else if (IsKeyPressed(KEY_UP)) {
            selectedOption = (selectedOption - 1 + numOptions) % numOptions;
        }

        if (IsKeyPressed(KEY_ENTER)) {
            switch (selectedOption) {
                case 0:
                    ping();
                    break;
                case 1:
                    snake();
                    break;
                case 2:
                    conway();  // Call the Game of Life function
                    break;
                case 3:
                    CloseWindow();
                    return 0;
                default:
                    break;
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int i = 0; i < numOptions; ++i) {
            if (i == selectedOption) {
                DrawText(TextFormat("> %s <", options[i]), 350, 180 + 40 * i, 20, RED);
            } else {
                DrawText(options[i], 375, 180 + 40 * i, 20, BLACK);
            }
        }

        EndDrawing();
    }

    // De-Initialization
    CloseWindow(); // Close window and OpenGL context
    return 0;
}
