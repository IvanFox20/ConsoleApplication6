/*#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include "HashTable.h"
#include "Analizator.h"

ofstream fout("output.txt");


using namespace std;
class SyntaxAnaliz
{

public:

    int otstyp = 4;
    bool TypeOk;
    Analizator A;
    HashTable<string> B;
    pair<string, string> f;
    string all_txt;
    SyntaxAnaliz() {

    };
    void outputt(int otstyp, string name)
    {

        for (int i = 0; i < otstyp; ++i)
        {
            fout << " ";
        }
        fout << name << "\n";
    }
    void outputt(int otstyp, string name, string name2)
    {

        for (int i = 0; i < otstyp; ++i)
        {
            fout << " ";
        }
        fout << name << " --> " << name2 << "\n";

    }
    void gift(Analizator Aa)
    {
        A = Aa;
    }

    void Program()
    {
        outputt(0, "Program");

        Begin();
        Descriptions();
        Operators();
        End();
        cout << "NO ERROR" << endl;
        //B.fout();
    }
    void Begin()
    {
        otstyp = 4;
        outputt(otstyp, "--Begin--");

        f = A.Licsema();
        B.Add(f.first, f.second);//first = значение, second = тип лексемы
        if (f.first == "PROGRAM" && f.second == "KEY WORDS")
        {
            otstyp += 5;
            outputt(otstyp, f.second, f.first);

            f = A.Licsema();
            B.Add(f.first, f.second);
            if (f.second != "ID")
            {
                cout << "ERROR " << f.first << endl;
                cout << "Ошибка произошла в разделе Begin, тип лексемы должен быть ID" << endl;
                exit(0);
            }
            else
            {
                otstyp += 10;
                outputt(otstyp, f.second, f.first);
            }
        }
        else
        {
            cout << "ERROR " << f.first << endl;
            cout << "Ошибка произошла в разделе Begin, должно быть PROGRAM" << endl;
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
        if (TypeOk)
            outputt(15, "VarList->");
        VarList();//если type - true, тогда продолжаем, если false тогда пропускаем 

    }

    void Type()
    {
        otstyp = 4;
        outputt(otstyp + 4, "Type");
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

            outputt(otstyp + 19, f.second, f.first);
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
            outputt(ott += 8, "--Operators--");
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
    void Op()
    {

        outputt(ott + 5, "Op");
        f = A.Licsema();
        B.Add(f.first, f.second);
        if (f.second == "ID")
        {
            outputt(ott + 7, f.second, f.first);
            f = A.Licsema();
            B.Add(f.first, f.second);

            if (f.first == "=")
            {
                outputt(ott + 7, f.second, f.first);
                Expr();
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
            Condition();
            f = A.Licsema();
            B.Add(f.first, f.second);
            if (f.first == "THEN")
            {
                outputt(ott + 7, f.second, f.first);
                Operators();
                f = A.Licsema();
                B.Add(f.first, f.second);
                if (f.first == "else")
                {
                    outputt(ott + 7, f.second, f.first);
                    Operators();
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

            cout << "ERROR " << endl;
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
            outputt(ott + 20, f.second, f.first);
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
            outputt(ott + 20, f.second, f.first);
            return;
        }
        else if (f.second == "IntNum")
        {
            outputt(ott + 20, "Const", f.first);
            return;
        }
        else if (f.first == "(")
        {
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
                outputt(ott + 15, f.second, f.first);
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
*/