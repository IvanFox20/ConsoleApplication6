#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <set>
#include <map>
#include "HashTable.h"
#include "Analizator.h"
#include "SyntaxAnaliz.h"
#include <deque>
#include <stack>
ofstream fout("Tree.txt");//синтаксический анализ
ofstream ffout("ListOfErrors.txt");//семантический
ofstream out("output.txt");
ofstream outt("output1.txt");
using namespace std;

HashTable<string> B;

class SemanticAnaliz
{
private:
    pair<string, string> begin;
    pair<string, string> end;
public:
    void SetBegin(pair<string, string> f)
    {
        this->begin = f;
    }
    void SetEnd(pair<string, string>f)
    {
        this->end = f;
    }
    void CheckBeginEnd()
    {
        ffout << "--Проверка разделов Begin End--" << endl;
        if (begin != end)
            ffout << "Program's name != end's name" << endl;
        else
            ffout << "Program's name = end's name" << endl;   
    }
    multiset<string>mts;
    map<string, int> AreThere;
    void CheckDescriptions()
    {
        ffout << "--Проверка раздела Descriptions--" << endl;
        for (auto it = mts.begin(); it != mts.end();)
        {
            if (AreThere.count(*it) == 0)
            {
                AreThere[*it] = 1;
            }
            if (mts.count(*it)>1)
            {
                ffout << "Переменная << " << *it << " >> была описана более 1 раза.." << endl;
                for (int i = 0; i < mts.count(*it); ++i)
                    ++it;
            }
            else
            {
                ++it;
            }
           
        }
    }
    
    set<string>st;
    void Operatorsss()//
    {
        ffout << "--Проверка раздела Operators--" << endl;
        for (auto i = st.begin(); i!=st.end(); ++i)
        {
            if (AreThere[*i] == 0)
            {
                ffout << "ERROR: Используемая переменная << " << *i << " >> не была описана в Descriptions" << endl;
            }
        }   
    }

    //для полиза раздела descriptions
    vector<string>vecforDescription;
    int countOfVarInEachDescr = 0;
    void descriptionsPOLIZ()
    {
        if (!vecforDescription.empty())
        {
            for (int i = vecforDescription.size() - 1; i >= 0; --i)
            {
                out << vecforDescription[i] << " ";
            }
            out << countOfVarInEachDescr <<" "<<"DECL"<<endl;
            vecforDescription.clear();
            countOfVarInEachDescr = 0;
        }
    }
    /*void writeProgram()
    {
        out << "PROGRAM" << " " << begin.first << endl;
    }*/
    void writeEnd()
    {
        out << "return" << " " << end.first << endl;
    }
    vector<vector<pair<string, string>>>vect;
    deque<pair<string,string>>dqforPOLIZoperators;
    void vivod()
    {
        vector<pair<string, string>>vec;
        for (int i = 0; i < dqforPOLIZoperators.size(); ++i)
        {
            
            
            if (dqforPOLIZoperators[i].first == "\n")
            {
                vect.push_back(vec);
                vec.clear();
            }
            else
            {
                
                if (i == dqforPOLIZoperators.size() - 1)
                {
                    vec.push_back(dqforPOLIZoperators[i]);
                    vect.push_back(vec);
                }
                else
                    vec.push_back(dqforPOLIZoperators[i]);
            }
            
        }
        map<string, int>mpOfOperators;
        

        stack<string>st;
        //здесь алгоритм дейкстра operators
        for (int i = 0; i < vect.size(); ++i)
        {
            
            mpOfOperators.insert(make_pair("=", 2));
            mpOfOperators.insert(make_pair("+", 3));
            mpOfOperators.insert(make_pair("-", 3));
            mpOfOperators.insert(make_pair("==", 2));//condition
            mpOfOperators.insert(make_pair("<", 2));
            mpOfOperators.insert(make_pair(">", 2));
            mpOfOperators.insert(make_pair(">=", 2));
            mpOfOperators.insert(make_pair("<=", 2));
            mpOfOperators.insert(make_pair("!=", 2));
            

            for (int j = 0; j < vect[i].size(); ++j)
            {
                
                //outt << vect[i][j].first << " ";
              
                    if (vect[i][j].second == "IntNum" || vect[i][j].second == "ID")
                    {
                        out << vect[i][j].first << " ";
                        continue;
                    }
                    else if (mpOfOperators[vect[i][j].first] != 0)
                    {
                        if (st.empty())
                        {
                            st.push(vect[i][j].first);
                        }
                        else
                        {
                            if (mpOfOperators[vect[i][j].first] > mpOfOperators[st.top()])
                            {
                                st.push(vect[i][j].first);
                            }
                            else
                            {
                                while (st.size() > 0&& mpOfOperators[vect[i][j].first] <= mpOfOperators[st.top()])
                                {
                                    out << st.top() << " ";
                                    st.pop();
                                }
                                //st.pop();
                                st.push(vect[i][j].first);
                            }
                        }
                        continue;

                    }
                    else if (vect[i][j].first == "(")
                    {
                        st.push(vect[i][j].first);
                        continue;
                    }
                    else if (vect[i][j].first == ")")
                    {
                        while (st.size() > 0 && st.top() != "(")
                        {
                            out << st.top() << " ";
                            st.pop();
                        }
                        st.pop();
                        continue;
                    }
                    else
                    {
                        while (!st.empty())
                        {
                            out << st.top() << " ";
                            st.pop();
                        }
                        out << vect[i][j].first << " ";
                    }


                

            }
            while (!st.empty())
            {
                out << st.top() << " ";
                st.pop();
            }
            out << endl;
        }
        
    }


   

};

