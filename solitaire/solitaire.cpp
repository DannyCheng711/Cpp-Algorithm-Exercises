#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include <string>
#include <vector>

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* helper function for internal use only which gets the dimensions of a board */
bool get_board_dimensions(const char *filename, int &height, int &width) {
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

  if (height > 0)
    return true;
  return false;
}

/* pre-supplied function to load a board from a file*/
char **load_board(const char *filename, int &height, int &width) {

  cout << "Loading puzzle board from '" << filename << "'...";
  
  bool success = get_board_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width + 1);
  
  ifstream input(filename);

  char line[512];

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
  }

  cout << " done (height = " << height << ", width = " << width << ")." << endl;
    
  return m;
}

/* pre-supplied function to print a board */
void print_board(char **board, int height, int width) {
  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    cout << static_cast<char>('A' + c);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(4) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << board[r][c];
    cout << endl;
  }
}

/* add your function definitions below */
bool are_identical(char **first, char **second, int height, int width){
  for (int r=0; r<height; r++){
    for (int c=0; c<width ;c++){
      if(first[r][c] != second[r][c]){
        return false;
      }
    }
  }
  return true;
}

bool make_move(char **board, const char* move, int height, int width){
  // move string should be 3  
  if (string(move).size() != 3) return false;

  char col = move[0];
  // invalid column 
  if (col >= 'A' + width || col < 'A') return false; 
  int row = move[1] - '0';
  // invalide row
  if (row >= height || col < 0) return false; 

  // invalid source marble position  
  if (board[row][col - 'A'] != 'O') return false;

  char direct = move[2];
  if (direct == 'N') {
    // intermediate marble exists and target is empty 
    if (row - 1 >= 0 && board[row - 1][col - 'A'] == 'O' &&
        row - 2 >= 0 && board[row - 2][col - 'A'] == '_' ){
          board[row][col - 'A'] = '_';
          board[row - 1][col - 'A'] = '_';
          board[row - 2][col - 'A'] = 'O';
    }else return false;
  } else if (direct == 'S') {
    // intermediate marble exists and target is empty 
    if (row + 1 < height && board[row + 1][col - 'A'] == 'O' &&
        row + 2 < height && board[row + 2][col - 'A'] == '_' ){
          board[row][col - 'A'] = '_';
          board[row + 1][col - 'A'] = '_';
          board[row + 2][col - 'A'] = 'O';
    }else return false;
  } else if (direct == 'W') {
    // intermediate marble exists and target is empty 
    if (col - 1 >= 'A' && board[row][col - 'A' - 1] == 'O' &&
        col - 2 >= 'A' && board[row][col - 'A' - 2] == '_' ){
          board[row][col - 'A'] = '_';
          board[row][col - 'A' - 1] = '_';
          board[row][col - 'A' - 2] = 'O';
    }else return false;
  } else if (direct == 'E') {
    // intermediate marble exists and target is empty 
    if (col + 1 >= 'A' && board[row][col - 'A' + 1] == 'O' &&
        col + 2 >= 'A' && board[row][col - 'A' + 2] == '_' ){
          board[row][col - 'A'] = '_';
          board[row][col - 'A' + 1] = '_';
          board[row][col - 'A' + 2] = 'O';
    }else return false;
  } else {
    return false;
  }
  
  return true;


}
void copy_board (char **new_board, char **board, int height, int width){
  for (int r = 0; r < height; r++){
    for (int c = 0; c < width ; c++){
      new_board[r][c] = board[r][c];
    }
  }
}

bool helper(int move_count, char **current, char **end, int height, int width, vector<string>& vec_solution, const char* move){
  if (move_count <= 6 && are_identical(current, end, height, width)){
    return true;
  }
  if (move_count > 6) return false;

  vector<char> directions = {'N', 'S', 'E', 'W'};
  
  // try each position and each directions 
  for (char r = '0'; r < '0' + height; r++){
    for (char c = 'A'; c < 'A' + width; c++){
      for (char dir : directions){
        // save current board
        char move[512] = {'\0'};
        int pos = 0;
        move[pos++] = c;  // Add column
        move[pos++] = r;  // Add row
        move[pos++] = dir; // Add direction
        move[pos] = '\0';  // Null-terminate the string
        
        // move = move + c + r + dir;
        char **savedboard = allocate_2D_array(height, width);
        copy_board(savedboard, current, height, width); 
        vec_solution.push_back(string(move));
        if (make_move(current, move, height, width) && 
            (helper(move_count + 1, current, end, height, width, vec_solution, move))){
              deallocate_2D_array(savedboard, height);
          return true;
        }
        // restroe board, solution
        copy_board(current, savedboard, height, width);
        vec_solution.pop_back();
        deallocate_2D_array(savedboard, height);
      } 
    }
  }
  
  return false; 

}

bool find_solution(char **begin, char **end, int height, int width, char solution[]){

  vector<string> vec_solution = {};
  strcpy(solution, "");

  if (helper(0, begin, end, height, width, vec_solution, "")){
    
    for (size_t i = 0; i < vec_solution.size(); i++){
      strcat(solution, vec_solution[i].c_str());
      if (i != vec_solution.size() - 1){
        strcat(solution, ",");
      }
    }
    return true;
  } else {
    return false;
  }
}