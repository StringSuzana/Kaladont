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

vector<vector<Word>> jagged_words;
void read_jagged_vector()
{
    for (size_t row = 0; row < jagged_words.size(); ++row)
    {
        for (size_t col = 0; col < jagged_words[row].size(); ++col)
        {
            cout << jagged_words[row][col].letter_group << endl;
        }
       
    }
}
int main()
{
    int counter = 1;
    //all_words = get_all_words_from_file();
    make_vector_of_vectors();
    read_jagged_vector();
    auto starting_word = all_words[66];
    write_word_to_kaladont_game(starting_word.text);
    set_is_used(starting_word.text);

    words_to_write.push_back(starting_word.text);

    while (words_to_write.size() >= counter)
    {
        string next = choose_next_word(words_to_write[words_to_write.size() - 1]);
        write_word_to_kaladont_game(next);
        set_is_used(next);
        words_to_write.push_back(next);
        counter++;
    }

    return 0;
}
void make_vector_of_vectors()
{
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

                temp.push_back(word);
            }
            if (temp.size() > 0)
            {
              
                jagged_words.push_back(temp);
            }
        }
    }
}

string choose_next_word(string w)
{
    string last_two_letters = w.substr(w.length() - 2, 2);

    auto filtered = get_acceptable_words(last_two_letters);
    fill_nested_list_with_words_lists(filtered);

    sort(filtered.begin(), filtered.end(), [](Word &w, Word &ww)
         { return ww < w; });
    cout << filtered[0].num_of_connectable_words << endl;

    return filtered[0].text;
}
void write_word_to_kaladont_game(string w)
{
    cout << w << endl;
    kaladont_file << w << endl;
    //kaladont_file.close();
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
            if (line.length() > 2)
            {
                Word word;
                word.isUsed = false;
                word.text = line;
                word.letter_group = word.text.substr(0, 2);
                word.first_two = word.text.substr(0, 2);
                word.last_two = word.text.substr(word.text.length() - 2, 2);

                all_words.push_back(word);
            }
        }
        kaladont_words.close();
        cout << "Done reading the file" << endl;
    }
    return all_words;
}
vector<Word> get_acceptable_words(string lastTwoLetters)
{
    vector<Word> newlist;
    copy_if(all_words.begin(), all_words.end(), back_inserter(newlist), [lastTwoLetters](Word match)
            { return (match.text.substr(0, 2).compare(lastTwoLetters) == 0) && match.isUsed == false && match.text.length() > 2; });

    return newlist;
}
void fill_nested_list_with_words_lists(vector<Word> &aceptable_list)
{
    //For every element in a list
    //Find last two letters
    //And based on those, make a new list of connectable words
    for (Word &word : aceptable_list)
    {
        vector<Word> newlis = get_acceptable_words(word.text.substr(word.text.length() - 2, 2));
        word.num_of_connectable_words = newlis.size();
    }
    cout << "fill_nested_list_with_words_lists done" << endl;
}
void set_is_used(string word)
{
    for (Word &w : all_words)
    {
        auto found = find_if(all_words.begin(), all_words.end(), [&, word](Word ww)
                             { return ww.text.compare(word) == 0; });
        found->isUsed = true;
    }
    cout << "set_is_used done" << endl;
}
