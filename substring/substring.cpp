#include <iostream>
#include <cstring> 

using namespace std;

bool is_prefix_helper(const char* str1, const char* str2, int str_idx){
    // base case
    if (!str1[str_idx]) return true;
    if (str1[str_idx] != str2[str_idx]) return false;

    return is_prefix_helper(str1, str2, str_idx + 1) ;     
}

bool is_prefix(const char* str1, const char* str2){
    
    if (strlen(str1) > strlen(str2)) return false;
    if (strlen(str1) == 0 || strlen(str2) == 0) return true;

    return is_prefix_helper(str1, str2, 0); 
}

int substring_position(const char* str1, const char* str2) {

    if (strlen(str1) == 0 || strlen(str2) == 0) return 0;

    for (int start = 0; str2[start]; start++){
        // manipulate str ++ to determine the string start point 
        if (is_prefix(str1, str2 + start)) {
            return start; 
        }
    }
    return -1; 
}