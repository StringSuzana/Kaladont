#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
//Shif, alt, f
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
    cout << s1.text;
    if (s1.words.size() < s2.words.size())
        return true;
    else
        return false;
}

ofstream kaladont_file("game_kaladont.txt");
vector<Word> all_words;
vector<string> words_to_write;

vector<Word> get_all_words();
vector<Word> fill_nested_words_lists(string lastTwoLetters);
vector<Word> fill_nested_list_with_words_lists(vector<Word> aceptable_list);
vector<Word> fill (string last_two_letters);
void write_word_info(Word w);
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
    auto filtered = fill_nested_words_lists(last_two_letters);
  //  auto final_list = fill_nested_list_with_words_lists(filtered);

    auto starts_with_letters = [last_two_letters](Word match)
    {
        return (match.text.substr(0, 2).compare(last_two_letters) == 0) && match.isUsed == false && match.text.size() > 4 && match.text.substr(match.text.length() - 2, 2).compare(last_two_letters) != 0 //no palindroms
               && match.words.size() > 2;
    };
    //sort(filtered.begin(), filtered.end());

    auto word = find_if (filtered.begin(), filtered.end(),
                         [last_two_letters](Word match)
    {
        return (match.text.substr(0, 2).compare(last_two_letters) == 0) && match.isUsed == false && match.text.size() > 4 && match.text.substr(match.text.length() - 2, 2).compare(last_two_letters) != 0 //no palindroms
               && match.words.size() > 2;
    });

    while (word != filtered.end()){
    cout << word->text << endl;

    }

    return word->text;
}
void write_word_to_kaladont_game(string w)
{
    kaladont_file << w << endl;
    //kaladont_file.close();
}
void write_word_info(Word w)
{
    ofstream sorted_file("sorted_words.txt");
    sorted_file << "WORD: " << w.text << endl;
    sorted_file << "ADDRESS: " << &w.text << endl;
    sorted_file << "nested words count: " << w.words.size() << endl;
    for (Word nested : w.words)
    {
        sorted_file << "====" << nested.text << " ===> " << &nested << endl;
    }
    sorted_file.close();
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
        cout << "Done" << endl;
    }
    return all_words;
}
vector<Word> fill_nested_words_lists(string lastTwoLetters)
{
    vector<Word> newlist;
    newlist.clear();
    copy_if(all_words.begin(), all_words.end(), back_inserter(newlist), [lastTwoLetters](Word match)
            { return (match.text.substr(0, 2).compare(lastTwoLetters) == 0) && match.isUsed == false && match.text.length() > 2 && match.text.substr(match.text.length() - 2, 2).compare(lastTwoLetters) != 0; });
    if (newlist.size() > 0)
    {
        for (auto nested : newlist)
        {
            string last_two_letters = nested.text.substr(nested.text.length() - 2, 2);
            auto starts_with_letters = [last_two_letters](Word match)
            {
                return (match.text.substr(0, 2).compare(last_two_letters) == 0) && match.isUsed == false && match.text.length() > 2 && match.text.substr(match.text.length() - 2, 2).compare(last_two_letters) != 0 //no palindroms
                       && match.words.size() > 2;
            };
            vector<Word> nested_words;
            copy_if(all_words.begin(), all_words.end(),
                    back_inserter(nested_words), starts_with_letters);

            nested.words = nested_words;
        }
    }

    return newlist;
}
// vector<Word> fill_nested_list_with_words_lists(vector<Word> aceptable_list)
// {
//     // za svaki element u listi
//     //pronađi zadnja dva slova
//     //i na temelju ta dva slova napravi listu
//     for (auto word : aceptable_list)
//     {
//         word.words = fill(word.text.substr(word.text.length() - 2, 2));
//     }

//     return aceptable_list;
// }
// vector<Word> fill (string last_two_letters){
//      vector<Word> newlist;
//     newlist.clear();
//     copy_if(all_words.begin(), all_words.end(), back_inserter(newlist), [last_two_letters](Word match)
//             { return (match.text.substr(0, 2).compare(last_two_letters) == 0) && match.isUsed == false && match.text.length() > 2 && match.text.substr(match.text.length() - 2, 2).compare(last_two_letters) != 0; });
//    return newlist;
// }