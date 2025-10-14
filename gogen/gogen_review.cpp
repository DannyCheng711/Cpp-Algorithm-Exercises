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
bool get_position(char **board, char ch, int &row, int &column) {
    for (int r = 0; r < HEIGHT; r++){
        for (int c = 0; c < WIDTH; c++){
            if (board[r][c] == ch) {
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

bool dfs(char **board, int row, int col, int word_index, char *word) {
  // base case
  if (row < 0 || col < 0 || row >= HEIGHT || col >= WIDTH) return false;
  if (!word[word_index]) return true;
  if (word[word_index] != board[row][col]) return false;

  char temp = board[row][col];
  board[row][col] = '#';

  if (dfs(board, row + 1, col, word_index + 1, word) || dfs(board, row - 1, col, word_index + 1, word) ||
      dfs(board, row, col + 1, word_index + 1, word) || dfs(board, row, col - 1, word_index + 1, word) ||
      dfs(board, row + 1, col + 1, word_index + 1, word) || dfs(board, row + 1, col - 1, word_index + 1, word) ||
      dfs(board, row - 1, col - 1, word_index + 1, word) || dfs(board, row - 1, col + 1, word_index + 1, word)){

        board[row][col] = temp;
        return true;

      }

  board[row][col] = temp;
  return false;
  
}

bool valid_solution(char **board, char **words){
  for (int n = 0 ; words[n]; n++) {

    char* word = words[n];
    bool find = false;

    for (int r = 0 ; r < HEIGHT; r++) {
      for (int c = 0; c < WIDTH; c++) {
        if (dfs(board, r, c, 0, word)) {
          find = true; 
        }
      }
    }

    if (!find) return false;

  }

  return true;

}

void update(char **board, char ch, Mask& x){
  int r = -1;
  int c = -1;
  if (get_position(board, ch, r, c)){
    x.set_all(false);
    x.set_element(r, c, true);
  } else {
    for (int row = 0; row < HEIGHT; row++) {
      for (int col = 0; col < WIDTH; col++) {
        if (isalpha(board[row][col]) && (board[row][col] != ch)){
          x.set_element(row, col, false);
        }
      }
    }
  }

  if (x.count() == 1) {
    int r_mask, c_mask;
    x.get_position(true, r_mask, c_mask);
    board[r_mask][c_mask] = ch;
  }

}

void neighbourhood_intersect(Mask& one, Mask& two) {

  Mask one_copy = one;
  one.intersect_with(two.neighbourhood());
  two.intersect_with(one_copy.neighbourhood());

}

bool helper(char **board, char **words, bool *used, Mask *masks, int word_count) {

  // Base case: If all words have been placed, check if the solution is valid
  if (word_count == 0) {
      return valid_solution(board, words);
  }

  // Save the current state of the masks and board for backtracking
  Mask original_masks[25];
  for (int i = 0; i < 25; i++) {
      original_masks[i] = masks[i];
  }

  char **original_board = allocate_2D_array(HEIGHT, WIDTH);
  for (int i = 0; i < HEIGHT; i++) {
      for (int j = 0; j < WIDTH; j++) {
          original_board[i][j] = board[i][j];
      }
  }

  // Try placing each unused word
  for (int w = 0; words[w]; w++) {
    if (used[w]) continue; 

    char *word = words[w];
    bool success = true;
    // Refine masks and update board for the current word
    for (int wc = 1; word[wc]; wc++) {
        int idx1 = word[wc - 1] - 'A';
        int idx2 = word[wc] - 'A';

        neighbourhood_intersect(masks[idx1], masks[idx2]);

        // If any mask becomes empty, abort processing this word
        if (masks[idx1].count() == 0 || masks[idx2].count() == 0) {
            success = false;
            break;
        }
    }

    if (success) {
      used[w] = true; // Mark the word as used

      // Update the board based on refined masks
      for (int i = 0; i < 25; i++) {
          update(board, 'A' + i, masks[i]);
      }

      // Recursively try placing the next word
      if (helper(board, words, used, masks, word_count - 1)) {
          deallocate_2D_array(original_board, HEIGHT);
          return true;
      }

      used[w] = false; // Backtrack: Mark the word as unused
    
    }

    // Backtracking: Restore the masks, board
    for (int i = 0; i < 25; i++) {
        masks[i] = original_masks[i];
    }
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            board[i][j] = original_board[i][j];
        }
    }
   
  }

  // Clean up dynamically allocated memory
  deallocate_2D_array(original_board, HEIGHT);
  return false;
}

bool solve_board(char **board, char **words) {

  Mask masks[25];
  // initialize
  for (int i = 0; i < 25; i++) {
    char ch = 'A' + i;
    Mask mask; 
    update(board, ch, mask);
    masks[i] = mask;
  }

  // iterating each word
  /*
  while (!valid_solution(board, words)){
    for (int n = 0; words[n]; n++){
      char *word = words[n];
      for (int ch_index = 1; word[ch_index]; ch_index++){
        neighbourhood_intersect(mask_array[word[ch_index - 1] - 'A'], mask_array[word[ch_index] - 'A']);
        
      }
    }

    for (int i = 0 ; i < 25; i++){
      update(board, 'A' + i, mask_array[i]);
    }

  }
  
  return true;
  */
  bool used[512] = {false}; // Assume a maximum of 100 words
  int word_count = 0;
  for (int i = 0; words[i]; i++) {
      word_count++;
  }

  print_board(board);

  return helper(board, words, used, masks, word_count);
}
