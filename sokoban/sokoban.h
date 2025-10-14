#include <cstdint>
// uncomment to activate simple test cases
// #define SIMPLE_TEST

// uncomment to animate solutions (requires make_move)
//#define ANIMATE_SOLUTION

#define SCREEN_HEIGHT 25

#define MAX_SOLUTION_LENGTH 12800

/* pre-supplied function to load a level from a file*/
char **load_level(const char *filename, int &height, int &width);

/* pre-supplied helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **array, int rows);

/* pre-supplied function to print a level */
void print_level(char **level, int height, int width);

/* pre-supplied function to map a level onto an unsigned 64-bit integer */
uint64_t level_hash(char **level, int height, int width);

#ifdef ANIMATE_SOLUTION
void animate_solution(char **level, int height, int width, const char *solution);
#endif

/* place your function prototypes here */
int goal_squares_without_boxes(char **level, int height, int width);
bool find_player(char **level, int height, int width, int &row, int &column); 
bool make_move(char **level, int height, int width, char dir, bool &is_push);
bool solve_level(char **current, int height, int width, char *solution);