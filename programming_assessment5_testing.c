#include <ncurses.h> // Make sure this is correct for your OS
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "common.h"

void draw_title_screen(Player* player) {
    if (player == NULL) return;

    mvaddstr(1, 1, "ROBOT RESCUE MISSION\n");
    mvaddstr(3, 1, "Instructions:\n");
    mvaddstr(4, 1, "Use arrow keys to move the robot.\n");
    mvaddstr(5, 1, "Rescue the person (P) while avoiding mines (X) and obstacles (#).\n");
    mvaddstr(6, 1, "Power-ups (*) may appear to help you when you reach level 3!\n");
    mvaddstr(7, 1, "Press 'm' to start the game...\n");
    refresh();

    while (getch() == -1);
}

void update_UI(Player* player, Robot* robot, Position* person, Position* mines, int mine_count, Game* game) {
    // Update the UI with player stats
    mvwprintw(stdscr, 1, 1, "Player: %s", player->name);
    mvwprintw(stdscr, 1, 11, "Lives: %d", player->lives);
    mvwprintw(stdscr, 1, 20, "Score: %d", player->score);
    mvwprintw(stdscr, 1, 30, "Level: %d", player->level);
    refresh();
}

void handle_input(Robot* robot, int input, Position* person, Position* mines, int* mine_count, Game* game, CrossObstacle* obstacle) {
    // TODO: Handle arrow key inputs
    switch (input) {
    case KEY_UP:
        robot->direction = 'N';
        if (robot->pos.y > 0)
            robot->pos.y--;
        break;
    case KEY_DOWN:
        robot->direction = 'S';
        if (robot->pos.y < BOARD_ROWS - 1)
            robot->pos.y++;
        break;
    case KEY_LEFT:
        robot->direction = 'W';
        if (robot->pos.x > 0)
            robot->pos.x--;
        break;
    case KEY_RIGHT:
        robot->direction = 'E';
        if (robot->pos.x < BOARD_COLS - 1)
            robot->pos.x++;
        break;
    default:
        // Auto movement if no key pressed
        move_robot_ai(robot, person, mines, *mine_count, obstacle);
    }
}

// Students to implement
void move_robot(Robot* robot, Position* person, int input) {
    if (robot == NULL || person == NULL) {
        return;
    }
    switch (input) {
    case KEY_UP:
        robot->direction = 'N';
        if (robot->pos.y > 0)
            robot->pos.y--;
        break;
    case KEY_DOWN:
        robot->direction = 'S';
        if (robot->pos.y < BOARD_ROWS - 1)
            robot->pos.y++;
        break;
    case KEY_LEFT:
        robot->direction = 'W';
        if (robot->pos.x > 0)
            robot->pos.x--;
        break;
    case KEY_RIGHT:
        robot->direction = 'E';
        if (robot->pos.x < BOARD_COLS - 1)
            robot->pos.x++;
        break;
    default:
        break;
    }
}

void move_robot_ai(Robot* robot, Position* person, Position* mines, int mine_count, CrossObstacle* obstacle) {
    // check for null pointers
    if (robot == NULL || person == NULL) {
        return;
    }

    int dx, dy = 0; // direction x and y
    int nx, ny;     // new x and y

    if (robot->pos.x < person->x) {
        dx = 1; // Move right
    } else if (robot->pos.x > person->x) {
        dx = -1; // Move left
    }
    if (robot->pos.y < person->y) {
        dy = 1; // Move down
    } else if (robot->pos.y > person->y) {
        dy = -1; // Move up
    }

    // try moving in x direction first
    if (dx != 0) { // makes the AI only try a move along x axis when a move is actually needed
        nx = robot->pos.x + dx;
        ny = robot->pos.y;
        if (!collides_at(nx, ny, mines, mine_count, obstacle)) {
            robot->pos.x = nx;
            return;
        }
    }
    // try moving in y direction
    if (dy != 0) { // makes the AI only try a move along y axis when a move is actually needed
        nx = robot->pos.x;
        ny = robot->pos.y + dy;
        if (!collides_at(nx, ny, mines, mine_count, obstacle)) {
            robot->pos.y = ny;
            return;
        }
    }

    // if it collides, try each direction in order N, S, E, W
    //  Try North
    nx = robot->pos.x;
    ny = robot->pos.y - 1;
    if (collides_at(nx, ny, mines, mine_count, obstacle)) {
        // blocked, try next direction
    } else {
        robot->pos.y     = ny;
        robot->direction = 'N';
        return;
    }

    // Try East
    nx = robot->pos.x + 1;
    ny = robot->pos.y;
    if (collides_at(nx, ny, mines, mine_count, obstacle)) {
        // blocked, try next direction
    } else {
        robot->pos.x     = nx;
        robot->direction = 'E';
        return;
    }

    // Try South
    nx = robot->pos.x;
    ny = robot->pos.y + 1;
    if (collides_at(nx, ny, mines, mine_count, obstacle)) {
        // blocked, try next direction
    } else {
        robot->pos.y     = ny;
        robot->direction = 'S';
        return;
    }

    // Try West
    nx = robot->pos.x - 1;
    ny = robot->pos.y;
    if (collides_at(nx, ny, mines, mine_count, obstacle)) {
        // blocked, all options tried -> stay in place
        return;
    } else {
        robot->pos.x     = nx;
        robot->direction = 'W';
        return;
    }
}

