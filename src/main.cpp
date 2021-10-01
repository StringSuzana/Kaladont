#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include "Word.hpp"

using namespace std;

ofstream kaladont_file("game_kaladont.txt");
vector<Word> all_words;
vector<string> words_to_write;
const string KA = "ka";
const string KALADONT = "KALADONT";
vector<vector<Word>> jagged_words;
vector<Word> starting_words;
bool is_kaladont = false;

void write_all_words();
void write_result(string w, int size);
void make_vector_for_starting_words();
int main()
{
    //all_words = get_all_words_from_file();
    make_vector_of_vectors();

    make_vector_for_starting_words();

    for (size_t row = 0; row < starting_words.size(); ++row)
    {

        is_kaladont = false;      //reset
        words_to_write.clear();   //reset
        make_vector_of_vectors(); //reset
        auto starting_word = starting_words[row];
        set_is_used(starting_word.text);
        words_to_write.push_back(starting_word.text);

        // while (words_to_write[words_to_write.size() - 1].compare(KALADONT) != 0)
        while (is_kaladont == false)
        {
            string next = choose_next_word(words_to_write[words_to_write.size() - 1]);
            set_is_used(next);
            words_to_write.push_back(next);
        }
        cout << "From word: " << words_to_write[0] << endl;
        cout << "words: " << words_to_write.size() << endl;
        write_result(starting_word.text, words_to_write.size());
    }
    // write_all_words();
    kaladont_file.close();
    return 0;
}
void write_result(string w, int size)
{
    kaladont_file << w << endl;
    kaladont_file << size << endl;
}
void make_vector_of_vectors()
{
    jagged_words.clear();
    string alphabet{"abcdefghijklmnopqrstuvwxyz"};

    for (int i = 0; i < alphabet.length(); i++)
    {
        for (int j = 0; j < alphabet.length(); j++)
        {
            string folder = "groups/";
            string two_letters = "";
            two_letters = (alphabet.substr(i, 1)).append(alphabet.substr(j, 1));
            two_letters = two_letters.append(".txt");
            string full_path = folder.append(two_letters);

            string line;
            ifstream kaladont_words(full_path);
            vector<Word> temp;
            while (getline(kaladont_words, line))
            {
                Word word;
                word.isUsed = false;
                word.text = line;
                word.letter_group = word.text.substr(0, 2);
                word.first_two = word.text.substr(0, 2);
                word.last_two = word.text.substr(word.text.length() - 2, 2);
                if (word.last_two.compare(KA) != 0)
                {
                    temp.push_back(word);
                }
            }
            if (temp.size() > 0)
            {
                jagged_words.push_back(temp);
            }
        }
    }
}
void make_vector_for_starting_words()
{
    string alphabet{"abcdefghijklmnopqrstuvwxyz"};

    for (int i = 0; i < alphabet.length(); i++)
    {
        for (int j = 0; j < alphabet.length(); j++)
        {
            string two_letters = "";
            two_letters = (alphabet.substr(i, 1)).append(alphabet.substr(j, 1));
            for (size_t row = 0; row < jagged_words.size(); ++row)
            {
                auto found = find_if(jagged_words[row].begin(), jagged_words[row].end(), [two_letters](Word w)
                                     {
                                         if (w.text.size() >= 2)
                                         {
                                             return w.text.substr(w.text.size() - 2, 2).compare(two_letters) == 0;
                                         }
                                         else
                                         {
                                             return false;
                                         }
                                     });
                Word word;
                if (found != jagged_words[row].end())//if nothing is found, find_if returns pointer to end of searched vector
                {
                    // cout << found->text << endl;
                    word.text = found->text;
                    if (word.text.size() >= 2 && word.text.substr(word.text.size() - 2, 2).compare(KA) != 0)
                    {
                        starting_words.push_back(word);
                        break;
                    }
                }
            }
        }
    }
    cout << "size " << starting_words.size();
}

string choose_next_word(string w)
{
    string last_two_letters = w.substr(w.length() - 2, 2);

    auto filtered = get_acceptable_words(last_two_letters);
    if (filtered.size() == 0)
    {
        is_kaladont = true;
        return KALADONT;
    }
    fill_nested_list_with_words_lists(filtered);

    sort(filtered.begin(), filtered.end(), [](Word &w, Word &ww)
         { return ww < w; });

    return filtered[0].text;
}
void write_word_to_kaladont_game(string w)
{
    kaladont_file << w << endl;
}
void write_all_words()
{
    for (auto word : words_to_write)
    {
        write_word_to_kaladont_game(word);
    }
}
vector<Word> get_all_words_from_file()
{
    vector<Word> all_words;
    string line;
    ifstream kaladont_words("words.txt");
    if (kaladont_words.is_open())
    {
        while (getline(kaladont_words, line))
        {
            if (line.length() >= 2)
            {
                Word word;
                word.isUsed = false;
                word.text = line;
                word.letter_group = word.text.substr(0, 2);
                word.first_two = word.text.substr(0, 2);
                word.last_two = word.text.substr(word.text.length() - 2, 2);

                if (word.last_two.compare(KA) != 0)
                {
                    all_words.push_back(word);
                }
            }
        }
        kaladont_words.close();
        cout << "Done reading the file" << endl;
    }
    return all_words;
}

vector<Word> get_acceptable_words(string lastTwoLetters)
{
    // copy_if(all_words.begin(), all_words.end(), back_inserter(newlist), [lastTwoLetters](Word match)
    //         { return (match.text.substr(0, 2).compare(lastTwoLetters) == 0) && match.isUsed == false && match.text.length() > 2; });
    for (size_t row = 0; row < jagged_words.size(); ++row)
    {
        if (jagged_words[row][0].letter_group.compare(lastTwoLetters) == 0)
        {

            return jagged_words[row];
        }
    }
    vector<Word> newlist;
    return newlist;
}
void fill_nested_list_with_words_lists(vector<Word> &aceptable_list)
{
    //For every element in a list
    //Find last two letters
    //And based on those, get count of connectable words
    for (Word &word : aceptable_list)
    {
        word.num_of_connectable_words = get_acceptable_words_count(word.last_two);
    }
}
int get_acceptable_words_count(string last_two_letters)
{

    for (size_t row = 0; row < jagged_words.size(); ++row)
    {
        if (jagged_words[row][0].letter_group.compare(last_two_letters) == 0)
        {
            return jagged_words[row].size();
        }
    }

    return 0;
}
void set_is_used(string word)
{
    string last_two_letters = word.substr(0, 2);
#if 0
    for (Word &w : all_words)
    {
        auto found = find_if(all_words.begin(), all_words.end(), [&, word](Word ww)
                             { return ww.text.compare(word) == 0; });
        found->isUsed = true;
    }
#endif
    for (size_t row = 0; row < jagged_words.size(); ++row)
    {
        if (jagged_words[row][0].letter_group.compare(last_two_letters) == 0)
        {
            auto found = remove_if(jagged_words[row].begin(), jagged_words[row].end(), [&, word](Word &w)
                                   { return w.text.compare(word) == 0; });
            jagged_words[row].erase(std::remove_if(jagged_words[row].begin(), jagged_words[row].end(), [&, word](Word &w)
                                                   { return w.text.compare(word) == 0; }));
        }
    }
}
