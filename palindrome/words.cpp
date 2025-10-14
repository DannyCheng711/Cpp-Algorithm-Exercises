#include <iostream>
#include <cstring>
#include <cctype>
#include <string>
#include <algorithm>
#include <unordered_map>

using namespace std;

void reverse(const char* word, char* reversed_word){
    reversed_word[0] = '\0';
    int word_len = strlen(word);
    for (int i = 0; word[i]; i++){
        reversed_word[word_len - i - 1] = word[i];
    }

    reversed_word[word_len] = '\0';

}


bool compare_iterative(const char* word1, const char* word2) {
    
    int cur_index1 = 0;
    int cur_index2 = 0;

    while (true) {

        // manipulate word1[cur_index1] and word2[cur_index2] when the value is '\0' may has undefined behaviour
        while (word1[cur_index1] && !isalpha(word1[cur_index1])) cur_index1++;
        while (word2[cur_index2] && !isalpha(word2[cur_index2])) cur_index2++;

        if (!word1[cur_index1] && !word2[cur_index2]) return true;
        
        if (tolower(word1[cur_index1]) == tolower(word2[cur_index2])){
            cur_index1++;
            cur_index2++;
        } else return false;

    }
}


bool compare(const char* word1, const char* word2) {
    
    while (*word1 && !isalpha(*word1)) word1++;
    while (*word2 && !isalpha(*word2)) word2++;

    // base case
    if (!*word1 && !*word2) return true;
    if (tolower(*word1) != tolower(*word2)) return false;

    return compare(word1 + 1, word2 + 1);
    
}

bool palindrome(const char* sentence) {

   char reverse_sentence[strlen(sentence)];

   reverse(sentence, reverse_sentence);

   return compare(sentence, reverse_sentence); 

}

bool anagram(const char* sentence1, const char* sentence2){
    string str1 = string(sentence1);
    string str2 = string(sentence2);

    // Remove non-alphabet characters from both strings iteratively
    string filtered_str1 = "";
    string filtered_str2 = "";

    for (char c : str1) {
        if (isalpha(c)) {
            filtered_str1 += tolower(c);
        }
    }

    for (char c : str2) {
        if (isalpha(c)) {
            filtered_str2 += tolower(c);
        }
    }

    sort(filtered_str1.begin(), filtered_str1.end());
    sort(filtered_str2.begin(), filtered_str2.end());

    return compare(filtered_str1.c_str(), filtered_str2.c_str()); 
}



bool anagram_umap(const char* sentence1, const char* sentence2){
    unordered_map<char, int> umap1;

    for (int index = 0; sentence1[index]; index++) {
        char ch1 = tolower(sentence1[index]);
        if (isalpha(ch1)) {
            ch1 = tolower(ch1);
            umap1[ch1]++ ;
        }
    }

    for (int index = 0; sentence2[index]; index++) {
        char ch2 = sentence2[index];
        if (isalpha(ch2)) {
            ch2 = tolower(ch2);
            umap1[ch2]-- ;
        }
    }

    for (auto& entry: umap1) {
        if (entry.second != 0) {
            return false;
        }
    }
    return true;

}