bool collides_at(int x, int y, Position* mines, int mine_count, CrossObstacle* obstacle) {
    // Check for collision with walls
    if (x <= 0 || x >= BOARD_COLS - 1 || y <= 0 || y >= BOARD_ROWS - 1) {
        return true; // hits wall
    }
    // Check for collision with mines
    if (mines != NULL) {
        for (int i = 0; i < mine_count; i++) {
            if (x == mines[i].x && y == mines[i].y) {
                return true; // hits mine
            }
        }
    }
    // Check for collision with obstacle
    if (is_obstacle_position(obstacle, x, y)) {
        return true; // hits obstacle
    }
    return false; // no collision
}
// Students to implement
void clear_robot(Game* game, Robot* robot) {
    // check if memory allocation is successful
    if (game == NULL || game->field.window == NULL || robot == NULL) { // also field.window to double check that it's been initialised, needs field.window because game is passed by value
        return;
    }
    // Clear robot's current position
    int max_rows = game->field.rows - 2; // accounting for border
    int max_cols = game->field.cols - 2; // accounting for border
    int robot_x  = robot->pos.x;
    int robot_y  = robot->pos.y;

    if (robot_x > 1 && robot_x < max_cols && robot_y > 1 && robot_y < max_rows) { // checking to see if body is within the borders before trying to clear
        mvwaddch(game->field.window, robot_y, robot_x, ' ');
    }

    // finding head position
    switch (robot->direction) {
    case 'N':
        robot_y--; // head above body
        break;
    case 'S':
        robot_y++; // head below body
        break;
    case 'E':
        robot_x++; // head to the right of body
        break;
    case 'W':
        robot_x--; // head to the left of body
        break;
    default:
        break;
    }
    if (robot_y > 1 && robot_y < max_rows && robot_x > 1 && robot_x < max_cols) { // checking to see if head is within the borders before trying to clear
        mvwaddch(game->field.window, robot_y, robot_x, ' ');
    }
    mvwaddch(game->field.window, robot_y, robot_x, ' '); // clearing robots position
}

// Students to implement
void draw_robot(Game* game, Robot* robot) {
    // TODO: Draw robot at new position
    if (game == NULL || robot == NULL) { // check for null pointers
        return;
    }

    // add colour
    if (has_colors()) {
        wattron(game->field.window, COLOR_PAIR(1)); // robot colour
    }
    // draw body
    mvwaddch(game->field.window, robot->pos.y, robot->pos.x, ROBOT_BODY);
    // draw head based on direction
    chtype head_ch = '^'; //
    switch (robot->direction) {
    case 'N':
        mvwaddch(game->field.window, robot->pos.y - 1, robot->pos.x, '^'); // head above body
        break;
    case 'S':
        mvwaddch(game->field.window, robot->pos.y + 1, robot->pos.x, 'v'); // head below body
        break;
    case 'E':
        mvwaddch(game->field.window, robot->pos.y, robot->pos.x + 1, '>'); // head to the right of body
        break;
    case 'W':
        mvwaddch(game->field.window, robot->pos.y, robot->pos.x - 1, '<'); // head to the left of body
        break;
    default:
        break;
    }
    if (has_colors()) {
        wattroff(game->field.window, COLOR_PAIR(1)); // turn off robot colour
    }
    wrefresh(game->field.window);
}

