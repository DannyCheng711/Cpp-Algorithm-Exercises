#include <iostream>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include <cstdlib>

#include "common.h"
#include "mask.h"
#include "gogen.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* internal helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* internal helper function which removes unprintable characters like carriage returns and newlines from strings */
void filter (char *line) {
  while (*line) {
    if (!isprint(*line))
     *line = '\0';
    line++;
  }
}

/* loads a Gogen board from a file */
char **load_board(const char *filename) {
  char **board = allocate_2D_array(5, 6);
  ifstream input(filename);
  assert(input);
  char buffer[512];
  int lines = 0;
  input.getline(buffer, 512);
  while (input && lines < HEIGHT) {
    filter(buffer);
    if (strlen(buffer) != WIDTH)
      cout << "WARNING bad input = [" << buffer << "]" << endl;
    assert(strlen(buffer) == WIDTH);
    strcpy(board[lines], buffer);
    input.getline(buffer, 512);
    lines++;
  }
  input.close();
  return board;
}

/* saves a Gogen board to a file */
bool save_board(char **board, const char *filename) {
  ofstream out(filename); 
  if (!out)
    return false;
  for (int r=0; r<HEIGHT; r++) {
    for (int c=0; c<WIDTH; c++) {
      out << board[r][c];
    }
    out << endl;
  }
  bool result = out.good();
  out.close();
  return result;
}

/* internal helper function for counting number of words in a file */
int count_words(const char *filename) {
  char word[512];
  int count = 0;
  ifstream in(filename);
  while (in >> word)
    count++;
  in.close();
  return count;
}

/* loads a word list from a file into a NULL-terminated array of char *'s */
char **load_words(const char *filename) {
  int count = count_words(filename);
  ifstream in(filename);
  assert(in);
  int n=0;
  char **buffer = new char *[count+1]; // +1 because we NULL terminate 
  char word[512];
  for (; (in >> word) && n<count; n++) {
    buffer[n] = new char[strlen(word) + 1];
    strcpy(buffer[n], word);
  }
  buffer[n] = NULL;
  in.close();
  return buffer;
}

/* prints a Gogen board in appropriate format */
void print_board(char **board) {
  for (int r=0; r<HEIGHT; r++) {
    for (int c=0; c<WIDTH; c++) {
      cout << "[" << board[r][c] << "]";
      if (c < WIDTH-1)
	cout << "--";
    }
    cout <<endl;
    if (r < HEIGHT-1) {
      cout << " | \\/ | \\/ | \\/ | \\/ |" << endl;
      cout << " | /\\ | /\\ | /\\ | /\\ |" << endl;
    }
  }
}

/* prints a NULL-terminated list of words */
void print_words(char **words) {
  for (int n=0; words[n]; n++) 
    cout << words[n] << endl;
}

/* frees up the memory allocated in load_board(...) */
void delete_board(char **board) {
  deallocate_2D_array(board, HEIGHT);
}

/* frees up the memory allocated in load_words(...) */
void delete_words(char **words) {
  int count = 0;
  for (; words[count]; count++);
  deallocate_2D_array(words, count);
}

/* add your own function definitions here */
bool get_position(char **board, char ch, int& row, int& column){
  for (int r = 0; r < 5; r++){
    for (int c = 0; c < 5; c++){
      if(board[r][c] == ch){
        row = r;
        column = c;
        return true;
      }
    }
  }

  row = -1;
  column = -1;
  return false; 

}

bool dfs(char **board, char *word, int row, int column, int index){
  // base case 
  if (word[index] == '\0'){
    return true;
  }
  if (row < 0 || column < 0 || row >= 5 || column >= 5 || word[index] != board[row][column]){
    return false;
  }

  // record temp cell value
  char temp = board[row][column];
  board[row][column] = '#';

  bool found = (
    // down
    dfs(board, word, row + 1, column, index + 1) ||
    // up
    dfs(board, word, row - 1, column, index + 1) ||
    // right
    dfs(board, word, row, column + 1, index + 1) ||
    // left
    dfs(board, word, row, column - 1, index + 1) ||
    // down-left
    dfs(board, word, row + 1, column - 1, index + 1) ||
    // down-right
    dfs(board, word, row + 1, column + 1, index + 1) ||
    // up-right
    dfs(board, word, row - 1, column + 1, index + 1) ||
    // up-left
    dfs(board, word, row - 1, column - 1, index + 1));

  // after getting result of finding, restore the cell value
  board[row][column] = temp;

  return found;

}

bool search_word(char **board, char* word){
  for (int r = 0; r < 5; r++){
    for (int c = 0; c < 5; c++){
      if (dfs(board, word, r, c, 0)){
        return true;
      }
    }
  }
  return false;
}

bool valid_solution(char **solution, char **words){

  for (int n=0; words[n]; n++){
    char *word = words[n];
    if (!search_word(solution, word)){
      return false;
    }
  }

  return true;
}

void update(char **board, char ch, Mask &mask){

  int row, col;
  if (get_position(board, ch, row, col)){
    mask.set_all(0);
    mask.set_element(row, col, true);
    return ;
  }

  for (int r = 0; r < 5; r++){
    for (int c = 0; c < 5; c++){
      if (board[r][c] >= 'A' && board[r][c] <= 'Y'){
        mask.set_element(r, c, false);
      }
    }
  }

  if (mask.count() == 1){
    mask.get_position(true, row, col);
    board[row][col] = ch;
  }

}

void neighbourhood_intersect(Mask &one, Mask &two){

  Mask one_nbr = one.neighbourhood();
  Mask two_nbr = two.neighbourhood();
  
  one.intersect_with(two_nbr);
  two.intersect_with(one_nbr);

}

bool solve_board(char **board, char **words){

  // Create an array of 25 masks (one for each letter ’A’ to ’Y’) and use the update(...) function to initialise them
  Mask masks[25]; 
  for (int i = 0 ; i < 25; i++){
    update(board, 'A' + i, masks[i]);
  }

  // Then repeatedly use the adjacent letters appearing in each word with the neighbourhood intersect(...) 
  // and update(...) functions to refine the masks of free letters as much as possible.
  while (!valid_solution(board, words)){
    for (int w = 0; words[w]; w++) {
      for (int wc = 1; words[w][wc]; wc++) {

        int idx1 = words[w][wc - 1] - 'A'; 
        int idx2 = words[w][wc] - 'A'; 
        // Attempt to intersect current and next masks
        neighbourhood_intersect(masks[idx1], masks[idx2]);
      }
    }

    for (int i = 0 ; i < 25; i++){
      update(board, 'A' + i, masks[i]);
    }
  }

  return true;

}

