#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include "solitaire.h"

using namespace std;

int main() {

  /* this section illustrates the use of the pre-supplied functions */
  cout << "============== Pre-supplied functions ==================" << endl << endl;
 
  int height, width;
  char **puzzle = load_board("board.txt", height, width);
  assert(puzzle);

  cout << "Printing puzzle board:" << endl;
  /* prints the puzzle board with row and column numbers */
  print_board(puzzle, height, width);
  cout << endl;
  
  cout << "====================== Question 1 ======================" << endl << endl;

  int height1, width1;
  char **board1 = load_board("board1.txt", height1, width1);
  assert(board1);
  print_board(board1, height, width);
  
  int height2, width2;
  char **board2 = load_board("board2.txt", height2, width2);
  assert(board2);
  print_board(board2, height, width);

  int height3, width3;
  char **board3 = load_board("board3.txt", height3, width3);
  assert(board3);
  print_board(board3, height, width);
  
  assert(height2 == height1 && width2 == width1);
  assert(height3 == height1 && width3 == width1);

  cout << endl;

  bool same = are_identical(board1, board2, height1, width1);
  cout << "Boards 1 and 2 are " << (same ? "" : "not ") << "identical." << endl << endl;

  same = are_identical(board2, board3, height1, width1);
  cout << "Boards 2 and 3 are " << (same ? "" : "not ") << "identical." << endl << endl;

  deallocate_2D_array(board1, height1);
  deallocate_2D_array(board2, height2);
  deallocate_2D_array(board3, height3);


  cout << "====================== Question 2 ======================" << endl << endl;
  
  
  cout << "Starting from:" << endl;
  print_board(puzzle, height, width);
  cout << endl;
  

  /* uncomment for simple test case */
  /*
  cout << "Move D1S is";
   if (make_move(puzzle, "D1S", height, width)) {
     cout << " valid" << endl;
    print_board(puzzle, height, width);
  } else 
     cout << " invalid" << endl;
     cout << endl; 
  */
  
  const char *moves[] = { "D1S", "D0S", "F2W", "D2N", "A2E", NULL };

  for (int n=0; moves[n]; n++) {
    cout << "Move " << moves[n] << " is";
    if (make_move(puzzle, moves[n], height, width)) {
      cout << " valid" << endl;
      print_board(puzzle, height, width);
    } else 
      cout << " invalid" << endl;
    cout << endl;      
  }

  deallocate_2D_array(puzzle, height);
  
  cout << "====================== Question 3 ======================" << endl << endl;

  /* uncomment for simple test case */
  
  int h,w;
  char **first = load_board("target5.txt", h, w);
  assert(first);
  char **second = load_board("solution.txt", h, w);
  assert(second);
  char sequence[512];
  bool result = find_solution(first, second, h, w, sequence);
  if (result) 
    cout << "Success! Move sequence: " << sequence << endl;
  else
    cout << "No solution found" << endl;
  cout << endl;
  deallocate_2D_array(first, h);
  deallocate_2D_array(second, w);
  
  
  const char *filenames[] = { "board.txt", "target1.txt", "target2.txt", "target3.txt",
    "target4.txt", "target5.txt", "solution.txt", NULL };
  
  char **start = load_board(filenames[0], height, width);
  assert(start);
  for (int n=1; filenames[n]; n++) {
    cout << "Attempting to reach target '" << filenames[n] << "'..." << endl;
    char **target = load_board(filenames[n], height, width);
    assert(target);
    
    char solution[1024];
    bool success = find_solution(start, target, height, width, solution);
    
    if (success) {
      print_board(start, height, width);
      cout << "Target '" << filenames[n] << "' achieved!" << endl;
      cout << "Move sequence: " << solution << endl << endl;
    } else  {
      cout << "No solution found" << endl;
      return 0;
    }
    
    deallocate_2D_array(target, height);
  }
  deallocate_2D_array(start, height);

  cout << "======================= The End ========================" << endl << endl;
  
  return 0;
}