bool check_collision(Game* game, Player* player, Robot* robot, Position* mines, int* mine_count, CrossObstacle* obstacle, Position* person) {
    if (robot == NULL || player == NULL || mines == NULL || mine_count == NULL || person == NULL) { // check for null pointers
        return false;
    }

    bool ret = false;

    // Check for collisions with walls,  mines and obstacles
    //  Check for collision with walls
    if (robot->pos.x <= 0 || robot->pos.x >= BOARD_COLS - 1 ||
    robot->pos.y <= 0 || robot->pos.y >= BOARD_ROWS - 1) {
        player->lives--; // lose a life when a wall is hit
        // reset robot position to starting point (middle left of board)
        int home_x       = robot->pos.x;
        int home_y       = robot->pos.y;
        robot->direction = 'N'; // reset direction to north
    }
    // display current lives, score and level on the screen
    update_UI(player, robot, person, mines, *mine_count, game);
    return ret;

    // Check for collision with mines
    for (int i = 0; i < *mine_count; i++) {
        if (robot->pos.x == mines[i].x && robot->pos.y == mines[i].y) {
            player->lives--; // lose a life when a mine is hit
            // reset robot position to starting point (middle left of board)
            int home_x       = robot->pos.x;
            int home_y       = robot->pos.y;
            robot->direction = 'N'; // reset direction to north
            // remove the mine that was hit
            ret = true;
            for (int j = i; j < *mine_count - 1; j++) {
                mines[j] = mines[j + 1];
            }
            (*mine_count)--; // decrease mine count
        }
    }

    // Update player stats
    mvprintw(1, 1, "Player: %s", player->name);
    mvprintw(1, 11, "Lives: %d", player->lives);
    mvprintw(1, 20, "Score: %d", player->score);
    mvprintw(1, 30, "Level: %d", player->level);
    refresh();
    return ret;
}

void clear_mines(Game* game, Position* mines, int mine_count) {
    if (game == NULL || game->field.window == NULL || mines == NULL) { // check for null pointers
        return;
    }
    // Clear all mines from the game field
    for (int i = 0; i < mine_count; i++) {
        mvwaddch(game->field.window, mines[i].y, mines[i].x, ' ');
    }
    wrefresh(game->field.window);
}

// drawing the 5 mines at the start of the game
void spawn_mines_initial(Position* mines, int* mine_count, Robot* robot, Position* person, Game* game) {
    int rand_x, rand_y = 0;
    if (mines == NULL || mine_count == NULL || robot == NULL || person == NULL || game == NULL) { // check for null pointers
        return;
    }

    // Generate random positions for new mines
    // 5 mines for level 1

    while (*mine_count < 5) {
        int tries     = 0;
        int max_tries = 100; // Prevent infinite loops
        while (tries < max_tries) {
            rand_x = rand() % (BOARD_COLS - 2) + 1; // Avoid borders
            rand_y = rand() % (BOARD_ROWS - 2) + 1; // Avoid borders

            // avoid existing mines
            bool on_mine = false; // checks to see if person spawns on mine
            for (int i = 0; i < *mine_count; ++i) {
                if (mines[i].x == person->x && mines[i].y == person->y) {
                    on_mine = true;
                    break;
                }
            }
            // avoid robot
            bool on_robot = false; // checks to see if mine spawns on robot
            if (robot->pos.x == rand_x && robot->pos.y == rand_y) {
                on_robot = true;
                break;
            }
            // if mine spawns on person or robot, continue to next iteration
            if (on_mine || on_robot) {
                continue;
                tries++; // increment tries
            } else {
                mines[*mine_count].x = rand_x;
                mines[*mine_count].y = rand_y;
                (*mine_count)++;
                init_pair(4, COLOR_RED, COLOR_BLACK);
                mvprintw(mines[*mine_count - 1].y + game->field.start.y, mines[*mine_count - 1].x + game->field.start.x, "%c", MINE); //-1 because indexing starts from 0
                tries = max_tries;                                                                                                    // Exit the tries loop
            }
        }
    }
}

// drawing 2 additional mines for each level up after level 1
void spawn_mines_additional(Position* mines, int* mine_count, Robot* robot, Position* person, Game* game) {
    if (mines == NULL || mine_count == NULL || robot == NULL || person == NULL || game == NULL) { // check for null pointers
        return;
    }
    int rand_x, rand_y = 0;
    const int mines_per_level = 2;
    const int max_tries       = 100; // Prevent infinite loops

    // Generate random positions for new mines
    // 2 additional mines for each level after level 1
    for (int m = 0; m < mines_per_level; m++) {
        int tries = 0;
        while (tries < max_tries && *mine_count < max_mines) {
            rand_x = rand() % (BOARD_COLS - 2) + 1; // Avoid borders
            rand_y = rand() % (BOARD_ROWS - 2) + 1; // Avoid borders

            // avoid existing mines
            bool on_mine = false; // checks to see if person spawns on mine
            for (int i = 0; i < *mine_count; ++i) {
                if (mines[i].x == person->x && mines[i].y == person->y) {
                    on_mine = true;
                    break;
                }
            }
            // avoid robot
            bool on_robot = false; // checks to see if mine spawns on robot
            if (robot->pos.x == rand_x && robot->pos.y == rand_y) {
                on_robot = true;
                break;
            }
            // if mine spawns on person or robot, continue to next iteration
            if (on_mine || on_robot) {
                tries++; // increment tries
                continue;
            } else {
                mines[*mine_count].x = rand_x;
                mines[*mine_count].y = rand_y;
                (*mine_count)++;
                init_pair(4, COLOR_RED, COLOR_BLACK);
                mvprintw(mines[*mine_count - 1].y + game->field.start.y, mines[*mine_count - 1].x + game->field.start.x, "%c", MINE); //-1 because indexing starts from 0
                tries = max_tries;                                                                                                    // Exit the tries loop
            }
        }
    }
}

