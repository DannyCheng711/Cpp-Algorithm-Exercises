#include <iostream>

using namespace std;


bool helper(const char* str1, const char* str2) {
    if (!*str1) return true;
    if (*str1 != *str2) return false;

    return helper(str1 + 1, str2 + 1);

}

bool is_prefix(const char* str1, const char* str2){
    return helper(str1, str2);
}

int substring_position(const char* str1, const char* str2){

    if (!*str1 && !*str2) return 0;
    if (!*str1 && *str2) return 0;

    for (int index = 0; str2[index]; index++){
        if (is_prefix(str1, str2 + index)) {
            return index;
        }
    }

    return -1;

}