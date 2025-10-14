#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include "laser.h"

using namespace std;

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

/* internal helper function which gets the dimensions of a board */
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

/* internal helper function which removes carriage returns and newlines from end of strings */
void filter (char *line) {
  while (*line) {
    if (!isprint(*line))
     *line = '\0';
    line++;
  }
}

/* pre-supplied function to load a board from a file*/
char **load_board(const char *filename, int &height, int &width) {

  bool success = get_board_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **board = allocate_2D_array(height, width + 1);
  
  ifstream input(filename);

  char line[512];

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    filter(line);
    strcpy(board[r], line);
  }
  
  return board;
}

/* pre-supplied function to print a board */
void print_board(char **board, int height, int width) {
  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(4) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << board[r][c];
    cout << endl;
  }
}

/* add your own function definitions here */
bool find_laser(char **board, int height, int width, int& row){
  
  for (int r = 0; r < height; r++){
    if (board[r][0] == '>'){
      row = r; 
      return true;
    }
  }
  return false;
}

char mirror_label(char **board, int height, int width, int row, int col){
  
  char cell = board[row][col];

  if (cell == '/') {
    if (row + 1 < height && col - 1 > 0 && isalpha(board[row + 1][col - 1])){
      return board[row + 1][col - 1];
    }else if (row - 1 > 0 && col + 1 < width && isalpha(board[row - 1][col + 1])) {
      return board[row - 1][col + 1];
    } else {
      return '\0';
    }
  }

  if (cell == '\\') {
    if (row - 1 > 0 && col - 1 > 0 && isalpha(board[row - 1][col - 1])){
      return board[row - 1][col - 1];
    }else if (row + 1 < height && col + 1 < width && isalpha(board[row + 1][col + 1])) {
      return board[row + 1][col + 1];
    } else {
      return '\0';
    }
  }

  return '\0';

}

void get_new_direction(char &direction, const char mirror, char &beam){
  if (mirror == '\\'){
    if (direction == 'r'){
      beam = '|'; 
      direction = 'd'; // down
    }
    else if (direction == 'l'){
      beam = '|'; 
      direction = 'u'; // up
    }
    else if (direction == 'd'){
      beam = '-';
      direction = 'r';
    }
    else if (direction == 'u'){
      beam = '-';
      direction = 'l';
    }
  }else{
    if (direction == 'r'){
      beam = '|'; 
      direction = 'u'; // up
    }
    else if (direction == 'l'){
      beam = '|'; 
      direction = 'd'; // down
    }
    else if (direction == 'd'){
      beam = '-'; 
      direction = 'l'; // left
    }
    else if (direction == 'u'){
      beam = '-'; 
      direction = 'r'; // right
    }
  }

}

bool shoot(char **board, int height, int width, char* message, int &last_row, int &last_col){

  last_col = 0;

  if (!find_laser(board, height, width, last_row)) {
    return false; // Laser not found, return false
  }

  char beam = '-';
  char direction = 'r'; // right
  int msg_idx = 0;
  char label;

  while(board[last_row][last_col] != '@' && board[last_row][last_col] != '?' && board[last_row][last_col] != '#'){

    if (board[last_row][last_col] == '\\' || board[last_row][last_col] == '/'){

      label = mirror_label(board, height, width, last_row, last_col);
      if (label != '\0'){
        message[msg_idx] = label;
        msg_idx += 1;
      }

      get_new_direction(direction, board[last_row][last_col], beam);
      
    }

    if (direction == 'r') last_col += 1;
    else if (direction == 'l') last_col -= 1;
    else if (direction == 'd') last_row += 1;
    else if (direction == 'u') last_row -= 1;

    if (board[last_row][last_col] == ' '){
      board[last_row][last_col] = beam;
    }
  }

  message[msg_idx] = '\0'; 
  return board[last_row][last_col] == '@';
}

char** copy_board(char** board, int height, int width) {
  char** new_board = allocate_2D_array(height, width);
  for (int r = 0; r < height; r++){
     for (int c = 0; c < width; c++){
        new_board[r][c] = board[r][c];
     }
  }

  return new_board;
}