// Students to implement
void game_over_screen(Player* player) {
    // TODO: Show game over screen with final score
    clear();
    mvprintw(5, 5, "Game Over!");
    mvprintw(7, 5, "Final Score: %d", player->score);
    mvprintw(9, 5, "Press any key to continue.");
    refresh();
    nodelay(stdscr, FALSE); // Makes getch() blocking
    getch();                // waits for user input
    nodelay(stdscr, TRUE);  // Restores non-blocking input
}

// Students to implement
void save_score(Player* player) {
    // TODO: Save score to leaderboard.txt
    FILE* file = fopen("leaderboard.txt", "a");
    // checks if file opened successfully
    if (file == NULL) {
        clear();
        mvprintw(5, 5, "Error: Could not open leaderboard");
        mvprintw(7, 5, "Press any key to return to the game.");
        refresh();
        nodelay(stdscr, FALSE); // Makes getch() blocking
        getch();                // waits for user input
        nodelay(stdscr, TRUE);  // Restores non-blocking input
        return;
    }

    fprintf(file, "%s %d\n", player->name, player->score); // coping name and score to file
    fclose(file);
}

// Students to implement
void show_leaderboard(void) {
    // Show ordered scores from leaderboard.txt, top 10 only, highest to lowest
    FILE* file = fopen("leaderboard.txt", "r");

    // checks if file opened successfully
    if (file == NULL) {
        clear();
        mvprintw(5, 5, "Error: Could not open leaderboard");
        mvprintw(7, 5, "Press any key to return to the game.");
        refresh();
        nodelay(stdscr, FALSE); // Makes getch() blocking
        getch();                // waits for user input
        nodelay(stdscr, TRUE);  // Restores non-blocking input
        return;
    }

    LeaderboardEntry top[10]; // array to hold top 10 scores
    int num_entries = 0;
    char name[MAX_NAME];
    int score, i;

    while (fscanf(file, "%19s %d", name, &score) == 2) { // 19 because there's a 0/, ==2 to ensure both name and score are read
        int insert_at = 0;
        while (insert_at < num_entries && insert_at < 10 && score <= top[insert_at].score) { // finding position to insert new score
            insert_at++;
        }

        // only insert if within top 10
        if (insert_at < 10) {
            if (num_entries < 10) { // increase num_entries when there's less than 10 entries
                num_entries++;
                int start_i = num_entries;
                if (start_i >= 10) {
                    start_i = 9; // because it starts from 0
                }

                for (i = start_i; i > insert_at; i--) { // Shift down entries to make space
                    top[i]       = top[i - 1];          // Shift down entries
                    top[i].score = top[i - 1].score;    // Shift down scores
                }

                // Insert new entry data
                strncpy(top[insert_at].name, name, MAX_NAME - 1);
                top[insert_at].name[MAX_NAME - 1] = '\0'; // Ensure null-termination
                top[insert_at].score              = score;
            }
        }
    }

    // close file after reading
    fclose(file);

    // Display leaderboard
    clear();
    mvprintw(1, 5, "Leaderboard - Top 10 Scores");
    for (i = 0; i < num_entries && i < 10; i++) {
        mvprintw(3 + i, 5, "%d. %s - %d", i + 1, top[i].name, top[i].score); // Display rank, name and score on a new line
        refresh();
    }

    // Wait for user input before returning to game
    mvprintw(15, 5, "Press any key to return to the game.");
    refresh();
    nodelay(stdscr, FALSE); // Makes getch() blocking
    getch();                // waits for user input
    nodelay(stdscr, TRUE);  // Restores non-blocking input
}

