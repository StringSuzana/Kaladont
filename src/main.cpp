#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
class Word
{
public:
    string text;
    bool isUsed;
    vector<Word> words;
};
bool operator<(const Word &s1, const Word &s2)
{
    if (s1.words.size() < s2.words.size())
        return true;
    else
        return false;
}

ofstream kaladont_file("game_kaladont.txt");
vector<Word> all_words;
vector<string> words_to_write;

vector<Word> get_all_words();
vector<Word> get_acceptable_words(string lastTwoLetters);
void fill_nested_list_with_words_lists(vector<Word> &aceptable_list);

void write_word_to_kaladont_game(string w);
string choose_next_word(string w);
int main()
{
    int counter = 1;
    all_words = get_all_words();

    auto starting_word = all_words[65];
    write_word_to_kaladont_game(starting_word.text);

    words_to_write.push_back(starting_word.text);

    while (words_to_write.size() >= counter)
    {
        string next = choose_next_word(words_to_write[words_to_write.size() - 1]);
        write_word_to_kaladont_game(next);

        words_to_write.push_back(next);
        counter++;
    }

    return 0;
}

string choose_next_word(string w)
{
    string last_two_letters = w.substr(w.length() - 2, 2);

    auto filtered = get_acceptable_words(last_two_letters);
    fill_nested_list_with_words_lists(filtered);

    sort(filtered.begin(), filtered.end(), [](Word &w, Word &ww)
         { return ww < w; });

    return filtered[0].text;
}
void write_word_to_kaladont_game(string w)
{
    cout << w << endl;
    kaladont_file << w << endl;
    //kaladont_file.close();
}

vector<Word> get_all_words()
{
    vector<Word> all_words;
    string line;
    ifstream kaladont_words("words.txt");
    if (kaladont_words.is_open())
    {
        while (getline(kaladont_words, line))
        {
            Word word;
            word.isUsed = false;
            word.text = line;
            all_words.push_back(word);
        }
        kaladont_words.close();
        cout << "Done reading the file" << endl;
    }
    return all_words;
}
vector<Word> get_acceptable_words(string lastTwoLetters)
{
    vector<Word> newlist;
    newlist.clear();
    copy_if(all_words.begin(), all_words.end(), back_inserter(newlist), [lastTwoLetters](Word match)
            { return (match.text.substr(0, 2).compare(lastTwoLetters) == 0) && match.isUsed == false && match.text.length() > 2 && match.text.substr(match.text.length() - 2, 2).compare(lastTwoLetters) != 0; });

    return newlist;
}
void fill_nested_list_with_words_lists(vector<Word> &aceptable_list)
{
    // za svaki element u listi
    //pronađi zadnja dva slova
    //i na temelju ta dva slova napravi listu
    for (Word &word : aceptable_list)
    {
        vector<Word> newlis = get_acceptable_words(word.text.substr(word.text.length() - 2, 2));
        for (auto w : newlis)
        {
            word.words.push_back(w);
        }
    }
}
