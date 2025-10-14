/* pre-supplied function and type declarations */

char **allocate_2D_array(int rows, int columns);
void deallocate_2D_array(char **array, int rows);
char **load_board(const char *filename, int &height, int &width);
void print_board(char **board, int height, int width);

enum Direction {NORTH, EAST, SOUTH, WEST};

/* add your own function and type declarations here */
bool find_laser(char **board, int height, int width, int& row);
char mirror_label(char **board, int height, int width, int row, int col);
bool shoot(char **board, int height, int width, char* message, int &last_row, int &last_col);
bool solve(char **board, int height, int width, const char* target);