#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <cstdlib>

#define H 16
#define W 41

using namespace std;

// Constants for characters
const char PACMAN = 'P';
const char GHOST = '@';
const char FOOD = '.';
const char WALL = '*';
const char EMPTY = ' ';

int food_collect = 0;
bool game_end = false;
int py = 1, px = 1;
int gy1 = 1, gx1 = 38, gy2 = 13, gx2 = 1;

// Function to create a random playfield
void generate_random_playfield(char playfield[][W]) {
    srand(time(0));  // Seed the random number generator

    int layout_choice = rand() % 5;  // Randomly select one of 5 layouts

    // Layout 1: Simple pattern
    if (layout_choice == 0) {
        const char layout[H][W] = {
            "****************************************",
            "*.....................................*",
            "*..*..*..*..*..*..*..*..*..*..*..*..*..*",
            "*..*..*..*..*..*..*..*..*..*..*..*..*..*",
            "*..*..*..*..*..*..*..*..*..*..*..*..*..*",
            "*..*..*..*..*..*..*..*..*..*..*..*..*..*",
            "*..*..*..*..*..*..*..*..*..*..*..*..*..*",
            "*.....................................*",
            "*..*..*..*..*..*..*..*..*..*..*..*..*..*",
            "*..*..*..*..*..*..*..*..*..*..*..*..*..*",
            "*..*..*..*..*..*..*..*..*..*..*..*..*..*",
            "*..*..*..*..*..*..*..*..*..*..*..*..*..*",
            "*..*..*..*..*..*..*..*..*..*..*..*..*..*",
            "*..*..*..*..*..*..*..*..*..*..*..*..*..*",
            "*.....................................*",
            "****************************************"
        };
        memcpy(playfield, layout, sizeof(layout));
    }
    // Layout 2: A more complex pattern
    else if (layout_choice == 1) {
        const char layout[H][W] = {
            "****************************************",
            "* ................................... *",
            "* * * * * * * * * * * * * * * * * * * *",
            "* * * * * * * * * * * * * * * * * * * *",
            "* * * * * * * * * * * * * * * * * * * *",
            "* * * * * * * * * * * * * * * * * * * *",
            "* ................................... *",
            "* * * * * * * * * * * * * * * * * * * *",
            "* * * * * * * * * * * * * * * * * * * *",
            "* * * * * * * * * * * * * * * * * * * *",
            "* * * * * * * * * * * * * * * * * * * *",
            "* * * * * * * * * * * * * * * * * * * *",
            "* ................................... *",
            "****************************************"
        };
        memcpy(playfield, layout, sizeof(layout));
    }
    // Layout 3: Maze-like pattern
    else if (layout_choice == 2) {
        const char layout[H][W] = {
            "****************************************",
            "* ................................... *",
            "* * * * * * * * * * * * * * * * * * * *",
            "* *   *       * *   *       * *   * * *",
            "* * * * * * * * * * * * * * * * * * * *",
            "* *   *       * * * * *   * *       * *",
            "* ................................... *",
            "* * * * * * * * * * * * * * * * * * * *",
            "* *   * *   * * * * * * *   * * * * * *",
            "* * * * * * * * * * * * * * * * * * * *",
            "* * * * * * * * * * * * * * * * * * * *",
            "* * * * * * * * * * * * * * * * * * * *",
            "* ................................... *",
            "****************************************"
        };
        memcpy(playfield, layout, sizeof(layout));
    }
    // Layout 4: Random walls and food
    else if (layout_choice == 3) {
        const char layout[H][W] = {
            "****************************************",
            "* ................................... *",
            "*...*...*.*..*...*.*..*...*....*....* *",
            "*....*..*..*.*....*.*....*..*..*..*...*",
            "*..*.*.....*...*....*....*.*...*.*....*",
            "*..*..*.*.*....*...*..*...*..*.....*...*",
            "* ................................... *",
            "*..*...*....*..*.....*.*..*..*...*...*",
            "*.*..*...*..*...*.....*.*....*...*....*",
            "*...*..*....*..*.*..*..*....*.*...*....*",
            "*.....*....*...*...*....*.*..*.*..*..* *",
            "*..*.*..*...*....*...*...*.*....*.....*",
            "* ................................... *",
            "****************************************"
        };
        memcpy(playfield, layout, sizeof(layout));
    }
    // Layout 5: Open space with sparse walls
    else if (layout_choice == 4) {
        const char layout[H][W] = {
            "****************************************",
            "* ................................... *",
            "*....*......*...*........*.....*...* *",
            "*...*....*..........*...........*..* *",
            "*...........*..*....*.....*......*...*",
            "*......*..*................*...*....*",
            "* ................................... *",
            "*.............*...........*..*.....*",
            "*..*....*....*.*.......*..........* *",
            "*....*....*........*...*.....*..*...*",
            "*...........*......*.............* *",
            "*................................... *",
            "****************************************"
        };
        memcpy(playfield, layout, sizeof(layout));
    }
}

