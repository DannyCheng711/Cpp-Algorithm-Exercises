#include <iostream>
#include <cstring>
#include <cassert>
#include <cctype>
#include <fstream>
#include "scrabble.h"

using namespace std;

/* insert your function definitions here */

int tile_score(char ch){
    // handle blank tile
    if (ch == ' ' || ch == '?'){
        return 0;
    }

    // check valid tile range
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')){
        // convert character to upper character
        char upper_ch = toupper(ch);
        int score[] = {
            1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

        return score[upper_ch - 'A'];
    
    }else{

        return -1; 
    
    }

}

bool helper(const char* word, const char* tiles, int index, char* played_tiles, bool* used){
    // base case 
    if (word[index] == '\0'){
        played_tiles[index] = '\0';
        return true;
    }

    for (int tile_index = 0; tiles[tile_index] != '\0'; tile_index++){
        if (!used[tile_index]){
            if (tiles[tile_index] == word[index] || tiles[tile_index] == ' ' || tiles[tile_index] == '?'){
                used[tile_index] = true;
                played_tiles[index] = tiles[tile_index]; 
                // Recursive call for the next character
                if (helper(word, tiles, index + 1, played_tiles, used)) {
                    return true;
                }
                used[tile_index] = false;
                played_tiles[index] = '\0'; 
            }
        }
    }

    return false;

}


bool can_form_word_from_tiles(const char* word, const char* tiles, char* played_tiles){
    
    // Initialize played_tiles and used array
    for (int i = 0; word[i] != '\0'; i++) {
        played_tiles[i] = '\0';
    }
    
    bool used[100] = {false};

    return helper(word, tiles, 0, played_tiles, used);

}

int compute_score(const char* played_tiles, const ScoreModifier* score_modifiers){

    // NONE, DOUBLE_LETTER_SCORE, TRIPLE_LETTER_SCORE, DOUBLE_WORD_SCORE, TRIPLE_WORD_SCORE

    bool double_word_score_flag = false;
    bool triple_word_score_flag = false;

    int score_sum = 0;

    for (int tile_index = 0; played_tiles[tile_index] != '\0'; tile_index++){
        int t_score = tile_score(played_tiles[tile_index]);

        if (score_modifiers[tile_index] == DOUBLE_LETTER_SCORE){
            t_score *= 2;
        }

        if (score_modifiers[tile_index] == TRIPLE_LETTER_SCORE){
            t_score *= 3;
        }
        
        if (score_modifiers[tile_index] == DOUBLE_WORD_SCORE) {
            double_word_score_flag = true;
        }

        if (score_modifiers[tile_index] == TRIPLE_WORD_SCORE) {
            triple_word_score_flag = true;
        }

        score_sum+= t_score;

    }

    if (double_word_score_flag){
        score_sum *= 2;
    }

    if (triple_word_score_flag){
        score_sum *= 3;
    }

    if (played_tiles[7] == '\0'){
        score_sum += 50; 
    }

    return score_sum;

}

int highest_scoring_word_from_tiles(const char* tiles, const ScoreModifier* score_modifiers, char* word){

    // read word in words.txt
    ifstream in_stream;

    // Open the input and output files
    ifstream file("words.txt");

    int highest_score = -1; 
    char highest_score_word[512] = {};
    while (file.getline(word, 512)) {  // Read line into C-style string

        char played_tiles[80] = {};
        bool success = can_form_word_from_tiles(word, tiles, played_tiles);
        if (success){
            int current_score = compute_score(played_tiles, score_modifiers);
            if (current_score > highest_score){
                highest_score = current_score;
                strcpy(highest_score_word, word); 
            }
        }
    }
    

    file.close(); // Close the file
    strcpy(word, highest_score_word);

    return highest_score;


}