void draw_obstacle(CrossObstacle* obstacle) {
    if (obstacle == NULL) {
        return;
    }
    int xmax, ymax;
    const int W = 10, H = 10; // width and height of obstacle
    getmaxyx(stdscr, ymax, xmax);
    // starting position is top-left corner of board
    int starty = (ymax - BOARD_ROWS) / 2;
    int startx = (xmax - BOARD_COLS) / 2;

    int width    = obstacle->width;
    int height   = obstacle->height;
    int center_x = obstacle->center_x;
    int center_y = obstacle->center_y;

    int west  = center_x - W / 2;
    int east  = center_x + W - 1; //-1 to account for 0 indexing
    int north = center_y - H / 2;
    int south = center_y + H - 1; //-1 to account for 0 indexing
    // add colours
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    // Draw horizontal part
    for (int x = west; x <= east; x++) {
        // check board limits
        if (x < 0 || x >= BOARD_COLS) {
            continue;
        }

        int sx = startx + x;
        int sy = starty + center_y;

        if (sx < 0 || sx >= BOARD_COLS || sy < 0 || sy >= BOARD_ROWS) {
            continue;
        }

        mvaddch(sy, sx, OBSTACLE);
        refresh();
    }
    // draw vertical part
    for (int y = north; y <= south; y++) {
        // check board limits
        if (y < 0 || y >= BOARD_ROWS) {
            continue;
        }

        int sx = startx + center_x;
        int sy = starty + y;

        if (sx < 0 || sx >= BOARD_COLS || sy < 0 || sy >= BOARD_ROWS) {
            continue;
        }

        mvaddch(sy, sx, OBSTACLE);
        refresh();
    }
    if (has_colors()) {
        attroff(COLOR_PAIR(3)); // turn off obstacle colour
    }
}

// Students to implement
bool is_obstacle_position(CrossObstacle* obstacle, int x, int y) {
    // checks if obstacle collided with the robot
    if (obstacle == NULL) {
        return false;
    }

    const int W = 10, H = 10; // width and height of obstacle
    int center_x = obstacle->center_x;
    int center_y = obstacle->center_y;

    int west  = center_x - W / 2;
    int east  = center_x + W - 1; //-1 to account for 0 indexing
    int north = center_y - H / 2;
    int south = center_y + H - 1; //-1 to account for 0 indexing

    // Check horizontal part
    if (y == center_y && x >= west && x <= east) {
        return true;
    }
    // Check vertical part
    if (x == center_x && y >= north && y <= south) { // because y increases as you go down
        return true;
    }
    return false;
}

WINDOW* init_game(Game* game) {
    // Initialize the board
    int ymax, xmax;
    getmaxyx(stdscr, ymax, xmax);
    game->field.rows    = BOARD_ROWS;
    game->field.cols    = BOARD_COLS;
    game->field.start.y = (ymax - BOARD_ROWS) / 2;
    game->field.start.x = (xmax - BOARD_COLS) / 2;
    game->field.window  = newwin(game->field.rows, game->field.cols, game->field.start.y, game->field.start.x);
    if (game->field.window == NULL) {
        endwin();
        fprintf(stderr, "Failed to create game window\n");
        exit(1);
    }
    box(game->field.window, 0, 0); // Draw border
    wrefresh(game->field.window);
    return game->field.window;
}
// Draw border directly on stdscr using Game.field size
void draw_border(Game* game) {
    int ymax, xmax;
    getmaxyx(stdscr, ymax, xmax);

    int start_y = (ymax - game->field.rows) / 2;
    int start_x = (xmax - game->field.cols) / 2;

    // Horizontal borders
    for (int x = start_x; x < start_x + game->field.cols; x++) {
        mvaddch(start_y, x, ACS_HLINE);
        mvaddch(start_y + game->field.rows - 1, x, ACS_HLINE);
    }

    // Vertical borders
    for (int y = start_y; y < start_y + game->field.rows; y++) {
        mvaddch(y, start_x, ACS_VLINE);
        mvaddch(y, start_x + game->field.cols - 1, ACS_VLINE);
    }

    // Corners
    mvaddch(start_y, start_x, ACS_ULCORNER);
    mvaddch(start_y, start_x + game->field.cols - 1, ACS_URCORNER);
    mvaddch(start_y + game->field.rows - 1, start_x, ACS_LLCORNER);
    mvaddch(start_y + game->field.rows - 1, start_x + game->field.cols - 1, ACS_LRCORNER);
}

