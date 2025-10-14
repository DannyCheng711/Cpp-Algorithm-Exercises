#include <iostream>
#include <cstring>
#include <unordered_map>
#include <vector>

using namespace std; 

unordered_map<char, int> letter_map = {
    {'b', 1}, {'f', 1}, {'p', 1},
    {'c', 2}, {'g', 2}, {'j', 2}, {'k', 2}, {'q', 2}, {'s', 2}, {'x', 2}, {'z', 2}, 
    {'d', 3}, {'t', 3},
    {'l', 4},
    {'m', 5}, {'n', 5}, {'r', 6}
};

void encode(const char* surname, char* soundex){

    for (int i = 0 ; i < 5; i++){
        soundex[i] = '\0';
    }

    soundex[0] = toupper(surname[0]);
    int soundexLen = 1;

    // start from 1, terminate at '\0' or len equal to 4
    for (int charIndex = 1; surname[charIndex] != '\0' && soundexLen < 4; charIndex++){

        // check duplicate value character
        char curLowerChar = tolower(surname[charIndex]);  

        if (letter_map.find(curLowerChar) != letter_map.end()){
            if (letter_map[curLowerChar] != letter_map[surname[charIndex - 1]]){
                soundex[soundexLen] =  '0' + letter_map[curLowerChar];
                soundexLen++;
            }
        }
    }  

    while (soundexLen < 4) {
        soundex[soundexLen] = '0'; 
        soundexLen++;
    }

    soundex[soundexLen] = '\0';

}

bool compare(const char* str1, const char* str2) {

    if (!*str1 && !*str2) return true;

    if (!*str1 || !*str2) return false;
    
    if (*str1 != *str2) return false;

    return compare(str1 + 1, str2 + 1);
}
// count("Leeson", "Linnings, Leasonne, Lesson and Lemon.")
int count (const char* str1, const char* str2) {
    char str1_soundex[5] = {'\0'};
    encode(str1, str1_soundex);

    int idx = 0;
    char word[strlen(str2)] = {'\0'};
    int wordLen = 0; // to control word length 
    int count = 0;

    while(str2[idx] != '\0') {
        
        if (str2[idx] == ',' || str2[idx] == ';' || str2[idx] == '.') {
            
            if (str2[idx + 1] != '\0' && str2[idx + 1] == ' ') idx++; 

            // if word existed
            if (wordLen > 0) {
                word[wordLen] = '\0';
                char word_soundex[5] = {'\0'};
                encode(word, word_soundex);
                if (!strcmp(str1_soundex, word_soundex) && strcmp("and", word_soundex)) {
                    count++;
                }

                // reset wordLen
                wordLen = 0;
            }
        } else {
            // add word 
            word[wordLen] = str2[idx]; 
            wordLen++; 
        }
        // increase str2 index
        idx++;
    }

    return count;

}