void restore_board(char** board, char** saved_board, int height, int width) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            board[i][j] = saved_board[i][j];
        }
    }
}

bool helper_solver(char **board, int height, int width, char* message, int &row, int &col, char direction, const char* target, char beam){
  
  char label;

  while(true){

    char cell = board[row][col];

    // base case
    if (cell == '@'){
      
      message[strlen(message)] = '\0';
      cout << message << endl;
      cout << target << endl;
      if (!strcmp(message, target)) return true;
      else return false;
    }

    if (cell == '#'){
      message[strlen(message)] = '\0';
      return false; 
    }

    if (cell == '/' || cell == '\\'){
      label = mirror_label(board, height, width, row, col);
      if (label != '\0'){
        message[strlen(message)] = label;
        message[strlen(message) + 1] = '\0';
      }

      get_new_direction(direction, board[row][col], beam);
    }

    if (cell == '?') {
      // save current state for backtracking
      int saved_row = row; 
      int saved_col = col;
      char saved_dir = direction;
      char saved_beam = beam;
      // Deep copy the board
      char** saved_board = copy_board(board, height, width);
      char saved_message[512];
      strcpy(saved_message, message);

      // treat ? as ' ';
      board[row][col] = ' ';
      // recursive
      if (direction == 'r') col += 1;
      else if (direction == 'l') col -= 1;
      else if (direction == 'd') row += 1;
      else if (direction == 'u') row -= 1;

      if (helper_solver(
        board, height, width, message, row, col, direction, target, beam)){
        return true;
      }

      row = saved_row;
      col = saved_col;
      direction = saved_dir;
      beam = saved_beam;
      strcpy(message, saved_message);

      // Restore the board
      restore_board(board, saved_board, height, width);
      
      // treat ? as '/'
      board[row][col] = '/';
      label = mirror_label(board, height, width, row, col);
      if (label != '\0'){
        message[strlen(message)] = label;
        message[strlen(message) + 1] = '\0';
      }
      get_new_direction(direction, '/', beam);
      // recursive
      if (direction == 'r') col += 1;
      else if (direction == 'l') col -= 1;
      else if (direction == 'd') row += 1;
      else if (direction == 'u') row -= 1;

      if (helper_solver(
        board, height, width, message, row, col, direction, target, beam)){
        return true;
      }

      row = saved_row;
      col = saved_col;
      direction = saved_dir;
      beam = saved_beam;
      strcpy(message, saved_message);
      
      // Restore the board
      restore_board(board, saved_board, height, width);
      
      // treat ? as '\\'
      board[row][col] = '\\';
      label = mirror_label(board, height, width, row, col);
      if (label != '\0'){
        cout << "label: " << label << endl;
        message[strlen(message)] = label;
        message[strlen(message) + 1] = '\0';
      }
      get_new_direction(direction, '\\', beam);

      // recursive
      if (direction == 'r') col += 1;
      else if (direction == 'l') col -= 1;
      else if (direction == 'd') row += 1;
      else if (direction == 'u') row -= 1;

      if (helper_solver(
        board, height, width, message, row, col, direction, target, beam)){
        return true;
      }

      row = saved_row;
      col = saved_col;
      direction = saved_dir;
      beam = saved_beam;
      strcpy(message, saved_message);

      // Restore the board
      restore_board(board, saved_board, height, width);

      deallocate_2D_array(saved_board, height);
      
      return false;

    }

    if (direction == 'r') col += 1;
    else if (direction == 'l') col -= 1;
    else if (direction == 'd') row += 1;
    else if (direction == 'u') row -= 1;

    if (board[row][col] == ' '){
      board[row][col] = beam;
    }
  }
}

bool solve(char **board, int height, int width, const char* target){

  char message[512] = {'\0'};
  int row = 0;
  int col = 0;
  char direction = 'r';
  char beam = '-';

  if (!find_laser(board, height, width, row)) {
    return false; // Laser not found, return false
  }
  
  return helper_solver(
    board, height, width, message, row, col, direction, target, beam);

}


