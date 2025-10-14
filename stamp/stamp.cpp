#include <iostream>
#include <openssl/sha.h>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <fstream>
#include <string>
#include "stamp.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

// helper function for internal use only
// transforms raw binary hash value into human-friendly hexademical form
void convert_hash(const unsigned char *str, char *output, int hash_length) {
  char append[16];
  strcpy (output, "");
  for (int n=0; n<hash_length; n++) {
    sprintf(append,"%02x",str[n]);
    strcat(output, append);
  }
}

// pre-supplied helper function
// generates the SHA1 hash of input string text into output parameter digest
// ********************** IMPORTANT **************************
// ---> remember to include -lcrypto in your linking step <---
// ---> so that the definition of the function SHA1 is    <---
// ---> included in your program                          <---
// ***********************************************************
void text_to_SHA1_digest(const char *text, char *digest) {
  unsigned char hash[SHA_DIGEST_LENGTH];
  SHA1( (const unsigned char *) text, strlen(text), hash);
  convert_hash(hash, digest, SHA_DIGEST_LENGTH);
}

/* add your function definitions here */
int leading_zeros(const char *digest){

  int zero_count = 0;
  bool leading_tag = true;

  for (int char_idx = 0; digest[char_idx]; char_idx++){
    // valid digit
    if ((digest[char_idx] >= '0' && digest[char_idx] <= '9') || (digest[char_idx] >= 'a' && digest[char_idx] <= 'f')){
      if (digest[char_idx] != '0') leading_tag = false;
      if (digest[char_idx] == '0' && leading_tag) zero_count++;
    }
    else {
      return -1;
    }
    
  }
  return zero_count;
}

bool file_to_SHA1_digest(string filename, char digest[]){

  ifstream file;
  file.open(filename);
  char character;

  string file_content = "";
  
  file.get(character);

  if (file.fail()) {
    strcpy(digest, "error");
    return false;
  }

  while(!file.eof()){
    file_content+= character;
    // file >> character; // skip space
    file.get(character);
  }

  file.close();

  text_to_SHA1_digest(file_content.c_str(), digest);

  return true;

} 

bool make_header(const char *recipient, const char *filename, char header[]){
  
  char digest[41]; 
  strcpy(header, ""); 
  
  if (file_to_SHA1_digest(string(filename), digest)){

    // recipent of header  
    strcat(header, recipient);
    strcat(header, ":");
    // digest of header  
    strcat(header, digest);
    strcat(header, ":");

    int count = 0;
    char header_wo_count[512];
    strcpy(header_wo_count, header);
    // header without count = 0
    strcat(header, to_string(count).c_str());

    char new_digest[41]; 
    text_to_SHA1_digest(header, new_digest);

    // hash should implement on the whole header
    while(leading_zeros(new_digest) < 5 && count < 10000000){
      // initialize digest and header 
      strcpy(new_digest, "");
      strcpy(header, header_wo_count);
      count++;
      text_to_SHA1_digest(strcat(header, to_string(count).c_str()), new_digest);
    }

    if(leading_zeros(new_digest) < 5 ) return false;
    else return true;
  }

  return false;

}

// ("wjk@imperial.ac.uk", "wjk@imperial.ac.uk:60eb8db1a3c0ea1ad3d5548f248b4a3e73711ee5:313889", "message1.txt");
MessageStatus check_header(const char *recipient, const char *header, const char *filename){
  // condition 1:  Confirm that the header is in the correct format (i.e. three fields separated by ’:’)
  string info = "";
  string recipent_from_header = "";
  string message_from_header = "";
  string count_from_header = "";
  int colon_count = 0;

  for (int i = 0; header[i]; i++){
    if (header[i] != ':') info += header[i];
    else if (header[i] == ':'){
      colon_count++;
      // first part 
      if (colon_count == 1){
        recipent_from_header = info;
      }
      // second part 
      if (colon_count == 2){
        message_from_header =  info;
      }
      info = "";
    }

  }

  // third part 
  count_from_header = info;
  
  if (colon_count != 2){
    return INVALID_HEADER;
  }
  

  // condition 2:  Check the recipient mentioned in the first field of the header matches the expected recipient
  if (strcmp(recipient, recipent_from_header.c_str())){
    return WRONG_RECIPIENT;
  }

  // condition 3: Check that the SHA1 digest of the message body given in the header matches the actual SHA1 digest of the received message.
  char digest[41];
  if (file_to_SHA1_digest(string(filename), digest)){
    if (strcmp(digest, message_from_header.c_str())){
      return INVALID_MESSAGE_DIGEST;
    }
  }else{
    return INVALID_MESSAGE_DIGEST;
  }

  // condition 4: Check that the SHA1 digest of the header is an acceptable header.
  char digest_header[41];
  text_to_SHA1_digest(header, digest_header);
  if (leading_zeros(digest_header) < 5) {
    return INVALID_HEADER_DIGEST;
  }
  
  return VALID_EMAIL;

}