#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <string>
#include <cctype>
#include <unistd.h>
#include <set>
#include <unordered_map>
#include <utility>

using namespace std;

// to control aspects of testing and solution animation
// see #define's for SIMPLE_TEST and ANIMATE_SOLUTION in sokoban.h

#include "sokoban.h"

/* pre-supplied helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **array = new char *[rows];
  assert(array);
  for (int r=0; r<rows; r++) {
    array[r] = new char[columns];
    assert(array[r]);
  }
  return array;
}

/* pre-supplied helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **array, int rows) {
  for (int r=0; r<rows; r++)
    delete [] array[r];
  delete [] array;
}

/* internal helper function which gets the dimensions of a level */
bool get_level_dimensions(const char *filename, int &height, int &width) {
  char line[512];
  
  ifstream input(filename);

  height = width = 0;

  input.getline(line,512);  
  while (input) {
    if ( (int) strlen(line) > width)
      width = strlen(line);
    height++;
    input.getline(line,512);  
  }

  //  cout << "get_level_dimensions(...): height = " << height << " width = " << width << endl;
  
  if (height > 0)
    return true;
  return false;
}

/* internal helper function which removes carriage returns and newlines from end of strings */
void filter (char *line) {
  while (*line) {
    if (!isprint(*line))
     *line = '\0';
    line++;
  }
}

/* pre-supplied function to load a level from a file*/
char **load_level(const char *filename, int &height, int &width) {

  cout << "Loading level from '" << filename << "'...";
  bool success = get_level_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **level = allocate_2D_array(height, width + 1);
  
  ifstream input(filename);

  char line[512];

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    filter(line);
    while ( (int) strlen(line) < width) {
      char sp[2] = " ";
      strcat(line, sp);
    }
    strcpy(level[r], line);

  }
  cout << " done (height = " << height << ", width = " << width << ")." << endl;  
  return level;
}

/* pre-supplied function to print a level */
void print_level(char **level, int height, int width) {
  if (width > 10) {
    cout << setw(4) << " " << " ";
    for (int c=0; c<width; c++)
      if (c && (c % 10) == 0) 
	cout << c/10;
      else
	cout << " ";
    cout << endl;
  }

  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(4) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << level[r][c];
    cout << endl;
  }
}

/* pre-supplied function to map a level onto an unsigned 64-bit integer */
uint64_t level_hash(char **level, int height, int width) {
  uint64_t hash = 0, sum = 0;

  int s1 = 0, s2=16;

  for (int r=0; r<height; r++)
    for (int c=0; c<width; c++) {
      sum += level[r][c];
      hash ^= (level[r][c] << s1);
      hash ^= (sum << s2);
      s1 += 3;
      s2 += 5;
      s1 %= 57;  
      s2 %= 57;  
    }
  return hash;
}

#ifdef ANIMATE_SOLUTION
void animate_solution(char **level, int height, int width, const char *solution) {
  print_level(level, height, width);
  bool is_push = false;
  for (unsigned int n=0; n<strlen(solution); n++) {
    cout << "trying " << solution[n] << endl;
    assert(make_move(level, height, width, solution[n], is_push));
    for (int rows=height; rows<SCREEN_HEIGHT; rows++)
      cout << endl;    
    print_level(level, height, width);
    usleep(100000);
  }
}
#endif

/* add your own function definitions here */
int goal_squares_without_boxes(char **level, int height, int width){
  int goal_count_wo_box = 0;
  for (int r = 0 ; r < height; r++){
    for (int c = 0; c < width; c++){
      if (level[r][c] == '.' || level[r][c] == '+'){
        goal_count_wo_box++;
      }
    }
  }
  return goal_count_wo_box;
}

bool find_player(char **level, int height, int width, int &row, int &column){
  for (int r = 0 ; r < height; r++){
    for (int c = 0; c < width; c++){
      if (level[r][c] == '+' || level[r][c] == '@'){
        row = r; 
        column = c;
        return true;
      }
    }
  }
  return false;
}

// Define direction mappings
unordered_map <char, pair<int, int>> directions = {
    {'l', {0, -1}},
    {'r', {0, 1}},
    {'u', {-1, 0}},
    {'d', {1, 0}}
};

bool is_Wall(char **level, int target_row, int target_col, int height, int width){
  
  char cell = level[target_row][target_col];

  // Movable cells: space, goal, box, box on goal
  if (cell == ' ' || cell == '.' || cell == '$' || cell == '*') {
      // Check for a box that may cause a corner deadlock
      if (cell == '$' || cell == '*') {
          bool top_wall = (target_row - 1 < 0 || level[target_row - 1][target_col] == '#');
          bool bottom_wall = (target_row + 1 >= height || level[target_row + 1][target_col] == '#');
          bool left_wall = (target_col - 1 < 0 || level[target_row][target_col - 1] == '#');
          bool right_wall = (target_col + 1 >= width || level[target_row][target_col + 1] == '#');

          // Box against two walls in an L-shape
          if ((top_wall && left_wall) || (top_wall && right_wall) ||
              (bottom_wall && left_wall) || (bottom_wall && right_wall)) {
              return false;
          }
      }

      // Additional checks for other deadlocks can be added here
      return true;
  }

  // Cell is not movable
  return false;

}