class SyntaxAnaliz
{
    
public:
    
    int otstyp = 4;
    bool TypeOk;
    Analizator A;
    pair<string, string> f;
    string all_txt;
    SemanticAnaliz Semantic;

   
    void outputt(int otstyp,string name)
    {
        
        for (int i = 0; i < otstyp; ++i)
        {
            fout << " ";
        }
        fout << name << "\n";
    }
    void outputt(int otstyp, string name,string name2)
    {
        
        for (int i = 0; i < otstyp; ++i)
        {
            fout << " ";
        }
        fout << name <<" --> "<<name2<<"\n";

    }
    void giftAnalizatorLicsem(Analizator Aa)
    {
        A = Aa;
    }
   
    void Program()
    {  
       outputt(0, "Program");

       Begin();
       //Semantic.writeProgram();
       Descriptions();
       Semantic.CheckDescriptions();//проверка на повторяющиеся переменные
       Operators();
       Semantic.Operatorsss();//так как у меня один тип int, тут только смотрим описаны ли все переменные
       Semantic.vivod();
       End();
       Semantic.writeEnd();
       Semantic.CheckBeginEnd();
       cout << "NO ERROR" << endl;
       B.fout();//вывод хэш таблицы
    }
    void Begin()
    {
        otstyp = 4; 
        outputt(otstyp, "--Begin--");
        
        f = A.Licsema();
        B.Add(f.first, f.second);//first = значение, second = тип лексемы
        if (f.first == "int" && f.second == "KEY WORDS")
        {
            otstyp+=5;
            outputt(otstyp, f.second,f.first);

            f = A.Licsema();
            
            B.Add(f.first, f.second);
            if (f.second != "FUNCTION NAME")
            {
                cout << "ERROR " << f.first << endl;
                cout << "Ошибка произошла в разделе Begin, тип лексемы должен быть ID" << endl;
                exit(0);
            }
            else
            {
                Semantic.SetBegin(f);//берем Program ____
                otstyp+=10;
                outputt(otstyp, f.second,f.first);    
            }
        }
        else
        {
            cout << "ERROR " << f.first << endl;
            cout << "Ошибка произошла в разделе Begin, должно быть PROGRAM" << endl;
            exit(0);
        }

        f = A.Licsema();
        B.Add(f.first, f.second);
        if (f.second != "brace" && f.first != "{")
        {
            cout << "ERROR " << f.first << endl;
            cout << "Ошибка произошла в разделе Begin, отсутсвует {" << endl;
            exit(0);
        }


    }
    void End()
    {
       
        otstyp = 4;
        outputt(otstyp, "--End--");
        f = A.Licsema();
        B.Add(f.first, f.second);//first = значение, second = тип лексемы
        if (f.first == "return" && f.second == "KEY WORDS")
        {
            otstyp += 5;
            outputt(otstyp, f.second, f.first);
            f = A.Licsema();
            B.Add(f.first, f.second);
            
            if (f.second != "ID")
            {
                cout << "ERROR " << f.first << endl;
                cout << "Ошибка произошла в разделе End, тип лексемы должен быть ID" << endl;
                exit(0);
            }
            else
            { 
                Semantic.SetEnd(f);//End ___
                otstyp += 10;
                outputt(otstyp, f.second, f.first);
            }
        }
        else
        {
            cout << "ERROR " << f.first << endl;
            cout << "Ошибка произошла в разделе End" << endl;
            exit(0);
        }
        f = A.Licsema();
        B.Add(f.first, f.second);
        if (f.second != "semicolon")
        {
            cout << "ERROR " << f.first << endl;
            cout << "Ошибка произошла в разделе End, пропущена точка с запятой" << endl;
            exit(0);
        }
        f = A.Licsema();
        B.Add(f.first, f.second);
        if (f.first != "}" && f.second != "brace")
        {
            cout << "ERROR " << f.first << endl;
            cout << "Ошибка произошла в разделе End, пропущена закрывающая скобка }" << endl;
            exit(0);
        }
    }
    void Descriptions()
    {
        otstyp = 4;
        outputt(otstyp, "--Descriptions--");
        Descr();//1 раз или..
        while (true)
        {
            f = A.Licsema();
            if (f.first == "int" && f.second == "KEY WORDS")//значит Type() будет
            {
                A.prev();
                Descr();
            }
            else
            {
                A.prev();
                break;
            }
        }
    }

