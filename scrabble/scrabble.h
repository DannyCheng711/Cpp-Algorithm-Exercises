enum ScoreModifier { NONE, DOUBLE_LETTER_SCORE, TRIPLE_LETTER_SCORE, DOUBLE_WORD_SCORE, TRIPLE_WORD_SCORE };

/* insert your function prototypes here */
int tile_score(char ch);
bool can_form_word_from_tiles(const char* word, const char* tiles, char* played_tiles);
int compute_score(const char* played_tiles, const ScoreModifier* modifiers);
int highest_scoring_word_from_tiles(const char* tiles, const ScoreModifier* score_modifiers, char* c_word);




