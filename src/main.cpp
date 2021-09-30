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
void write_all_words();
void create_all_files()
{
    string alphabet{"abcdefghijklmnopqrstuvwxyz"};
    std::wstring abeceda= L"abcćčdđefghijklmnopqrsštuvwxyzž";

    for (int i = 0; i < abeceda.length(); i++)
    {
        for (int j = 0; j < abeceda.length(); j++)
        {
            string folder = "groups/";
            string two_letters = "";
            two_letters =(abeceda.substr(i, 1)).append(abeceda.substr(j, 1));
            two_letters = two_letters.append(".txt");
            string full_path = folder.append(two_letters);
            std::ofstream file(full_path);
            file.close();
        }
    }
}
int main()
{
    int counter = 1;
    //all_words = get_all_words_from_file();
    make_vector_of_vectors();

    auto starting_word = jagged_words[1][3];
    write_word_to_kaladont_game(starting_word.text);
    set_is_used(starting_word.text);

    words_to_write.push_back(starting_word.text);

    while (words_to_write.size() >= counter && words_to_write[words_to_write.size() - 1].compare(KALADONT) != 0)
    {
        string next = choose_next_word(words_to_write[words_to_write.size() - 1]);
        //write_word_to_kaladont_game(next);
        set_is_used(next);
        words_to_write.push_back(next);
        counter++;
    }
    write_all_words();
    kaladont_file.close();
    return 0;
}
void make_vector_of_vectors()
{
    string alphabet{"abcdefghijklmnopqrstuvwxyz"};
    std::wstring abeceda= L"abcćčdđefghijklmnopqrsštuvwxyzž";

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
    if (filtered.size() == 0)
    {
        return KALADONT;
    }
    fill_nested_list_with_words_lists(filtered);

    sort(filtered.begin(), filtered.end(), [](Word &w, Word &ww)
         { return ww < w; });
    // cout << filtered[0].num_of_connectable_words << endl;

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
    // copy_if(all_words.begin(), all_words.end(), back_inserter(newlist), [lastTwoLetters](Word match)
    //         { return (match.text.substr(0, 2).compare(lastTwoLetters) == 0) && match.isUsed == false && match.text.length() > 2; });
    for (size_t row = 0; row < jagged_words.size(); ++row)
    {
        if (jagged_words[row][0].letter_group.compare(lastTwoLetters) == 0)
        {
            for (size_t col = 0; col < jagged_words[row].size(); ++col)
            {
                if (jagged_words[row][col].isUsed == false && jagged_words[row][col].last_two.compare(KA) != 0)
                {
                    newlist.push_back(jagged_words[row][col]);
                    //  cout << jagged_words[row][col].text << " ";
                }
            }
        }
    }

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
    // cout << "fill_nested_list_with_words_lists done" << endl;
}
void set_is_used(string word)
{
    // for (Word &w : all_words)
    // {
    //     auto found = find_if(all_words.begin(), all_words.end(), [&, word](Word ww)
    //                          { return ww.text.compare(word) == 0; });
    //     found->isUsed = true;
    // }
    for (size_t row = 0; row < jagged_words.size(); ++row)
    {
        if (jagged_words[row][0].letter_group.compare(word.substr(0, 2)) == 0)
        {
            for (size_t col = 0; col < jagged_words[row].size(); ++col)
            {
                if (jagged_words[row][col].text.compare(word) == 0)
                {
                    jagged_words[row][col].isUsed = true;
                    // cout << "set_is_used done" << endl;
                }
            }
        }
    }
}