    void Descr()
    {
       
        outputt(7, "Descr");
        TypeOk = true;
        Type();
        if(TypeOk)
            outputt(15, "VarList->");
        VarList();//если type - true, тогда продолжаем, если false тогда пропускаем 
        Semantic.descriptionsPOLIZ();
        f = A.Licsema();
        B.Add(f.first, f.second);
        if (!(f.first == ";" && f.second == "semicolon"))
        {
            TypeOk = false;
            cout << "ERROR " << f.first << endl;
            cout << "Ошибка произошла в разделе Descriptions, пропущена точка с запятой" << endl;
            exit(0);
        }
    }

    void Type()
    {
        otstyp = 4;
        outputt(otstyp+4, "Type");
        f = A.Licsema();
        B.Add(f.first, f.second);//first = значение, second = тип лексемы
        if (!(f.first == "int" && f.second == "KEY WORDS"))
        {
            TypeOk = false;
            cout << "ERROR " << f.first << endl;
            cout << "Ошибка произошла в разделе Descriptions, должно быть int, тип лексемы должен быть - KEY WORDS" << endl;
            exit(0);   
        }
        else
        {
            Semantic.vecforDescription.push_back(f.first);
            otstyp += 7;
            outputt(otstyp, f.second, f.first);
        }


    }
    void VarList()
    {
        otstyp = 4;
        
        f = A.Licsema();
        B.Add(f.first, f.second);
        if (f.second == "ID")
        {
            Semantic.mts.insert(f.first);
            Semantic.vecforDescription.push_back(f.first);
            Semantic.countOfVarInEachDescr++;
            outputt(otstyp+19,f.second, f.first);
            f = A.Licsema();
            B.Add(f.first, f.second);
            if (f.first == ",")
            {
                outputt(otstyp + 19, f.second, f.first);
                VarList();
            }
            else
            {
                A.prev();//переход к предыдущей лексеме
            }
        }
        else
        {
            cout << "ERROR " << f.first << endl;
            cout << "Ошибка произошла в разделе Descriptions, тип лексемы должен быть ID" << endl;
            exit(0);
        }
    }
    int ott = 0;
    bool time = true;
    void Operators()
    {
        //otstyp = 4;
        if (time)
        {
            outputt(4, "--Operators--");
            time = false;
        }
        else
        outputt(ott+=8, "--Operators--");
        Op();
        while (true)
        {
            f = A.Licsema();
            if (f.second == "ID")
            {
                A.prev();
                Op();
            }
            else if (f.first == "if")
            {
                A.prev();
                Op();
            }
            else
            {
                A.prev();
                break;
            }
        }

    }
    pair<string, string>perehod;
    int chislo = 0;//p1 p2 p3 p4 для переходов, всегда разные
    void Op()
    {
        string chisloo = to_string(chislo);
        if (perehod.first!="")//определяем будет спуск вниз или нет
        {
            if (perehod.first[1] != 'p')
            {
                perehod.first = "\n";
                perehod.second = "WORK";
                Semantic.dqforPOLIZoperators.push_back(perehod);
                chislo = 0;
            }
        }
        outputt(ott+5, "Op");
        f = A.Licsema();
        B.Add(f.first, f.second);
        if (f.second == "ID")//значит будет присваивание
        {
            perehod.first = "Perehod";
            perehod.second = "WORK";
            Semantic.dqforPOLIZoperators.push_back(f);
            Semantic.st.insert(f.first);
            outputt(ott + 7, f.second, f.first);
            f = A.Licsema();
            B.Add(f.first, f.second);
            
            if (f.first == "=")
            {
                outputt(ott + 7, f.second, f.first);
                Semantic.dqforPOLIZoperators.push_back(f);
                Expr();

                f = A.Licsema();
                B.Add(f.first, f.second);

                if (f.first != ";")
                {
                    cout << "ERROR " << f.first << endl;
                    cout << "Ошибка произошла в разделе Operators, пропущена точка с запятой ;" << endl;
                }
            }
            else
            {
                cout << "ERROR " << f.first << endl;
                cout << "Ошибка произошла в разделе Operators, должен быть оператор = " << endl;
                exit(0);
            }
            
        }
        else if (f.first == "if")
        {
            outputt(ott + 7, f.second, f.first);
            f = A.Licsema();
            B.Add(f.first, f.second);
            if (f.first != "(")
            {
                cout << "ERROR " << f.first << endl;
                cout << "Ошибка произошла в разделе Operators, пропущена открыващая скобка ( после if " << endl;
                exit(0);
            }
            Condition();
            f = A.Licsema();
            B.Add(f.first, f.second);
            if (f.first == ")")
            {
                chislo++;
                chisloo = to_string(chislo);//чтобы у p менялась всегда цифра
                perehod.first = " p"+chisloo+" !F "  ;// p1 !F
                perehod.second = "WORK";
                Semantic.dqforPOLIZoperators.push_back(perehod);
                outputt(ott + 7, f.second, f.first);
                Op();
                f = A.Licsema();
                B.Add(f.first, f.second);
                if (f.first == "else")
                {
                    chislo++;
                    chisloo = to_string(chislo);
                    perehod.first = " p" + chisloo + " ! ";
                    perehod.second = "WORK";
                    Semantic.dqforPOLIZoperators.push_back(perehod);
                    outputt(ott + 7, f.second, f.first);
                    Op();
                }
                else
                {
                    A.prev();
                    return;
                }
            }
            else
            {
                cout << "ERROR " << f.first << endl;
                cout << "Ошибка произошла в разделе Operators, должен быть оператор THEN " << endl;
                exit(0);
            }
        }
        else
        {

            cout << "ERROR " <<  endl;
            cout << "Хоть что-то должно быть в разделе Operators,пустым он не может быть " << endl;
            exit(0);
        }
        
    }

