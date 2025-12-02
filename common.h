
#define BOARD_ROWS 20 // You are free to change this
#define BOARD_COLS 50 // You are free to change this
#define MAX_NAME 20
#define ROBOT_BODY 'o'
#define ROBOT_HEAD '^'
#define PERSON 'P'
#define MINE 'X'
#define OBSTACLE '#'
#define POWER_UP '*'
#define max_mines 50


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
typedef struct {
    int rows;
    int cols;
    Position start;
    WINDOW* window;
} Field;

typedef struct {
    Field field;
    // Add other fields from your full Game struct here
} Game;

typedef struct {
    char name[MAX_NAME];
    int score;
} LeaderboardEntry;
