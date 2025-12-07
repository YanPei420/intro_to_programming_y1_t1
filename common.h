
#define BOARD_ROWS 20 
#define BOARD_COLS 50 
#define MAX_NAME 20
#define ROBOT_BODY 'o'
#define ROBOT_HEAD '^'
#define PERSON 'P'
#define MINE 'X'
#define OBSTACLE '#'
#define POWER_UP '*'
#define max_mines 50


// Structs
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

typedef struct {
    int rows;
    int cols;
    Position start;
    WINDOW* window;
} Field;

typedef struct {
    Field field;
} Game;

typedef struct {
    char name[MAX_NAME];
    int score;
} LeaderboardEntry;
