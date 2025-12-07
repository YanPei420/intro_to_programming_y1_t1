#include <ncurses.h> // Make sure this is correct for your OS
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "common.h"

// Function prototypes
WINDOW* init_game(Game* game);
void draw_title_screen(Player* player);
void update_UI(Player* player, Robot* robot, Position* person, Position* mines, int mine_count, Game* game);
void handle_input(Robot* robot, int input, Position* person, Position* mines, int* mine_count, Game* game, CrossObstacle* obstacle);
void move_robot(Robot* robot, Position* person);
void move_robot_ai(Robot* robot, Position* person, Position* mines, int mine_count, CrossObstacle* obstacle);
bool collides_at(int x, int y, Position* mines, int mine_count, CrossObstacle* obstacle);
void clear_robot(Game* game, Robot* robot);
void draw_robot(Game* game, Robot* robot);
bool check_collision(Game* game, Player* player, Robot* robot, Position* mines, int* mine_count, CrossObstacle* obstacle, Position* person);
void draw_person(Game* game, Position* person, Robot* robot, CrossObstacle* obstacle);
void spawn_mines_initial(Position* mines, int* mine_count, Robot* robot, Position* person, Game* game);
void spawn_mines_additional(Position* mines, int* mine_count, Robot* robot, Position* person, Game* game);
void power_ups(Player* player, Position* power_up, Robot* robot, Position* person, Position* mines, int* mine_count);
void game_over_screen(Player* player);
void save_score(Player* player);
void show_leaderboard(void);
void draw_obstacle(CrossObstacle* obstacle);
void draw_border(Game* game);
void draw_person(Game* game, Position* person, Robot* robot, CrossObstacle* obstacle);
bool is_obstacle_position(CrossObstacle* obstacle, int x, int y);
void activate_power_up(void);
void deactivate_power_up(void);
void clear_person(Game* game, Position* person);
void reset_game(Game* game, Player* player, Robot* robot, Position* person, Position* mines, int* mine_count, CrossObstacle* obstacle);
static void board_to_screen(Game* game, int bx, int by, int* sx, int* sy);
static void robot_head_pos(Robot* robot, int* hx, int* hy);
static void check_cell_collision(Game* game, Player* player, Robot* robot, Position* mines, int* mine_count, CrossObstacle* obstacle, Position* person, int x, int y, bool* life_lost);

/* Helper: convert board coords (bx,by) to absolute screen coords (sx,sy) */
static void board_to_screen(Game* game, int bx, int by, int* sx, int* sy) {
    if (game == NULL || sx == NULL || sy == NULL) return;
    *sy = game->field.start.y + by;
    *sx = game->field.start.x + bx;
}

/* Helper: compute robot head board coords from robot.direction */
static void robot_head_pos(Robot *robot, int *hx, int *hy) {
    if (robot == NULL || hx == NULL || hy == NULL) return;
    *hx = robot->pos.x;
    *hy = robot->pos.y;
    if (robot->direction == KEY_UP)    *hy = robot->pos.y - 1;
    else if (robot->direction == KEY_DOWN)  *hy = robot->pos.y + 1;
    else if (robot->direction == KEY_LEFT)  *hx = robot->pos.x - 1;
    else if (robot->direction == KEY_RIGHT) *hx = robot->pos.x + 1;
}

void draw_title_screen(Player* player) {
    if (player == NULL) return;

    mvaddstr(1, 1, "ROBOT RESCUE MISSION\n");
    mvaddstr(3, 1, "Instructions:\n");
    mvaddstr(4, 1, "Use arrow keys to move the robot.\n");
    mvaddstr(5, 1, "Rescue the person (P) while avoiding mines (X) and obstacles (#).\n");
    mvaddstr(6, 1, "Power-ups (*) may appear to help you when you reach level 3!\n");
    mvaddstr(7, 1, "Press 'm' to start the game...\n");
    refresh();

    while (getch() == -1); // Wait for any key press
}

