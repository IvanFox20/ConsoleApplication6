#pragma once
#include <string>

class HashFunction2
{
public:
    int operator()(const std::string& s, int table_size) const
    {
        int key = table_size + 1;
        int hash_result = 0;
        for (int i = 0; i != s.size(); ++i)
            hash_result = (key * hash_result + s[i]) % table_size;
        hash_result = (hash_result * 2 + 1) % table_size;
        return hash_result;
    }
};