void move_ghosts() {
    // Ghost 1 patrols rectangle clockwise
    if (gy1 == 1 && gx1 > 1) gx1--;
    else if (gx1 == 1 && gy1 < 7) gy1++;
    else if (gy1 == 7 && gx1 < 38) gx1++;
    else if (gx1 == 38 && gy1 > 1) gy1--;

    // Ghost 2 patrols rectangle counter-clockwise
    if (gy2 == 13 && gx2 < 38) gx2++;
    else if (gx2 == 38 && gy2 > 7) gy2--;
    else if (gy2 == 7 && gx2 > 1) gx2--;
    else if (gx2 == 1 && gy2 < 13) gy2++;
}

void user_input(char playfield[][W]) {
    if (kbhit()) {
        char c = getch();
        int new_py = py, new_px = px;

        switch (c) {
            case 72: new_py--; break; // up
            case 80: new_py++; break; // down
            case 75: new_px--; break; // left
            case 77: new_px++; break; // right
        }

        // Prevent movement into wall or out of bounds
        if (new_py >= 0 && new_py < H && new_px >= 0 && new_px < W &&
            playfield[new_py][new_px] != WALL) {
            py = new_py;
            px = new_px;
        }
    }
}

void setup(char playfield[][W]) {
    // Check ghost collision before placing characters
    if ((gy1 == py && gx1 == px) || (gy2 == py && gx2 == px)) {
        game_end = true;
        return;
    }

    // If food present, collect it
    if (playfield[py][px] == FOOD) {
        food_collect++;
    }

    // Clear previous characters
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            if (playfield[i][j] == PACMAN || playfield[i][j] == GHOST)
                playfield[i][j] = EMPTY;
    // Place the new positions
    playfield[py][px] = PACMAN;
    playfield[gy1][gx1] = GHOST;
    playfield[gy2][gx2] = GHOST;
}

void draw_playfield(char playfield[][W]) {
    system("cls");
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            cout << playfield[i][j];
        }
        cout << endl;
    }
    cout << "Score: " << food_collect << endl;
}

void game_result() {
    system("cls");
    if (food_collect == 500) {
        cout << "Congratulations! You won the game!" << endl;
        cout << "Your total score: " << food_collect << endl;
    } else {
        cout << "Game Over!" << endl;
        cout << "Your total score: " << food_collect << endl;
    }
    cout << "Do you want to play again? (y/n): ";
    char choice;
    cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        food_collect = 0;
        py = 1; px = 1;
        gy1 = 1; gx1 = 38;
        gy2 = 13; gx2 = 1;
        game_end = false;
    } else {
        exit(0);
    }
}

int main() {
    char playfield[H][W];
    generate_random_playfield(playfield);
    
    while (true) {
        while (!game_end) {
            setup(playfield);
            user_input(playfield);
            move_ghosts();
            draw_playfield(playfield);
            Sleep(200);
        }

        game_result();
    }

    return 0;
}
