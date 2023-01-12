#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;



class Analizator {
public:
    string all_txt;
    string buf;
    int num = 0;
    int pos = 0;
    
    vector <string> KW = { "FUNCTION NAME","return","int" };
    vector <string> SEP = { "\n","\t" };
    vector <string> Raz = { "=" , "+","-","if","THEN","else","==","!=" ,">=" ,"<=" ,">" ,"<" };
    Analizator() {};
    Analizator(string all_txt) {
        this->all_txt = all_txt;
    }
    string Go_next() {
        int i = num;

        string buf;
        while (all_txt[i] != '\n' && all_txt[i] != ' ') {
            buf += all_txt[i];
            i++;

        }
        
        pos = i + 1 - num;//
        num = i + 1;
        return buf;
    };
    int DKA() {
        int state = 1;
        buf = Go_next();
        bool isnum = false;
        pair <string, string> answer;
        if (isdigit(buf[0])) {
            state = 2;
        }
        else if (isalpha(buf[0])) {
            state = 3;
        }
        else {
            return 4;
        }
        switch (state)
        {
        case 2:
            for (int i = 0; i < buf.size(); i++) {
                if (!isdigit(buf[i])) {
                    return 4;
                }
            }
            return 2;
        case 3:
            for (int i = 0; i < buf.size(); i++) {
                if (!isalpha(buf[i])) {
                    return 4;
                }
            }
            return 3;
        }
    }
    void prev()
    {
        num -= pos;
    }
    pair <string, string> Licsema() {
        pair <string, string> answer;
        int a = DKA();
        if (a == 2) {
            answer.first = buf;
            answer.second = "IntNum";
            return answer;
        }
        if (a == 3) {
            if (find(KW.begin(), KW.end(), buf) != KW.end()) {
                answer.first = buf;
                answer.second = "KEY WORDS";
                return answer;
            }
            else if (find(Raz.begin(), Raz.end(), buf) != Raz.end())//if then else .eg. and td..
            {
                answer.first = buf;
                answer.second = "OPERATOR";
                return answer;
            }
            else
            {
                answer.first = buf;
                answer.second = "ID";
                return answer;
            }
        }
        if (a == 4) {
            if (find(Raz.begin(), Raz.end(), buf) != Raz.end()) {//+ - =
                answer.first = buf;
                answer.second = "OPERATOR";
                return answer;
            }
            bool isFunctionName = true;
            for (int i = 0; i < buf.size() - 3; i++)
            {
                if (!isalpha(buf[i]))
                {
                    isFunctionName = false;
                    break;
                }
            }
            if (isFunctionName)
            {
                if (buf[buf.size() - 2] == '(' && buf[buf.size() - 1] == ')');
                {
                    answer.first = buf;
                    answer.second = "FUNCTION NAME";
                    return answer;
                }
            }
            else if (buf == ",")
            {
                answer.first = buf;
                answer.second = "Comma";
                return answer;
            }
            else if (buf == "(" || buf == ")")
            {
                answer.first = buf;
                answer.second = "bracket";
                return answer;
            }
            else if (buf == "{" || buf == "}")
            {
                answer.first = buf;
                answer.second = "brace";
                return answer;
            }
            else if (buf == ";")
            {
                answer.first = buf;
                answer.second = "semicolon";
                return answer;
            }
            else {
                answer.first = buf;
                answer.second = "ERROR";
                return answer;
            }
        }

    }
    bool is_empty() {
        if (num < all_txt.size()) {
            return true;
        }
        else {
            return false;
        }
    }
};
