#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <string>
#include <cctype>
#include <unistd.h>
#include <set>

using namespace std;

// to control aspects of testing and solution animation
// see #define's for SIMPLE_TEST and ANIMATE_SOLUTION in sokoban.h

#include "sokoban.h"

int main() {

    /* this section illustrates the use of the pre-supplied functions */
  cout << "============== Pre-supplied functions ==================" << endl << endl;

  int height, width;

  /* loading level */
  char **level = load_level("level4.txt", height, width);
  assert(level);
  cout << "Done!" << endl;

  /* printing level */
  print_level(level, height, width);
  cout << "Done!" << endl << endl;

  /* compute hash of level */
  cout << "Hash of level is: " << level_hash(level, height, width) << endl << endl;

  cout << "Making change to level configuration..." << endl;

  /* make change and re-compute hash
     point is to show that every different level configuration
     has a unique hash */
  level[8][12] = ' ';
  level[7][12] = '@';
  print_level(level, height, width);
  cout << endl;
  cout << "Hash of level is now: " << level_hash(level, height, width) << endl << endl;
  deallocate_2D_array(level, height);

  cout << "====================== Question 1 ======================" << endl << endl;

#ifdef SIMPLE_TEST
  {
    char **current = load_level("level1.txt", height, width);
    print_level(current, height, width);
    cout << "Number of goal squares without boxes: "
	 << goal_squares_without_boxes (current, height, width)
	 << endl << endl;
    deallocate_2D_array(current, height);
  }
#else  
  const char *q1files[] = {"level0.txt", "level1.txt", "level4.txt", NULL };

  const int q1expected[] = { 2, 2, 6 };

  for (int n=0; q1files[n]; n++) {
    char **current = load_level(q1files[n], height, width);
    print_level(current, height, width);
    int found = goal_squares_without_boxes (current, height, width);
    cout << "Number of goal squares without boxes: "
	 << found << " (" << ((found == q1expected[n]) ? "" : "un")
	 << "expected) " << endl << endl;
    deallocate_2D_array(current, height);
  }
#endif


  cout << "====================== Question 2 ======================" << endl << endl;

#ifdef SIMPLE_TEST
  {
    char **current = load_level("level3.txt", height, width);
    print_level(current, height, width);
    int row, col;
    bool success = find_player(current, height, width, row, col);
    if (success)
      cout << "Player found at (" << row << "," << col << ")" << endl;
    else
      cout << "Player not found!" << endl;
    cout << endl;
    deallocate_2D_array(current, height);
  }
#else 
  const char *q2files[] = {"level0.txt", "level2.txt", "level4.txt", NULL};
  const int q2expected[][2] = { {1,1}, {4,8}, {8,12} };
    
  for (int n=0; q2files[n]; n++) {
    char **current = load_level(q2files[n], height, width);
    print_level(current, height, width);
    int row, col;
    bool success = find_player(current, height, width, row, col);
    if (success) {
      cout << "Player found at (" << row << "," << col << ") (";
      if (row != q2expected[n][0] || col != q2expected[n][1])
	cout << "un";
      cout << "expected)" << endl;
    } else
      cout << "Player not found! (unexpected)" << endl;
    cout << endl;
    deallocate_2D_array(current, height);
  }
#endif

  cout << "====================== Question 3 ======================" << endl << endl;


#ifdef SIMPLE_TEST
  {
    char **current = load_level("level4.txt", height, width);
    print_level(current, height, width);

    cout << "Trying to move in direction 'l'" << endl;
    bool is_push = false;
    bool valid = make_move(current, height, width, 'l', is_push);
    if (valid) {
      cout << "Valid move! Is "
	   << (!is_push ? "not": "") << " a push." << endl;     
      cout << "New level configuration:" << endl;
      print_level(current, height, width);
    } else 
      cout << "Invalid move." << endl;
    cout <<endl;

    cout << "Trying to move in direction 'u'" << endl;
    is_push = false;
    valid = make_move(current, height, width, 'u', is_push);
    if (valid) {
      cout << "Valid move! Is "
	   << (!is_push ? "not": "") << " a push." << endl;     
      cout << "New level configuration:" << endl;
      print_level(current, height, width);
    } else 
      cout << "Invalid move." << endl;
    cout << endl;
    deallocate_2D_array(current, height);
  }
#else
  const char *q3files[] = {"level4.txt", "level2.txt", "level1.txt", "level3.txt", NULL };
  const char q3dirs[] = {'u', 'r', 'l', 'd' };
  bool q3expected[][2] = { {true, false}, {true, true}, {false, false}, {false, false} };

  for (int n=0; q3files[n]; n++) {
    char **current = load_level(q3files[n], height, width);
    print_level(current, height, width);
    cout << "Trying to move in direction '" << q3dirs[n] << "'" << endl;
    bool is_push = false;
    bool valid = make_move(current, height, width, q3dirs[n], is_push);
    if (valid) {
      cout << "Valid move! Is "
	   << (!is_push ? "not ": "") << "a push (";
      if (valid != q3expected[n][0] || is_push != q3expected[n][1]) 
	cout << "un";
      cout << "expected)." << endl;     
      cout << "New level configuration:" << endl;
      print_level(current, height, width);
    } else {
      cout << "Invalid move (";
      if (valid != q3expected[n][0])
	cout << "un";
      cout << "expected)." << endl;
    }
    cout << endl;
    deallocate_2D_array(current, height);
  }
#endif

  cout << "====================== Question 4 ======================" << endl << endl;


  #ifdef SIMPLE_TEST
  {
    char solution[MAX_SOLUTION_LENGTH];
    cout << "Trying to solve level0.txt..." << endl;
    char **current = load_level("level0.txt", height, width);
    print_level(current, height, width);
    if (solve_level(current, height, width, solution)) {
      cout << "Success! Final level configuration:" << endl;
      print_level(current, height, width);
      cout << "Solution is '" << solution << "' (length "
	   << strlen(solution) << ")" << endl;
#ifdef ANIMATE_SOLUTION

      /* if animation is not working for you for your screen size, 
         feel free to adjust SCREEN_HEIGHT in sokoban.h */

      cout << "Press enter to animate solution..." << endl;
      char pause[512];
      cin.getline(pause,512);
      current = load_level("level0.txt", height, width);
      animate_solution(current, height, width, solution);
#endif
    } else 
      cout << "no solution!" << endl;
    cout << endl;
    deallocate_2D_array(current, height);      
  }
#else    
  const char *q4files[] = {"level0.txt", "level1.txt", "level2.txt", "level3.txt", "level4.txt", NULL };  
  for (int n=0; q4files[n]; n++) {
    char solution[MAX_SOLUTION_LENGTH];
    cout << "Trying to solve " << q4files[n] << "..." << endl;
    char **current = load_level(q4files[n], height, width);
    print_level(current, height, width);
    if (solve_level(current, height, width, solution)) {
      cout << "Success! Final level configuration:" << endl;
      print_level(current, height, width);
      cout << "Solution is '" << solution << "' (length "
	   << strlen(solution) << ")" << endl;
#ifdef ANIMATE_SOLUTION

      /* if animation is not working for you for your screen size, 
         feel free to adjust SCREEN_HEIGHT in sokoban.h */

      cout << "Press enter to animate solution..." << endl;
      char pause[512];
      cin.getline(pause,512);
      current = load_level(q4files[n], height, width);
      animate_solution(current, height, width, solution);
#endif
    } else
      cout << "no solution!" << endl;
    cout << endl;
    deallocate_2D_array(current, height);
  }
#endif


  cout << "======================= The End ========================" << endl << endl;
     
  return 0;
}