bool is_Movable(char cell) {
    return cell == ' ' || cell == '.' || cell == '$' || cell == '*';
}

void update_player_position (char **level, int start_row, int start_col, int new_row, int new_col, char current_player_symbol) {
  if (level[new_row][new_col] == '.') level[new_row][new_col] = '+'; 
  else level[new_row][new_col] = '@'; 
  
  if (current_player_symbol == '+') level[start_row][start_col] = '.';
  else level[start_row][start_col] = ' ';

}

bool is_Pushable (char **level, int height, int width, int start_row, int start_col, int med_row, int med_col, char dir, char current_player_symbol){
  int target_row = med_row + directions[dir].first;
  int target_col = med_col + directions[dir].second;

  if (target_col < 0 || target_col >= width || target_row < 0 || target_row >= height || 
      !(level[target_row][target_col] == '.' || level[target_row][target_col] == ' ')) return false; 

  if (!is_Wall(level, target_row, target_col, height, width)) return false;

  // push box to the left space
  if (level[target_row][target_col] == '.') level[target_row][target_col] = '*';
  else level[target_row][target_col] = '$';

  // update player position
  if (level[med_row][med_col] == '$') level[med_row][med_col] = '@';
  else level[med_row][med_col] = '+';
  if (current_player_symbol == '+') level[start_row][start_col] = '.';
  else level[start_row][start_col] = ' ';

  return true;
}

bool make_move(char **level, int height, int width, char dir, bool &is_push){

  int start_row = -1;
  int start_col = -1;

  if (!find_player(level, height, width, start_row, start_col)) return false;
    
  char current_player_symbol = level[start_row][start_col];
 
  int new_row = start_row + directions[dir].first;
  int new_col = start_col + directions[dir].second;

  // check invalid move
  if (new_col < 0 || new_col >= width || new_row < 0 || new_row >= height || !is_Movable(level[new_row][new_col])) return false; 

  // target is '.' or ' '
  if (level[new_row][new_col] == '.' || level[new_row][new_col] == ' ') {
    update_player_position(level, start_row, start_col, new_row, new_col, current_player_symbol);
    return true;
  }
  
  if (level[new_row][new_col] == '$' || level[new_row][new_col] == '*') {
    if (is_Pushable(level, height, width, start_row, start_col, new_row, new_col, dir, current_player_symbol)){
      is_push = true;
      return true;
    } else return false;
  }

  return false; 

} 

void copy_board(char **current, char **new_current, int height, int width){
  for (int r = 0; r < height; r++){
    for (int c = 0; c < width; c++){
      new_current[r][c] = current[r][c]; 
    }
  }
}

bool helper(char **current, int height, int width, char *solution, int &sol_length, set<uint64_t> &history, bool is_push) {

  uint64_t current_hash = level_hash(current, height, width);
  // base case
  if (goal_squares_without_boxes(current, height, width) == 0) return true;
  if (strlen(solution) >= MAX_SOLUTION_LENGTH ) return false;

  if (history.find(current_hash) != history.end()) return false;

  
  
  cout << "solution: " << solution << endl; 
  //// cout << "=========" << endl;
  // try each direction
  for (const auto& entry: directions) {
    
    char dir = entry.first;
    // Save the current `is_push` state
    bool previous_is_push = is_push;

    //// cout << "direction: " << dir << endl;

    // Append the current move to the solution
    solution[sol_length] = dir;
    sol_length++; 
    solution[sol_length] = '\0';
    history.insert(current_hash);
    char **current_copy = allocate_2D_array(height, width);
    copy_board(current, current_copy, height, width);

    if (make_move(current, height, width, dir, is_push) && (helper(current, height, width, solution, sol_length, history, is_push))){ 
        deallocate_2D_array(current_copy, height);
        return true;
    }

    // restore is_push
    is_push = previous_is_push;
    // restore solution
    sol_length--;
    solution[sol_length] = '\0';
    // restore history record
    history.erase(current_hash);
    // restore board
    copy_board(current_copy, current, height, width);
    deallocate_2D_array(current_copy, height);
  }

    
  // drurrdlulldrrR

  return false;

}

bool solve_level(char **current, int height, int width, char *solution){

  set<uint64_t> history = {};
  int sol_length = 0;

  return helper(current, height, width, solution, sol_length, history, false);

}


