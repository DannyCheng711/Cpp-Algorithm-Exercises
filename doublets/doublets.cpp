#include <iostream>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <cctype>
#include <fstream>
#include <unordered_set>
#include <vector>
#include <string>

using namespace std;

#include "dictionary.h"
#include "doublets.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* Looks up a given word in the dictionary of approved words. 
   Returns true if the word is in the dictionary.
   Otherwise returns false. */

bool dictionary_search(const char *word) {
  static Dictionary dictionary("words.txt");
  return dictionary.search(word);
}

/* add your function definitions here */
bool valid_step(const char* text1, const char* text2){

   if (!dictionary_search(text1) || !dictionary_search(text2)) return false;
   if (strlen(text1) != strlen(text2)) return false;

   int change_ch_count = 0;
   for (int i = 0; text1[i] != '\0'; i++){
      if ((toupper(text1[i]) - toupper(text2[i])) != 0){
         change_ch_count += 1;
      }
   }
   if (change_ch_count != 1) return false;
   else return true;
}

void word_to_lowercase(char* text){
   for(size_t i = 0; i < strlen(text); i++){
      text[i] = tolower(text[i]);
   }
}

bool display_chain(const char** chain,  ostream& output_stream){
   
   output_stream << chain[0] << endl;
   unordered_set<const char*> visited = {}; 
   visited.insert(chain[0]);

   int i = 1;
   while (chain[i]){
      
      if(visited.find(chain[i]) != visited.end()){
         return false;
      }

      if (valid_step(chain[i - 1], chain[i])){
         if (chain[i + 1]){
            char temp_text[strlen(chain[i])]; 
            strcpy(temp_text, chain[i]);
            word_to_lowercase(temp_text);
            output_stream << temp_text << endl;
         } else {
            output_stream << chain[i] << endl;
         }
         
      }else{
         return false;
      }
      i++;
   }
   
   return true;
}


bool valid_chain(const char** chain){
   
   unordered_set<const char*> visited = {}; 
   visited.insert(chain[0]);

   int i = 1;
   while (chain[i]){
      // rule 3
      if(visited.find(chain[i]) != visited.end()){
         return false;
      }
      // rule 2 and 4
      if (!valid_step(chain[i - 1], chain[i])){
         return false;  
      }
      i++;
   }
   
   return true;
}

bool helper(char* current_word, const char* end_word, int current_step, int max_steps, vector<char*> &temp_answer_chain){

   // this copy is store in temp_answer_chain
   // need new or string because:
   // The chain persists across recursive calls.
   // The words in temp_answer_chain must have independent memory that doesn’t get overwritten during recursion.
   char* word_copy = new char[strlen(current_word) + 1]; 
   strcpy(word_copy, current_word);
   temp_answer_chain.push_back(word_copy); // add current word to the chain

   if(!strcmp(current_word, end_word)){
      temp_answer_chain.pop_back();
      return true;
   }
   // step extend to max step, return false
   if (current_step >= max_steps){
      temp_answer_chain.pop_back();
      return false;
   }

   for (size_t idx = 0; idx < strlen(current_word); idx++){
      char temp_ch = current_word[idx];
      for (int ch_idx = 0; ch_idx < 26; ch_idx++){
         // update word
         char origin_word[strlen(current_word)]; 
         strcpy(origin_word, current_word); 
         current_word[idx] = ch_idx + 'A';

         if (valid_step(origin_word, current_word) && helper(current_word, end_word, current_step + 1, max_steps, temp_answer_chain)){   
            return true;
         }
         // restore word
         current_word[idx] = temp_ch;

      }
   }

   temp_answer_chain.pop_back();

   return false;

}

bool find_chain(const char* start_word, const char* target_word, const char** answer_chain, int max_steps){
   
   char* current_word = new char[strlen(start_word) + 1];
   strcpy(current_word, start_word);
   vector<char*> temp_answer_chain = {};
   
   if (helper(current_word, target_word, 0, max_steps, temp_answer_chain)){
      
      delete[] current_word;

      int chain_size = temp_answer_chain.size();
      // Allocate memory and copy strings into the answer_chain
      for (int i = 0; i < chain_size; i++) {
         answer_chain[i] = new char[strlen(temp_answer_chain[i]) + 1];
         strcpy(const_cast<char*>(answer_chain[i]), temp_answer_chain[chain_size - i - 1]); // reversely
      }
      answer_chain[chain_size] = NULL;  // Null-terminate the array

      return true;
   }

   return false;
}
