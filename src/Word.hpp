#ifndef F4C1AEBD_1082_4D22_9705_69FD091697D5
#define F4C1AEBD_1082_4D22_9705_69FD091697D5

#include <string>
#include <vector>
using namespace std;

class Word
{
public:
    std::string text;
    std::string letter_group;
    std::string first_two;
    std::string last_two;
    bool isUsed;
    int num_of_connectable_words;
};
bool operator<(const Word &s1, const Word &s2)
{
    if (s1.num_of_connectable_words < s2.num_of_connectable_words)
        return true;
    else
        return false;
}


vector<Word> get_all_words_from_file();
vector<Word> get_acceptable_words(string lastTwoLetters);
int get_acceptable_words_count(string last_two_letters);
void fill_nested_list_with_words_lists(vector<Word> &aceptable_list);
void set_is_used(string word);
void write_word_to_kaladont_game(string w);
string choose_next_word(string w);

/**
 * @section Second idea:
 * Vector of vectors
 */
void make_vector_of_vectors();


#endif /* F4C1AEBD_1082_4D22_9705_69FD091697D5 */