void update_UI(Player* player, Robot* robot, Position* person, Position* mines, int mine_count, Game* game) {
    mvwprintw(stdscr, 1, 1, "Player: %s", player->name);
    mvwprintw(stdscr, 1, 11, "Lives: %d", player->lives);
    mvwprintw(stdscr, 1, 20, "Score: %d", player->score);
    mvwprintw(stdscr, 1, 30, "Level: %d", player->level);
    refresh();
}

void handle_input(Robot* robot, int input, Position* person, Position* mines, int* mine_count, Game* game, CrossObstacle* obstacle) {
    // Handle arrow key inputs
    switch (input) {
    case KEY_UP:
    case KEY_DOWN:
    case KEY_LEFT:
    case KEY_RIGHT:
        robot->direction = input;
    }
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

    int center_x = obstacle->center_x;
    int center_y = obstacle->center_y;

    int west  = center_x - W / 2;
    int east  = center_x + W / 2; //-1 to account for 0 indexing
    int north = center_y - H / 2;
    int south = center_y + H / 2; //-1 to account for 0 indexing
    // add colours
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    // Draw horizontal part
    for (int x = west; x <= east; x++) {
        int sx = startx + x;
        int sy = starty + center_y;
        mvaddch(sy, sx, OBSTACLE);
    }
    // draw vertical part
    for (int y = north; y <= south; y++) {
        int sx = startx + center_x;
        int sy = starty + y;
        mvaddch(sy, sx, OBSTACLE);
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
    int east  = center_x + W / 2; //-1 to account for 0 indexing
    int north = center_y - H / 2;
    int south = center_y + H / 2; //-1 to account for 0 indexing

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

void move_robot(Robot* robot, Position* person) {
    if (robot == NULL || person == NULL) return;

    switch (robot->direction) {
    case KEY_UP:
        robot->direction = KEY_UP;
        if (robot->pos.y > 0)
            robot->pos.y--;
        break;
    case KEY_DOWN:
        robot->direction = KEY_DOWN;
        if (robot->pos.y < BOARD_ROWS - 1)
            robot->pos.y++;
        break;
    case KEY_LEFT:
        robot->direction = KEY_LEFT;
        if (robot->pos.x > 0)
            robot->pos.x--;
        break;
    case KEY_RIGHT:
        robot->direction = KEY_RIGHT;
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

    int dx = 0, dy = 0; // direction x and y
    int nx, ny;         // new x and y

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
        robot->direction = KEY_UP;
        return;
    }

    // Try East
    nx = robot->pos.x + 1;
    ny = robot->pos.y;
    if (collides_at(nx, ny, mines, mine_count, obstacle)) {
        // blocked, try next direction
    } else {
        robot->pos.x     = nx;
        robot->direction = KEY_RIGHT;
        return;
    }

    // Try South
    nx = robot->pos.x;
    ny = robot->pos.y + 1;
    if (collides_at(nx, ny, mines, mine_count, obstacle)) {
        // blocked, try next direction
    } else {
        robot->pos.y     = ny;
        robot->direction = KEY_DOWN;
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
        robot->direction = KEY_LEFT;
        return;
    }
}

static void check_cell_collision(Game* game, Player* player, Robot* robot, Position* mines, int* mine_count, CrossObstacle* obstacle, Position* person, int x, int y, bool* life_lost) {
    if (game == NULL || player == NULL || robot == NULL || mines == NULL || mine_count == NULL || obstacle == NULL || person == NULL || life_lost == NULL) {
        return;
    }

    {
    if (life_lost == NULL) return;

    /* Wall */
    if (x <= 0 || x >= BOARD_COLS - 1 || y <= 0 || y >= BOARD_ROWS - 1) {
        *life_lost = true;
        return;
    }

    /* Obstacle */
    if (is_obstacle_position(obstacle, x, y)) {
        *life_lost = true;
        return;
    }

    /* Mines */
    if (mines != NULL && mine_count != NULL) {
        for (int i = 0; i < *mine_count; ++i) {
            if (mines[i].x == x && mines[i].y == y) {
                /* remove the mine by shifting the array down */
                for (int j = i; j < *mine_count - 1; ++j) {
                    mines[j] = mines[j + 1];
                }
                (*mine_count)--;
                *life_lost = true;
                return;
            }
        }
    }

    /* Person (rescue) */
    if (person != NULL && person->x == x && person->y == y) {
        player->score = player->score + 1;
        person->x = -1;
        person->y = -1;
        return;
    }
}

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



void clear_robot(Game* game, Robot* robot) {
    if (game == NULL || robot == NULL) return;

    int bx = robot->pos.x;
    int by = robot->pos.y;
    int sx, sy;

    /* Clear body if inside playable area (1..cols-2, 1..rows-2) */
    if (bx >= 1 && bx <= game->field.cols - 2 && by >= 1 && by <= game->field.rows - 2) {
        board_to_screen(game, bx, by, &sx, &sy);
        mvaddch(sy, sx, ' ');
    }

    /* Clear head */
    int hx, hy;
    robot_head_pos(robot, &hx, &hy);
    if (hx >= 1 && hx <= game->field.cols - 2 && hy >= 1 && hy <= game->field.rows - 2) {
        board_to_screen(game, hx, hy, &sx, &sy);
        mvaddch(sy, sx, ' ');
    }
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

void draw_robot(Game* game, Robot* robot) {
    if (game == NULL || robot == NULL) return;

    int bx = robot->pos.x;
    int by = robot->pos.y;
    int sx, sy;

    /* draw body */
    board_to_screen(game, bx, by, &sx, &sy);
    if (has_colors()) attron(COLOR_PAIR(1));
    mvaddch(sy, sx, ROBOT_BODY);
    if (has_colors()) attroff(COLOR_PAIR(1));

    /* draw head using same colour pair so it visually matches and indicates occupancy */
    int hx, hy;
    robot_head_pos(robot, &hx, &hy);
    if (hx >= 1 && hx <= game->field.cols - 2 && hy >= 1 && hy <= game->field.rows - 2) {
        board_to_screen(game, hx, hy, &sx, &sy);
        if (has_colors()) attron(COLOR_PAIR(1));
        char head_ch = '^';
        switch (robot->direction) {
        case KEY_UP: head_ch = '^'; break;
        case KEY_DOWN: head_ch = 'v'; break;
        case KEY_LEFT: head_ch = '<'; break;
        case KEY_RIGHT: head_ch = '>'; break;
        default: break;
        }
        mvaddch(sy, sx, head_ch);
        if (has_colors()) attroff(COLOR_PAIR(1));
    }
    refresh();
}

/*
 * Enhanced collision checker:
 * - Treats both body and head as occupied cells for collision/interaction.
 * - If body/head hits wall/obstacle/mine: decrement life and remove mine if hit.
 * - If body/head hits person: award a point, respawn person and trigger level logic elsewhere.
 *
 * Returns true if a collision that costs a life occurred (wall/mine/obstacle).
 */
bool check_collision(Game* game, Player* player, Robot* robot, Position* mines, int* mine_count, CrossObstacle* obstacle, Position* person) {
    if (game == NULL || player == NULL || robot == NULL || mines == NULL || mine_count == NULL || person == NULL) return false;

    bool life_lost = false;

    /* check a single cell (x,y) */
    auto do_check = [&](int x, int y) {
        // wall
        if (x <= 0 || x >= BOARD_COLS - 1 || y <= 0 || y >= BOARD_ROWS - 1) {
            life_lost = true;
            return;
        }
        // obstacle
        if (is_obstacle_position(obstacle, x, y)) {
            life_lost = true;
            return;
        }
        // mines
        for (int i = 0; i < *mine_count; ++i) {
            if (mines[i].x == x && mines[i].y == y) {
                // remove mine by shifting
                for (int j = i; j < *mine_count - 1; ++j) mines[j] = mines[j + 1];
                (*mine_count)--;
                life_lost = true;
                return;
            }
        }
        // person / rescue
        if (person->x == x && person->y == y) {
            player->score++;
            person->x = -1;
            person->y = -1;
            return;
        }
    };

    // check body
    do_check(robot->pos.x, robot->pos.y);
    // check head
    int hx, hy;
    robot_head_pos(robot, &hx, &hy);
    do_check(hx, hy);

    // update UI numbers
    mvprintw(1, 1, "Player: %s", player->name);
    mvprintw(1, 12, "Lives: %d", player->lives);
    mvprintw(1, 23, "Score: %d", player->score);
    mvprintw(1, 35, "Level: %d", player->level);
    refresh();

    if (life_lost) {
        player->lives--;
        return true;
    }
    return false;
}



    /* Update UI display of stats (lives, score, level) */
    mvprintw(1, 1, "Player: %s", player->name);
    mvprintw(1, 12, "Lives: %d", player->lives);
    mvprintw(1, 23, "Score: %d", player->score);
    mvprintw(1, 35, "Level: %d", player->level);
    refresh();

    if (life_lost) {
        player->lives--;
        return true;
    }
    return false;
}

// drawing the 5 mines at the start of the game
void spawn_mines_initial(Position* mines, int* mine_count, Robot* robot, Position* person, Game* game) {
    if (mines == NULL || mine_count == NULL || robot == NULL || person == NULL || game == NULL) {
        return;
    }

    const int want = 5;
    const int max_tries = 500;

    while (*mine_count < want && *mine_count < max_mines) {
        int tries = 0;
        bool placed = false;
        while (tries < max_tries) {
            int rx = rand() % (game->field.cols - 2) + 1;
            int ry = rand() % (game->field.rows - 2) + 1;

            bool conflict = false;
            // avoid placing on person
            if (person && person->x == rx && person->y == ry) conflict = true;
            // avoid placing on robot
            if (robot && robot->pos.x == rx && robot->pos.y == ry) conflict = true;
            // avoid existing mines
            for (int j = 0; j < *mine_count && !conflict; ++j) {
                if (mines[j].x == rx && mines[j].y == ry) conflict = true;
            }

            if (!conflict) {
                mines[*mine_count].x = rx;
                mines[*mine_count].y = ry;
                // draw mine on board
                int sx, sy;
                board_to_screen(game, rx, ry, &sx, &sy);
                if (has_colors()) attron(COLOR_PAIR(4));
                mvaddch(sy, sx, MINE);
                if (has_colors()) attroff(COLOR_PAIR(4));
                (*mine_count)++;
                placed = true;
                break;
            }
            tries++;
        }
        if (!placed) break; // cannot place more safely
    }
}

void spawn_mines_additional(Position* mines, int* mine_count, Robot* robot, Position* person, Game* game) {
    if (mines == NULL || mine_count == NULL || robot == NULL || person == NULL || game == NULL) return;

    const int add = 2;           /* two mines per level increment */
    const int max_tries = 200;

    for (int a = 0; a < add && *mine_count < max_mines; ++a) {
        int tries = 0;
        bool placed = false;
        while (tries < max_tries) {
            int rx = rand() % (game->field.cols - 2) + 1;
            int ry = rand() % (game->field.rows - 2) + 1;
            bool conflict = false;

            if (robot && robot->pos.x == rx && robot->pos.y == ry) conflict = true;
            if (person && person->x == rx && person->y == ry) conflict = true;
            if (is_obstacle_position(NULL, rx, ry)) {
                /* conservative: skip if you cannot check obstacle here */
            }
            for (int j = 0; j < *mine_count && !conflict; ++j) {
                if (mines[j].x == rx && mines[j].y == ry) conflict = true;
            }

            if (!conflict) {
                mines[*mine_count].x = rx;
                mines[*mine_count].y = ry;
                int sx, sy;
                board_to_screen(game, rx, ry, &sx, &sy);
                if (has_colors()) attron(COLOR_PAIR(4));
                mvaddch(sy, sx, MINE);
                if (has_colors()) attroff(COLOR_PAIR(4));
                (*mine_count)++;
                placed = true;
                break;
            }
            tries++;
        }
        if (!placed) break;
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

void save_score(Player* player) {
    // Save score to leaderboard.txt
    FILE* file = fopen("leaderboard.txt", "a");
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

void show_leaderboard(void) {
    FILE* file = fopen("leaderboard.txt", "r");
    if (file == NULL) {
        clear();
        mvprintw(5, 5, "No leaderboard found (leaderboard.txt missing).");
        mvprintw(7, 5, "Press any key to continue...");
        refresh();
        nodelay(stdscr, FALSE);
        getch();
        nodelay(stdscr, TRUE);
        return;
    }

    LeaderboardEntry top[10];
    int num_entries = 0;
    char name[MAX_NAME];
    int score;

    while (fscanf(file, "%19s %d", name, &score) == 2) {
        int insert_at = 0;
        while (insert_at < num_entries && insert_at < 10) {
            if (top[insert_at].score >= score) insert_at++;
            else break;
        }

        if (insert_at < 10) {
            int last = num_entries;
            if (last > 9) last = 9;
            for (int i = last; i > insert_at; --i) {
                top[i] = top[i - 1];
            }
            strncpy(top[insert_at].name, name, MAX_NAME - 1);
            top[insert_at].name[MAX_NAME - 1] = '\0';
            top[insert_at].score = score;
            if (num_entries < 10) num_entries++;
        }
    }
    fclose(file);

    clear();
    mvprintw(1, 5, "Leaderboard - Top 10 Scores");
    if (num_entries == 0) {
        mvprintw(3, 5, "No scores recorded yet.");
    } else {
        for (int i = 0; i < num_entries; ++i) {
            mvprintw(3 + i, 5, "%2d. %-*s %6d", i + 1, MAX_NAME - 1, top[i].name, top[i].score);
        }
    }
    mvprintw(15, 5, "Press any key to return to the game.");
    refresh();
    nodelay(stdscr, FALSE);
    getch();
    nodelay(stdscr, TRUE);
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
    if (game == NULL || person == NULL) return;

    // pick a free random spot only when person coords are invalid
    if (person->x == -1 || person->y == -1) {
        const int max_tries = 200;
        int tries = 0;
        while (tries < max_tries) {
            int rx = rand() % (game->field.cols - 2) + 1;
            int ry = rand() % (game->field.rows - 2) + 1;

            bool bad = false;
            if (robot && robot->pos.x == rx && robot->pos.y == ry) bad = true;
            if (is_obstacle_position(obstacle, rx, ry)) bad = true;
            // avoid existing mines if you want (optional: check mines array if available)

            if (!bad) {
                person->x = rx;
                person->y = ry;
                break;
            }
            tries++;
        }
    }

    // draw using board_to_screen -> mvaddch so coordinates map correctly to stdscr
    int sx, sy;
    board_to_screen(game, person->x, person->y, &sx, &sy);
    if (has_colors()) attron(COLOR_PAIR(2));
    mvaddch(sy, sx, PERSON);
    if (has_colors()) attroff(COLOR_PAIR(2));
    refresh();
}

void reset_game(Game* game, Player* player, Robot* robot, Position* person, Position* mines, int* mine_count, CrossObstacle* obstacle) {
    if (player == NULL || robot == NULL || person == NULL || mines == NULL || game == NULL) {
        return;
    }

    // Reset player stats
    player->lives = 3;
    player->score = 0;
    player->level = 1;

    clear_robot(game, robot);
    int home_x = 1 + (BOARD_COLS - 2) / 4;
    int home_y = 1 + (BOARD_ROWS - 2) / 4;
    robot->pos.x = home_x;
    robot->pos.y = home_y;
    robot->direction = 0; // do not default to KEY_UP (prevents immediate upward motion)

    draw_robot(game, robot);

    // Reset person
    person->x = rand() % (BOARD_COLS - 2) + 1;
    person->y = rand() % (BOARD_ROWS - 2) + 1;
    draw_person(game, person, robot, obstacle);

    // obstacle
    int ymax, xmax;
    getmaxyx(stdscr, ymax, xmax);
    obstacle->center_x = BOARD_COLS / 2;
    obstacle->center_y = BOARD_ROWS / 2;
    obstacle->width = 10;
    obstacle->height = 10;
    draw_obstacle(obstacle);

    *mine_count = 0;
    spawn_mines_initial(mines, mine_count, robot, person, game);
}

/* --- Main loop and level handling fixes (replace the part in main where you decide manual/AI,
   move the robot and handle level-up).
// ...existing code...*/

static void game_loop(Game* game, Player* player, Robot* robot, Position* person, Position* mines, int* mine_count, CrossObstacle* obstacle) {
    if (game == NULL || player == NULL || robot == NULL || person == NULL || mines == NULL || mine_count == NULL || obstacle == NULL) return;

    const int WAIT_MS          = 3000; /* wait for first arrow after 'm' pressed */
    const int INITIAL_DELAY_MS = 120;  /* frame delay in milliseconds */
    const int MIN_DELAY_MS     = 12;   /* minimum delay in ms */
    int delay_ms               = INITIAL_DELAY_MS;

    /* ensure starting values */
    player->lives = 3;
    if (player->level < 1) player->level = 1;
    if (player->score < 0) player->score = 0;

    /* place initial mines and person */
    spawn_mines_initial(mines, mine_count, robot, person, game);
    if (person->x == -1 || person->y == -1) {
        draw_person(game, person, robot, obstacle);
    }

    /* Determine initial control mode: wait up to WAIT_MS for an arrow key.
       Use variable name `input` as requested. */
    timeout(WAIT_MS);    /* getch blocks up to WAIT_MS ms for first input */
    int input = getch(); /* key code or ERR */
    timeout(0);          /* restore non-blocking mode */
    bool manual_mode = false;

    /* Only switch to manual if an arrow was pressed. Otherwise AI mode. */
    if (input == KEY_UP || input == KEY_DOWN || input == KEY_LEFT || input == KEY_RIGHT) {
        manual_mode      = true;
        robot->direction = input; /* robot will move in this direction each tick */
    } else {
        manual_mode = false;
        /* keep robot->direction as-is; AI will move robot */
    }

    /* use non-blocking input during the loop */
    nodelay(stdscr, TRUE);

    /* main loop */
    while (player->lives > 0) {
        /* read current input (non-blocking) into variable named `input` */
        input = getch(); /* returns ERR if nothing was pressed */

        /* if arrow pressed at any time, switch to manual and update direction */
        if (input == KEY_UP || input == KEY_DOWN || input == KEY_LEFT || input == KEY_RIGHT) {
            manual_mode      = true;
            robot->direction = input;
        }

        /* clear previous robot drawing */
        clear_robot(game, robot);

        /* Movement: manual keeps moving in robot->direction (if set),
           AI else. Manual does NOT move if robot->direction is unset (0). */
        if (manual_mode) {
            /* ensure direction is a known arrow */
            if (robot->direction == KEY_UP || robot->direction == KEY_DOWN ||
            robot->direction == KEY_LEFT || robot->direction == KEY_RIGHT) {

                int nx = robot->pos.x;
                int ny = robot->pos.y;

                if (robot->direction == KEY_UP)
                    ny = robot->pos.y - 1;
                else if (robot->direction == KEY_DOWN)
                    ny = robot->pos.y + 1;
                else if (robot->direction == KEY_LEFT)
                    nx = robot->pos.x - 1;
                else if (robot->direction == KEY_RIGHT)
                    nx = robot->pos.x + 1;

                /* If candidate collides, do not move into blocked cell here.
                   check_collision below will handle life decrement and reset. */
                if (!collides_at(nx, ny, mines, *mine_count, obstacle)) {
                    robot->pos.x = nx;
                    robot->pos.y = ny;
                } else {
                    /* keep heading but don't move into blocked cell */
                }
            }
        } else {
            /* AI mode chooses a safe move (move_robot_ai avoids collisions) */
            move_robot_ai(robot, person, mines, *mine_count, obstacle);
        }

        /* After move, check for collisions and interactions (body+head) */
        bool life_event = check_collision(game, player, robot, mines, mine_count, obstacle, person);

        if (life_event) {
            /* robot hit wall/obstacle/mine: reset robot to safe home */
            robot->pos.x     = 1 + (game->field.cols - 2) / 4;
            robot->pos.y     = 1 + (game->field.rows - 2) / 4;
            robot->direction = 0; /* clear direction so robot doesn't auto-move */
            /* update UI below */
            if (player->lives <= 0) break;
        }

        /* If person was collected (check_collision marks person->x/y = -1), respawn person */
        if (person->x == -1 && person->y == -1) {
            /* spawn/ draw new person */
            draw_person(game, person, robot, obstacle);

            /* Level handling: level increases by 1 for every 5 points.
               If player->score advanced multiple thresholds, perform repeated level-ups. */
            int new_level = 1 + (player->score / 5);
            while (player->level < new_level) {
                player->level = player->level + 1;
                /* spawn exactly 2 more mines per level */
                spawn_mines_additional(mines, mine_count, robot, person, game);
                /* increase speed by 2 ms per level (reduce delay) */
                delay_ms = delay_ms - 2;
                if (delay_ms < MIN_DELAY_MS) delay_ms = MIN_DELAY_MS;
            }
        }

        /* Power-ups available after level 3 */
        if (player->level >= 3) {
            power_ups(player, NULL, robot, person, mines, mine_count);
        }

        /* draw frame: redraw inner board area, mines, person, robot, obstacle */
        /* clear inner board area (1 .. cols-2, 1 .. rows-2) */
        for (int by = 1; by < game->field.rows - 1; ++by) {
            for (int bx = 1; bx < game->field.cols - 1; ++bx) {
                int sx, sy;
                board_to_screen(game, bx, by, &sx, &sy);
                mvaddch(sy, sx, ' ');
            }
        }

        /* draw mines */
        for (int i = 0; i < *mine_count; ++i) {
            int sx, sy;
            board_to_screen(game, mines[i].x, mines[i].y, &sx, &sy);
            if (has_colors()) attron(COLOR_PAIR(4));
            mvaddch(sy, sx, MINE);
            if (has_colors()) attroff(COLOR_PAIR(4));
        }

        /* draw person */
        if (person->x >= 0 && person->y >= 0) {
            int sx, sy;
            board_to_screen(game, person->x, person->y, &sx, &sy);
            if (has_colors()) attron(COLOR_PAIR(2));
            mvaddch(sy, sx, PERSON);
            if (has_colors()) attroff(COLOR_PAIR(2));
        }
        draw_obstacle(obstacle);
        draw_robot(game, robot);
        update_UI(player, robot, person, mines, *mine_count, game);

        /* frame delay (ms -> us) */
        if (delay_ms > 0) {
            usleep((useconds_t)delay_ms * 1000);
        } else {
            usleep(10000);
        }
    } /* end main while */

    // Game over
    nodelay(stdscr, FALSE); /* make getch blocking for menus */
    game_over_screen(player);
    save_score(player);
    show_leaderboard();
    nodelay(stdscr, TRUE); /* restore non-blocking if caller expects it */
}

// free allocated memory and window
if (game.field.window != NULL) {
    endwin();
}
free(obstacle_ptr);
free(mines);
free(player_ptr);
free(robot_ptr);
free(person_ptr);
// Cleanup and exit
endwin();
return 0;