    void RelationOperators()
    {
        outputt(ott + 15, "RelationOperators");
        vector<string>RelOps = { "==","!=" ,">=" ,"<=" ,">" ,"<" };
        f = A.Licsema();
        B.Add(f.first, f.second);
        if (!(find(RelOps.begin(), RelOps.end(), f.first) != RelOps.end()))//если нашего оператора нет среди этих, тогда ошибка
        {
            A.prev();
            cout << "ERROR " << f.first << endl;
            cout << "Ошибка произошла в разделе Operators, тип лексемы должен быть - оператор сравнений" << endl;
            exit(0);
        }
        else
        {
            Semantic.dqforPOLIZoperators.push_back(f);
            outputt(ott + 20, f.second,f.first);
        }
    }

    void Condition()
    {
        outputt(ott + 13, "Condition");

        Expr();
        RelationOperators();
        Expr();
    }

    void SimpleExpr()
    {
        outputt(ott + 17, "SimpleExpr");
        f = A.Licsema();
        B.Add(f.first, f.second);
        if (f.second == "ID")
        {
            Semantic.dqforPOLIZoperators.push_back(f);
            Semantic.st.insert(f.first);
            outputt(ott + 20, f.second, f.first);
            return;
        }   
        else if (f.second == "IntNum")
        {
            Semantic.dqforPOLIZoperators.push_back(f);
            outputt(ott + 20, "Const", f.first);
            return;
        }
        else if (f.first == "(")
        {
            Semantic.dqforPOLIZoperators.push_back(f);
            outputt(ott + 20, f.second, f.first);
            Expr();
            f = A.Licsema();
            if (f.first != ")")
            {
                cout << "ERROR " << f.first << endl;
                cout << "Ошибка произошла в разделе Operators, EXPR должно закрываться скобкой" << endl;
                exit(0);
            }
            else
            {
                Semantic.dqforPOLIZoperators.push_back(f);
                outputt(ott + 20, f.second, f.first);
                return;
            }
                
        }
        else
        {
            cout << "ERROR " << f.first << endl;
            cout << "Ошибка произошла в разделе Operators, тип лексемы не подходит, должно быть ID или IntNum или ( Expr )" << endl;
            exit(0);
        }
    }
    void Expr()
    {
        outputt(ott + 15, "Expr");
        SimpleExpr();
        while (true)
        {
            f = A.Licsema();
            B.Add(f.first, f.second);
            if (f.first == "+" || f.first == "-")
            {
                Semantic.dqforPOLIZoperators.push_back(f);

                outputt(ott + 15, f.second,f.first);
                Expr();
            }
            else
            {
                A.prev();
                break;
            }
        }
    }

};







int main() {
    setlocale(LC_ALL, "rus");
    ifstream fin("input.txt");
    string all_txt;
    string buffer;
    while (!fin.eof()) 
    {
        getline(fin, buffer);
        all_txt += buffer + "\n";
    }
    SyntaxAnaliz BB;
    Analizator A(all_txt);
    BB.giftAnalizatorLicsem(A); 
    BB.Program();
}