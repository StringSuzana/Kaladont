#pragma once
#include <string>

class Word
{
public:
    std::string text;
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
