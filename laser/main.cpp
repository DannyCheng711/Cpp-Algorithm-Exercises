#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <cassert>
#include "laser.h"

using namespace std;

/* internal helper function used to check results */
void check(bool condition) {
  cout << " [";
  if (!condition)
    cout << "in";
  cout << "correct]" << endl;
}

int main() {

  int height, width, row;

    cout << "============== Pre-supplied functions ==================" << endl << endl;

  cout << "Loading board from 'biscuit.txt'... ";
  char **board = load_board("biscuit.txt", height, width);
  assert(board);
  cout << "Done!" << endl<< "Printing board..." << endl;
  print_board(board, height, width);
  cout << "Done!" << endl << endl;

  cout << "====================== Question 1 ======================" << endl << endl;

  bool success = find_laser(board, height, width, row);
  if (!success) 
    cout << "Laser could not be found!" << endl;
  else 
    cout << "Laser found at start of row " << row;
  check(row == 2);
  cout << endl;

  cout << "====================== Question 2 ======================" << endl << endl;

  /* uncomment this if you prefer to start with a simple test case */
  
  char mylabel = mirror_label(board, height, width, 2, 15);   
  assert(mylabel == 'B');     
  

  
  int coords[][2] = { {2,15}, {4, 7}, {7, 24}, {9,15}, {4, 24}, {5, 3} };
  char expected[] = { 'B', 'C', 'S', 'T', '\0', '\0' };

  for (int n=0; n<6; n++) {
    cout << "The mirror label at (" << setw(2) << coords[n][0] << ", " << setw(2) << coords [n][1] << ")" << " is ";
    char label = mirror_label(board, height, width, coords[n][0], coords[n][1]);
    if (label != '\0')
      cout << label;
    else
      cout << "(no label or not mirror)";
    check(label == expected[n]);
  }

  cout << endl;

  deallocate_2D_array(board, height);
  
  cout << "====================== Question 3 ======================" << endl << endl;

  /* uncomment this if you prefer to start with a simple test case */
  /*
    char message[512];
    board = load_board("biscuit.txt", height, width);
    assert(board);
    int last_row, last_col;
    success = shoot(board, height, width, message, last_row, last_col);
    print_board(board, height, width);
    assert(success == true);
    assert(last_row == 9 && last_col == 31);
  */

  
  struct TestCase {
    char filename[512];
    char message[512];
    bool success;
    int coordinates[2];
  };

  TestCase tests[4] = {
    {"biscuit.txt", "BISCUIT", true, { 9, 31} },
    {"biscuit-plan.txt", "BIS", false, { 4, 24} },
    {"happynewyear.txt", "HAPPYNEWYEAR", true, {13, 35} },
    {"happynewyear-plan.txt", "HAP", false, { 6, 28} }
  };
  
  for (int n=0; n<4; n++) {
    cout << "Loading board from '" << tests[n].filename << "'...";
    board = load_board(tests[n].filename, height, width);
    assert(board);
    cout << " Done." << endl;
    
    cout << "[" << tests[n].filename << "] Shooting laser..." << endl;
    char message[512];
    int lr = 0, lc = 0;
    success = shoot(board, height, width, message, lr, lc);

    print_board(board, height, width); 
    cout << "[" << tests[n].filename << "] message is \"" << message << "\"";
    check (!strcmp(message, tests[n].message));
    cout << "[" << tests[n].filename << "] return value is " << ((success) ? "true" : "false");
    check (success == tests[n].success);
    cout << "[" << tests[n].filename << "] final coordinates are (" << setw(2) << lr << ", " << setw(2) << lc << ")";
    check (lr == tests[n].coordinates[0] && lc == tests[n].coordinates[1]);
    cout << endl;
    
    deallocate_2D_array(board, height);
  }
  
  cout << "====================== Question 4 ======================" << endl << endl;

  char message[512];
  int lr, lc;
  
  cout << "Loading board from 'biscuit-plan.txt'... ";  
  board = load_board("biscuit-plan.txt", height, width);
  assert(board);
  cout << "Done!" << endl;

  cout << "Solving...";
  success = solve(board, height, width, "BISCUIT");
  cout << " Done!" << endl;
  print_board(board, height, width);
  cout << ( success ? "Solved!" : "No solution!" );
  check(success);
  cout << "Conducting shooting test...";
  success = shoot(board, height, width, message, lr, lc);
  check(success && lr == 9 && lc == 31 && !strcmp(message, "BISCUIT"));
  print_board(board, height, width);    
  deallocate_2D_array(board, height);
  cout << endl;

  cout << "Loading board from 'happynewyear-plan.txt'... ";  
  board = load_board("happynewyear-plan.txt", height, width);
  assert(board);
  cout << "Done!" << endl;

  cout << "Solving...";
  success = solve(board, height, width, "HAPPYNEWYEAR");
  cout << " Done!" << endl;
  print_board(board, height, width);
  cout << ( success ? "Solved!" : "No solution!" );
  check(success);
  cout << "Conducting shooting test...";
  success = shoot(board, height, width, message, lr, lc);
  check(success && lr == 13 && lc == 35 && !strcmp(message, "HAPPYNEWYEAR"));
  print_board(board, height, width);    
  deallocate_2D_array(board, height);
  cout << endl;

  cout << "========================= FIN ==========================" << endl << endl;
  
  return 0;
}
