#include <ncurses.h> // Make sure this is correct for your OS
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define BOARD_ROWS 20 // You are free to change this
#define BOARD_COLS 50 // You are free to change this
#define MAX_NAME 20
#define ROBOT_BODY 'O'
#define ROBOT_HEAD '^'
#define PERSON 'P'
#define MINE 'X'
#define OBSTACLE '#'

// Add any additional definitions here

// Structs
// Change as you see fit
typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    Position pos;
    int direction; // N, S, E, W
} Robot;

typedef struct {
    char name[MAX_NAME];
    int score;
    int lives;
    int level;
} Player;

typedef struct {
    int width;
    int height;
    int center_x;
    int center_y;
} CrossObstacle;

// Other structs you could add such as Leaderboard, Game Settings, etc.

// Function prototypes, change as you see fit
WINDOW* init_game(Player player, Robot robot, Position person, Position* mines, int mine_count);
void draw_title_screen(void);
void update_UI(Player player, Robot robot, Position person, Position* mines, int mine_count);
void handle_input(Robot* robot, int input, Position* person, Position* mines, int mine_count);
void move_robot(Robot* robot, Position* person);
void move_robot_ai(Robot* robot, Position* person, Position* mines, int mine_count);
void clear_robot(Robot* robot);
void draw_robot(Robot* robot);
void check_collision(Robot* robot, Position* mines, int mine_count);
void spawn_person(Position* person, Robot* robot, Position* mines, int mine_count);
void spawn_mines(Position* mines, int* mine_count, Robot* robot, Position* person);
void game_over_screen(Player* player);
void save_score(Player* player);
void show_leaderboard(void);
void draw_obstacle(CrossObstacle* obstacle);
bool is_obstacle_position(CrossObstacle* obstacle, int x, int y);

int main() {
    // Initialize ncurses
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    srand(time(NULL));
    nodelay(stdscr, TRUE); // This call makes getch() non-blocking, you may need to change this

    // Initialize game variables
    // TODO use DMA to allocate memory for these
    Player player   = { .lives = 3, .score = 0, .level = 1 };
    Robot robot     = { .pos = { 0, 0 }, .direction = 'N' };
    Position person = { 0, 0 };
    Position mines[50]; // Maximum 50 mines
    int mine_count = 0;
    int delay      = 10000;

    // dma allocation for initialising game variable


    // TODO: Show title screen and get player name

    WINDOW* board = init_game(player, robot, person, mines, mine_count);
    update_UI(player, robot, person, mines, mine_count);

    // TODO: Game loop
    while (true) {
        // Handle input
        int ch = getch();
        handle_input(&robot, ch, &person, mines, mine_count);

        // TODO: Game logic
        // Most of your implementation will go here


        // Delay in microseconds
        usleep(delay);
    }

    // Wait for user input before exiting
    nodelay(stdscr, FALSE); // Make getch() blocking
    getch();

    // Cleanup and exit
    endwin();
    return 0;
}

// Dummy function
void draw_title_screen() {
    // TODO: Implement title screen with instructions
    // Add a starting screen before the box is drawn [1 mark]
    // A dummy draw_title_screen() function has been provided for you
    // Add some game instructions to the starting screen
    // Ask the user to input their name
    // Save this in a struct to track the user’s name, current score and lives remaining
    // You could earn marks for creativity here
}

// Basic border drawing function provided, change as you see fit
WINDOW* init_game(Player player, Robot robot, Position person, Position* mines, int mine_count) {
    // Initialize the board
    int ymax, xmax;
    getmaxyx(stdscr, ymax, xmax);
    WINDOW* board = newwin(BOARD_ROWS, BOARD_COLS, (ymax - BOARD_ROWS) / 2, (xmax - BOARD_COLS) / 2);
    box(board, 0, 0);
    refresh();
    wrefresh(board);
    // Update the UI
    update_UI(player, robot, person, mines, mine_count);
    return board;
}

// Students to change as they see fit
void update_UI(Player player, Robot robot, Position person, Position* mines, int mine_count) {
    mvwprintw(stdscr, 1, 1, "Player: %s", player.name);
    mvwprintw(stdscr, 1, 11, "Lives: %d", player.lives);
    mvwprintw(stdscr, 1, 20, "Score: %d", player.score);
    mvwprintw(stdscr, 1, 30, "Level: %d", player.level);
    wrefresh(stdscr);
}

// Students to implement
void handle_input(Robot* robot, int input, Position* person, Position* mines, int mine_count) {
    // TODO: Handle arrow key inputs
    switch (input) {
    case KEY_UP:
        // Students to implement
        break;
    case KEY_DOWN:
        // Students to implement
        break;
    case KEY_LEFT:
        // Students to implement
        break;
    case KEY_RIGHT:
        // Students to implement
        break;
    default:
        // Auto movement if no key pressed
        move_robot_ai(robot, person, mines, mine_count);
    }
}

// Students to implement
void move_robot(Robot* robot, Position* person) {
    // TODO: Move robot based on current direction这个link一下x and y then change position based on user input
}

// Students to implement
void move_robot_ai(Robot* robot, Position* person, Position* mines, int mine_count) {
    // TODO: Implement AI movement towards person while avoiding mines
}

// Students to implement
void clear_robot(Robot* robot) {
    // TODO: Clear robot's current position
}

// Students to implement
void draw_robot(Robot* robot) {
    // TODO: Draw robot at new position
}

// Students to implement
void check_collision(Robot* robot, Position* mines, int mine_count) {
    // TODO: Check for collisions with walls and mines
}

// Students to implement
void spawn_person(Position* person, Robot* robot, Position* mines, int mine_count) {
    // TODO: Spawn person at random location这个就用那个random for x y and then use if to check if it's at a wall or mine, if yes then redo
}

// Students to implement
void spawn_mines(Position* mines, int* mine_count, Robot* robot, Position* person) {
    // TODO: Spawn new mines for each level 又是用random加2个
}

// Students to implement
void game_over_screen(Player* player) {
    // TODO: Show game over screen with final score
}

// Students to implement
void save_score(Player* player) {
    // TODO: Save score to leaderboard.txt
}

// Students to implement
void show_leaderboard(void) {
    // TODO: Show ordered scores from leaderboard.txt
    // Think about possible edge cases, such as too many scores to display or the file not existing
}

// Students to implement
void draw_obstacle(CrossObstacle* obstacle) {
    // TODO: Draw the cross-shaped obstacle on the game field这个用random弄个centre_mine_x and y 然后draw x and y
}

// Students to implement
bool is_obstacle_position(CrossObstacle* obstacle, int x, int y) {
    // TODO: Check if the given (x, y) position is part of the obstacle
}