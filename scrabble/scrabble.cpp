#include <iostream>
#include <cctype>
#include <cstring>
#include <fstream>
#include "scrabble.h"

using namespace std;


int tile_score(char ch){

    int score[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 
                5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 
                1, 4, 4, 8, 4, 10};

    if (ch == ' ' || ch == '?') return 0;
    else if (isalpha(ch)) return score[tolower(ch) - 'a'];
    else return -1; 

}

bool helper(int word_index, const char* word, const char* tiles, bool* used, char* played_tiles) {
    
    // base case 
    if (word_index == int(strlen(word))) {
        played_tiles[word_index] = '\0';
        return true;
    } 

    // check each charater in the tiles
    for (int t_index = 0; tiles[t_index]; t_index++) {
        // character is not used and equal to the current character in this word 
        if (!used[t_index] && (word[word_index] == tiles[t_index] || tiles[t_index] == '?' || tiles[t_index] == ' ')) {
            // add current character to the played_tiles
            played_tiles[word_index] = tiles[t_index];
            used[t_index] = true;
            if (helper(word_index + 1, word, tiles, used, played_tiles)) {
                return true;
            }
            played_tiles[word_index] = '\0';
            used[t_index] = false;
        }
    }

    return false;
}

bool can_form_word_from_tiles(const char* word, const char* tiles, char* played_tiles){

    bool used[80] = {false};
    played_tiles[0] = '\0';

    return helper(0, word, tiles, used, played_tiles);
    
}

int compute_score(const char *played_tiles, const ScoreModifier* modifiers){

    bool double_word_score_flag = false;
    bool triple_word_score_flag = false;

    int total_score = 0;

    for (int i = 0; played_tiles[i]; i++){
        char ch = played_tiles[i];
        if (modifiers[i] == DOUBLE_LETTER_SCORE) total_score += 2 * tile_score(ch);
        else if (modifiers[i] == TRIPLE_LETTER_SCORE) total_score += 3 * tile_score(ch);
        else total_score += tile_score(ch);

        if (modifiers[i] == DOUBLE_WORD_SCORE) double_word_score_flag = true;
        if (modifiers[i] == TRIPLE_WORD_SCORE) triple_word_score_flag = true;

    }

    if (double_word_score_flag) total_score *= 2 ; 
    if (triple_word_score_flag) total_score *= 3 ;
    if (strlen(played_tiles) == 7) total_score += 50;

    return total_score;

}

int highest_scoring_word_from_tiles(const char* tiles, const ScoreModifier* modifiers, char* c_word){
    
    fstream in("words.txt");
    char word[512]; 

    if (!in) {
        cerr << "fail to open the file" << endl;
        return -1 ;
    }

    // input word from the words
    
    int highest_score = -1; 
   
    while (in >> word) {

        char played_tiles[512] = {'\0'};
        if (can_form_word_from_tiles(word, tiles, played_tiles)) {
            int score = compute_score(played_tiles, modifiers);
            if (score > highest_score) {
                highest_score = score; 
                strcpy(c_word, word);
            }
        }

    }

    in.close();



    return highest_score;



}