void power_ups(Player* player, Position* power_up, Robot* robot, Position* person, Position* mines, int* mine_count) {
    int rand_x, rand_y         = 0;
    int power_up_x, power_up_y = 0;
    int power_up_active = 0;
    time_t power_up_end = 0;
    *power_up           = (Position){ 0, 0 };
    // seed the random number generator
    srand(time(NULL));
    // checks that level is 3 or above before spawning power-up
    if (player->level >= 3) {
        // generate colour for power-up and random position for power-up and ensure it's not on robot, person, mines, wall or obstacle
        do {
            rand_x      = rand() % (BOARD_COLS - 2) + 1; // Avoid borders
            rand_y      = rand() % (BOARD_ROWS - 2) + 1; // Avoid borders
            power_up->x = rand_x;
            power_up->y = rand_y;
            if (has_colors()) {
                if (start_color() == OK) {
                    init_pair(5, COLOR_GREEN, COLOR_WHITE);
                    attron(COLOR_PAIR(5));
                    mvaddch(power_up->y, power_up->x, POWER_UP);
                    refresh();
                }
            }
        } while (power_up->x == robot->pos.x && power_up->y == robot->pos.y); // checks whether robot has reached power-up
        power_up_active = 1;

        if (power_up_active == 1) {
            // Power-up effect: can pass through mines for 10 seconds
            // Set a 10 second timer to indicate the power-up is active so user can pass through mines and is aware of how much time is left
            power_up_end = time(NULL) + 10; // 10 seconds from now
            while (time(NULL) < power_up_end) {
                mvprintw(2, 1, "Power-up active! Time left: %d seconds", (int)(power_up_end - time(NULL)));
                refresh();
            }
            // After 10 seconds, remove power-up from screen
            mvaddch(power_up->y, power_up->x, ' ');
            if (has_colors()) {
                attroff(COLOR_PAIR(5)); // turn off power-up colour
            }
            refresh();
            power_up_active = 0;                                      // Deactivate power-up after 10 seconds
            mvprintw(2, 1, "                                      "); // clear power-up timer message
            refresh();
        }
    }
}

void draw_person(Game* game, Position* person, Robot* robot, CrossObstacle* obstacle) {
    if (game == NULL || game->field.window == NULL || person == NULL) { // check for null pointers
        return;
    }
    // add colour
    if (has_colors()) {
        wattron(game->field.window, COLOR_PAIR(2)); // person colour
    }
    // Draw person at random position that's not on robot, obstacle or wall
    if (person->x > 0 && person->x < game->field.cols - 1 &&
    person->y > 0 && person->y < game->field.rows - 1) { // checking to see if person is within the borders before trying to draw
        int obstacle_collision = is_obstacle_position(obstacle, person->x, person->y);
        if (!obstacle_collision) {                                           // only draw person if not colliding with obstacle
            while (robot->pos.x == person->x && robot->pos.y == person->y) { // ensure person doesn't spawn on robot
                person->x = rand() % (game->field.cols - 2) + 1;             // Avoid borders
                person->y = rand() % (game->field.rows - 2) + 1;             // Avoid borders
            }
            mvwaddch(game->field.window, person->y, person->x, PERSON);
        }
    }
    if (has_colors()) {
        wattroff(game->field.window, COLOR_PAIR(2)); // turn off person colour
    }
    wrefresh(game->field.window);
}

void clear_person(Game* game, Position* person) {
    if (game == NULL || game->field.window == NULL || person == NULL) { // check for null pointers
        return;
    }
    // Clear person's current position
    mvwaddch(game->field.window, person->y, person->x, ' ');
    wrefresh(game->field.window);
}

void reset_game(Game* game, Player* player, Robot* robot, Position* person, Position* mines, int* mine_count, CrossObstacle* obstacle) {
    if (player == NULL || robot == NULL || person == NULL || mines == NULL || game == NULL) { // check for null pointers
        return;
    }

    // Reset player stats
    player->lives = 3;
    player->score = 0;
    player->level = 1;

    // setting 'home' position for robot when it hits a wall or mine or obsticle, this home position is the centre of the second quadrant of the board
    // second quadrent centre position
    clear_robot(game, robot);
    int home_x       = 1 + (BOARD_COLS - 2) / 4; // centre of second quadrent x
    int home_y       = 1 + (BOARD_ROWS - 2) / 4; // centre of second quadrent y
    robot->pos.x     = home_x;
    robot->pos.y     = home_y;
    robot->direction = 'N'; // reset direction to north
    draw_robot(game, robot);

    // Reset person position
    clear_person(game, person);
    person->x = rand() % (BOARD_COLS - 2) + 1; // Avoid borders
    person->y = rand() % (BOARD_ROWS - 2) + 1; // Avoid borders
    draw_person(game, person, robot, obstacle);

    // drawing cross obstacle in the centre of the board
    obstacle->center_x = BOARD_COLS / 2;
    obstacle->center_y = BOARD_ROWS / 2;
    obstacle->width    = 10;
    obstacle->height   = 10;
    draw_obstacle(obstacle);
    *mine_count = 0;
    spawn_mines_initial(mines, mine_count, robot, person, game);
}


