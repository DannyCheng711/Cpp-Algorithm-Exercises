/*  This pre-supplied function looks up a given word in the dictionary
    of approved words. It is assumed that the input word is in uppercase.
    The function returns true if the word is in the dictionary.
    Otherwise returns false. */

bool dictionary_search(const char *word);

/* add your function prototypes here */
bool valid_step(const char* text1, const char* text2);
bool display_chain(const char** chain,  ostream& output_stream);
bool valid_chain(const char** chain);
bool find_chain(const char* start_word, const char* target_word, const char** answer_chain, int max_steps);