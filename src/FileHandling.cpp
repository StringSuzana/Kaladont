#if 0
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include "Word.hpp"

void create_all_files();
void populate_all_files(vector<Word> &all_words);

static void appendLineToFile(string filepath, string line)
{
    std::ofstream file;
    file.open(filepath, std::ios::out | std::ios::app);
    file << line << std::endl;
}
void populate_all_files(vector<Word> &all_words)
{
    for (auto &w : all_words)
    {
        string folder = "groups/";
        string file_name = (w.letter_group).append(".txt");
        appendLineToFile(folder.append(file_name), w.text);
    }
}
void remove_empty_files(){
//if empty
//remove("groups/filename.txt");
}
void remove(string filename){
    if( remove(filename.c_str()) != 0 )
    perror( "Error deleting file" );
  else
    puts( "File successfully deleted" );
}
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
            two_letters = (abeceda.substr(i, 1)).append(abeceda.substr(j, 1));
            two_letters = two_letters.append(".txt");
            string full_path = folder.append(two_letters);
            std::ofstream file(full_path);
            file.close();
        }
    }
}
#endif