int main() {
    // Initialize ncurses
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    srand(time(NULL));
    nodelay(stdscr, TRUE); // This call makes getch() non-blocking, you may need to change this

    // colour pairs
    if (has_colors()) {
        if (start_color() == OK) {
            init_pair(1, COLOR_CYAN, COLOR_WHITE);    // robot colour
            init_pair(2, COLOR_MAGENTA, COLOR_WHITE); // person colour
            init_pair(3, COLOR_BLUE, COLOR_BLACK);    // obstacle colour
            init_pair(4, COLOR_RED, COLOR_BLACK);     // mine colour
            init_pair(5, COLOR_GREEN, COLOR_WHITE);   // power-up colour
        }
    }

    // pointers declaration for dma allocated variables
    Player* player_ptr = malloc(sizeof *player_ptr);
    if (player_ptr == NULL) {
        endwin();
        fprintf(stderr, "Memory allocation failed for Player\n");
        return 1;
    }
    memset(player_ptr, 0, sizeof *player_ptr);

    Robot* robot_ptr = malloc(sizeof *robot_ptr);
    if (robot_ptr == NULL) {
        free(player_ptr);
        endwin();
        fprintf(stderr, "Memory allocation failed for Robot\n");
        return 1;
    }
    memset(robot_ptr, 0, sizeof *robot_ptr);

    Position* person_ptr = malloc(sizeof *person_ptr);
    if (person_ptr == NULL) {
        free(player_ptr);
        free(robot_ptr);
        endwin();
        fprintf(stderr, "Memory allocation failed for Person\n");
        return 1;
    }
    memset(person_ptr, 0, sizeof *person_ptr);

    Position* mines = malloc(max_mines * sizeof *mines);
    if (mines == NULL) {
        free(player_ptr);
        free(robot_ptr);
        free(person_ptr);
        endwin();
        fprintf(stderr, "Memory allocation failed for Mines\n");
        return 1;
    }
    memset(mines, 0, max_mines * sizeof *mines);


    CrossObstacle* obstacle_ptr = malloc(sizeof *obstacle_ptr);
    if (obstacle_ptr == NULL) {
        free(player_ptr);
        free(robot_ptr);
        free(person_ptr);
        free(mines);
        endwin();
        fprintf(stderr, "Memory allocation failed for CrossObstacle\n");
        return 1;
    }
    memset(obstacle_ptr, 0, sizeof *obstacle_ptr);

    // initialising stats for obstacle
    obstacle_ptr->width    = 10;
    obstacle_ptr->height   = 10;
    obstacle_ptr->center_x = BOARD_COLS / 2;
    obstacle_ptr->center_y = BOARD_ROWS / 2;

    // Set initial player stats
    player_ptr->lives = 3;
    player_ptr->score = 0;
    player_ptr->level = 1;

    // Set initial robot stats
    robot_ptr->pos.x     = 1 + (BOARD_COLS - 2) / 2; // centre of board
    robot_ptr->pos.y     = 1 + (BOARD_ROWS - 2) / 2; // centre of board
    robot_ptr->direction = 'N';

    // Set initial person position
    person_ptr->x = 1;
    person_ptr->y = 1;

    // Initialise Game struct to zero
    Game game;
    memset(&game, 0, sizeof(Game));

    int mine_count = 0;
    int delay      = 10000000;

    // Show title screen and get player name
    draw_title_screen(player_ptr);
    WINDOW* board = init_game(&game);
    update_UI(player_ptr, robot_ptr, person_ptr, mines, mine_count, &game);

    // Create the game window and draw the arena
    init_game(&game);   // sets field size, creates WINDOW, draws box
    draw_border(&game); // draws the border directly on stdscr too
    mvprintw(1, 2, "Draw Arena. Press any key to quit.");
    refresh();
    // Wait for a key press, then clean up
    while (getch() == -1);

    // Game loop
    while (true) {
        // Handle input
        int input = getch();
        handle_input(robot_ptr, input, person_ptr, mines, &mine_count, &game, obstacle_ptr);


        while (1);

        // TODO: Game logic~
        // Most of your implementation will go here
        reset_game(&game, player_ptr, robot_ptr, person_ptr, mines, &mine_count, obstacle_ptr);
        update_UI(player_ptr, robot_ptr, person_ptr, mines, mine_count, &game);

        // Detect if first key is an arrow key for manual mode
        bool manual_mode = false;
        timeout(3000);   // wait up to 3000 ms for first input
        input = getch(); // returns a key
        timeout(0);      // restore non-blocking
        if (input != ERR && (input == KEY_UP || input == KEY_DOWN || input == KEY_LEFT || input == KEY_RIGHT)) {
            manual_mode = true; // arrow key pressed -> manual mode
        }
        if (input == KEY_UP) {
            robot_ptr->direction = 'N';
        } else if (input == KEY_DOWN) {
            robot_ptr->direction = 'S';
        } else if (input == KEY_LEFT) {
            robot_ptr->direction = 'W';
        } else if (input == KEY_RIGHT) {
            robot_ptr->direction = 'E';
        } else {
            manual_mode = false; // no arrow key pressed -> AI mode
            move_robot_ai(robot_ptr, person_ptr, mines, mine_count, obstacle_ptr);
        }
        nodelay(stdscr, TRUE); // ensure non-blocking

        while (player_ptr->lives > 0) {
            // Clear previous robot position
            clear_robot(&game, robot_ptr);

            // Move robot based on mode
            if (manual_mode) {
                move_robot(robot_ptr, person_ptr, input);
                draw_robot(&game, robot_ptr); // Redraw robot after manual move
                update_UI(player_ptr, robot_ptr, person_ptr, mines, mine_count, &game);

            } else {
                move_robot_ai(robot_ptr, person_ptr, mines, mine_count, obstacle_ptr);
                draw_robot(&game, robot_ptr); // Redraw robot after AI move
                update_UI(player_ptr, robot_ptr, person_ptr, mines, mine_count, &game);
            }

            // Check for collisions
            if (check_collision(&game, player_ptr, robot_ptr, mines, &mine_count, obstacle_ptr, person_ptr) == true) {
                break; // Exit loop if collision results in game over
                player_ptr->lives--;
                update_UI(player_ptr, robot_ptr, person_ptr, mines, mine_count, &game);
                reset_game(&game, player_ptr, robot_ptr, person_ptr, mines, &mine_count, obstacle_ptr);
                // Reset mines
                for (int i = 0; i < mine_count; i++) {
                    clear_mines(&game, &mines[i], mine_count);
                }
            }
            // Spawn additional mines and power-ups based on level
            if (player_ptr->level >= 2) {
                spawn_mines_additional(mines, &mine_count, robot_ptr, person_ptr, &game);
            }
            if (player_ptr->level >= 3) {
                Position power_up;
                power_ups(player_ptr, &power_up, robot_ptr, person_ptr, mines, &mine_count);
            }
            // level up for every 5 points
            if (player_ptr->score != 0 && player_ptr->score % 5 == 0) {
                player_ptr->level = (player_ptr->score / 5) + 1;
            }
            // Redraw robot at new position
            draw_robot(&game, robot_ptr);
            draw_person(&game, person_ptr, robot_ptr, obstacle_ptr);
            draw_obstacle(obstacle_ptr);
            for (int i = 0; i < mine_count; i++) {
                spawn_mines_initial(&mines[i], &mine_count, robot_ptr, person_ptr, &game);
                spawn_mines_additional(&mines[i], &mine_count, robot_ptr, person_ptr, &game);
            }

            // Update UI
            update_UI(player_ptr, robot_ptr, person_ptr, mines, mine_count, &game);
            // Refresh game window
            wrefresh(game.field.window);
            // Small delay to control game speed
            usleep(delay);
        }
        // Check for game over
        if (player_ptr->lives <= 0) {
            game_over_screen(player_ptr);
            save_score(player_ptr);
            show_leaderboard();
            reset_game(&game, player_ptr, robot_ptr, person_ptr, mines, &mine_count, obstacle_ptr);
            // Reset mines
            for (int i = 0; i < mine_count; i++) {
                clear_mines(&game, &mines[i], mine_count);
            }
            break;
        }
        // Update UI
        update_UI(player_ptr, robot_ptr, person_ptr, mines, mine_count, &game);
        // Refresh game window
        wrefresh(game.field.window);
        // Small delay to control game speed
        usleep(delay);

        // Wait for user input before exiting
        nodelay(stdscr, FALSE); // Make getch() blocking
        getch();

        // free allocated memory and window
        if (game.field.window != NULL) {
            delwin(game.field.window);
        }
        free(obstacle_ptr);
        free(mines);
        free(player_ptr);
        free(robot_ptr);
        free(person_ptr);

        // Cleanup and exit
        endwin();
        return 0;
    }
}
