#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <vector>

//#include "lecture.h"

using namespace std;



bool compare(string arr[], int tam, string pal)
{
    for (int i = 0; i < tam; i++)
    {
        if (arr[i] == pal)
        {
            return true;
        }
    }
    return false;
}

bool isF(const char *abs)
{
    if (*abs == '\n' || *abs == '\t')
    {
        return true;
    }
    return false;
}

class anLexico
{
private:
    string ope[8] = {"++", "-", "*", "+", "->", "<->"};
    string delim[5] = {"}}", "ENDIF", "ENDEIF", "ENDEL"};
    string reserved[19] = {"FUNCTION", "IF", "ELSEIF", "ELSE", "RETURN", "addNoise", "reduceNoise", "colorRi",
                           "compare", "read", "save", "rotate", "combine", "solar", "main:", "PRINT", "then", "ENDIF", "scale"};
    string id = "|";
    string str[2] = {"'", "%"};
    string num[10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

public:
    void readFile(string);
    bool isDigit(const char *);
    bool isWord(const char *);
    void anTokens(string, vector<string> &, vector<string> &);
    char peekChar(const char *s, int ss);
};

// CPP

bool anLexico::isWord(const char *ch)
{
    if ((*ch >= 'a' && *ch <= 'z') || (*ch >= 'A' && *ch <= 'Z'))
    {
        return true;
    }
    return false;
}

bool anLexico::isDigit(const char *ch)
{
    if (*ch >= '0' && *ch <= '9')
    {
        return true;
    }
    return false;
}

char anLexico::peekChar(const char *s, int ss)
{
    return *(s + ss);
}

void anLexico::anTokens(string source, vector<string> &result, vector<string> &to_parse)
{
    //STACK FOR CODE
    vector<vector<string>> backEnd;

    int cont;
    bool pt = false;
    ifstream lectureF(source);
    // cout << "FLAG " << '\n';
    bool salto = false;
    std::string str((std::istreambuf_iterator<char>(lectureF)), std::istreambuf_iterator<char>());
    string lastE;
    // cout << "FLAG " << '\n';
    if (lectureF.is_open())
    {
        const char *ch;
        lastE = *ch;
        ch = str.c_str();
        int line;
        line = 1;
        while (*ch != lectureF.eof())
        {
            // cout << *ch << " ";
            // cout << line << " BOOL " << *ch << pt << '\n';
            cont = result.size();
            // cout << "CH:-" << *ch  << "-"<< '\n';
            // lectureF >> ch;
            if (*ch == '.' && pt == true)
            {
                pt = false;
                ch++;
                to_parse.push_back(".");
                continue;
            }
            else if (*ch == ' ' || *ch == '\t')
            {
                ch++;
                continue;
            }
            else if (*ch == '\n')
            {
                ch++;
                line++;
                // to_parse.push_back("n");
                continue;
            }
            else
            {

                bool fnd = true;
                string aux;
                switch (*ch)
                {
                case '+':
                    if (peekChar(ch, 1) == '+')
                    {
                        ch += 2;
                        result.push_back("<TOKEN: OP, LEXEMA: ++  >");
                        to_parse.push_back("plusplus");
                        salto = true;
                    }
                    else if (peekChar(ch, 1) == ' ' || isDigit(ch) || isWord(ch))
                    {
                        ch += 1;
                        result.push_back("<TOKEN: OP, LEXEMA: + >");
                        to_parse.push_back("plus");
                    }
                    else
                    {
                        ch++;
                        result.push_back("ERROR: " + line);
                    }

                    break;
                case '-':
                    if (peekChar(ch, 1) == ' ' || isDigit(ch) || isWord(ch))
                    {
                        ch++;
                        result.push_back("<TOKEN: OP, LEXEMA: -- >");
                        to_parse.push_back("minus");
                    }
                    else if (peekChar(ch, 1) == ' ' || isDigit(ch) || isWord(ch))
                    {
                        ch += 1;
                        result.push_back("<TOKEN: OP, LEXEMA: - >");
                        to_parse.push_back("minus");
                    }
                    else
                    {
                        ch++;
                        result.push_back("ERROR: " + line);
                    }

                    break;
                case '*':
                    if (peekChar(ch, 1) == ' ' || isDigit(ch) || isWord(ch))
                    {
                        ch++;
                        result.push_back("<TOKEN: OP, LEXEMA: * >");
                        to_parse.push_back("*");
                    }
                    else
                    {
                        ch++;
                        result.push_back("ERROR: " + line);
                    }

                    break;

                case '<':
                    if (peekChar(ch, 1) == '-')
                    {
                        ch += 2;
                        result.push_back("<TOKEN: OP, LEXEMA: <- >");
                        to_parse.push_back("<-");
                    }
                    else
                    {
                        ch++;
                        result.push_back("ERROR: " + line);
                    }
                    break;
                case '=':
                    pt = true;
                    if (peekChar(ch, 1) == '=')
                    {
                        ch += 2;
                        result.push_back("<TOKEN: OP, LEXEMA: ++ >");
                        to_parse.push_back("assignassign");
                    }
                    else if (peekChar(ch, 1) == ' ' || isDigit(ch) || isWord(ch))
                    {
                        ch += 1;
                        result.push_back("<TOKEN: OP, LEXEMA: = >");
                        to_parse.push_back("assign");
                    }
                    else
                    {
                        ch++;
                        result.push_back("ERROR: " + line);
                    }

                    break;
                case '\"':

                    aux = *ch;

                    while (fnd)
                    {
                        ch++;

                        if (*ch == '\"')
                        {
                            // cout << "ENTRE FINAL "<< '\n';
                            // cout << aux << '\n';
                            aux += *ch;
                            ch++;
                            result.push_back("<TOKEN: STRING, LEXEMA: " + aux + "  >");
                            to_parse.push_back("string");
                            fnd = false;
                            break;
                            /*while(*ch == ' ')
                            {
                                ch++;
                            }
                            if(*ch == '.')
                            {

                                aux = *ch;
                                result.push_back("<TOKEN: FIN, LEXEMA: " + aux + "  >");
                                to_parse.push_back(aux);
                            }
                            else
                            {
                                result.push_back("<ERROR: " + to_string(line) + ">");
                            }*/
                        }
                        else if (*ch == lectureF.eof() || isF(ch))
                        {
                            // cout << "PRINT" << aux << '\n';
                            result.push_back("<ERROR: " + to_string(line) + ">");
                            fnd = false;
                        }
                        aux += *ch;
                    }
                    break;

                case '%':
                    // cout << "ENTRE" << '\n';
                    fnd = true;
                    aux.clear();
                    aux = *ch;

                    while (fnd)
                    {
                        // cout << *ch << '\n';
                        ch++;
                        if (*ch == '%')
                        {
                            cout << aux << '\n';
                            aux += *ch;
                            ch++;
                            result.push_back("<TOKEN: COMENTARIO, LEXEMA: " + aux + "  >");
                            to_parse.push_back("%%");
                            fnd = false;
                            break;
                        }
                        else if (*ch == lectureF.eof() || isF(ch))
                        {
                            result.push_back("<ERROR: " + to_string(line) + ">");
                            fnd = false;
                        }
                        aux += *ch;
                    }
                    break;
                }

                if (result.size() > cont)
                {
                    continue;
                }

                else
                {
                    // cout << "ELSEF:" << *ch << "-" << isDigit(ch) << '\n';
                    aux.clear();

                    if (isDigit(ch))
                    {

                        string auxS = "";
                        bool isD = true;
                        bool isR = false;
                        bool erroR = false;

                        while (isD)
                        {
                            // cout << "ENTRE " << *ch << '\n';
                            auxS += *ch;
                            if (isDigit(ch))
                            {
                                isD = true;
                            }
                            else if (!isDigit(ch) && *ch == '.')
                            {
                                isD = false;
                                isR = true;
                            }
                            else if (*ch == ' ' || *ch == lectureF.eof() || isF(ch))
                            {
                                auxS = auxS.substr(0, auxS.length() - 1);
                                isD = false;
                            }
                            else
                            {
                                ch++;
                                // cout << "EERROR" << *(ch + 1) << '\n';
                                while (*ch != ' ' || isF(ch))
                                {
                                    auxS += *ch;
                                    ch++;
                                }
                                isD = false;
                                result.push_back("<ERROR: " + to_string(line) + ">");
                                erroR = true;
                            }
                            ch++;
                        }
                        if (!erroR && !isR)
                        {
                            result.push_back("<TOKEN: NUM, LEXEMA: " + auxS + "  >");
                            to_parse.push_back("num");
                        }
                        while (isR)
                        {
                            auxS += *ch;
                            if (isDigit(ch))
                            {
                                isR = true;
                            }
                            else if (*ch == ' ' || *(ch + 1) == lectureF.eof() || isF(ch))
                            {
                                auxS = auxS.substr(0, auxS.length() - 1);
                                isR = false;
                            }
                            else
                            {
                                result.push_back("<ERROR: " + to_string(line) + ">");
                                break;
                            }
                            ch++;
                        }
                        if (!erroR && isR)
                        {
                            result.push_back("<TOKEN: NUM_REAL, LEXEMA: " + auxS + "  >");
                            to_parse.push_back("num");
                        }
                        auxS.clear();
                        cont = result.size();
                        continue;
                    }
                    else if (isWord(ch))
                    {

                        bool fnd = true;
                        // cout << *ch << '\n';
                        aux = *ch;
                        while (fnd)
                        {
                            // cout << "IS WORDA-"<< *ch <<"-"<< '\n';

                            if (isWord(ch))
                            {
                                //   cout << "IS WORDD-"<< *ch <<"-"<< '\n';
                                // cout << "ENTRE AL POSRESERVED" <<'\n';
                                if (compare(reserved, 19, aux))
                                {
                                    // cout << *ch << '\n';
                                    pt = true;
                                    // cout << aux << pt << '\n';
                                    result.push_back("<TOKEN: RESERVED, LEXEMA: " + aux + "  >");
                                    to_parse.push_back(aux);
                                    fnd = false;
                                }
                            }
                            else if (*ch == ' ' || *ch == lectureF.eof() || isF(ch))
                            {
                                // cout << "AQUI " << aux  << "-"<< '\n';
                                std::size_t found = aux.find("main:");
                                result.push_back("<TOKEN: ID, LEXEMA: " + aux + "  >");
                                fnd = false;

                                if (found != std::string::npos || aux == "return" || aux == "ENDFUNCTION")
                                {
                                    // cout << aux << '\n';
                                    to_parse.push_back("main:");
                                }
                                else
                                {
                                    to_parse.push_back("id");
                                }
                            }
                            // cout << *ch << '\n';

                            ch++;
                            aux += *ch;
                        }
                        // cout << *ch << '\n';
                        continue;
                    }
                }

                break;
            }
        }
        if (*ch == ';')
        {
            result.push_back("<TOKEN: EOF, LEXEMA: ; >");
            to_parse.push_back(";");
        }
        else
        {
            result.push_back("<ERROR: " + to_string(line) + ">");
        }

        lectureF.close();
    }
}


void to_file(vector<string> to_parser)
{
    ofstream file;
    ofstream file2;
    ofstream file3;

    //file.open("D:/8vo semestre/Compiladores/FILES/build/Debug/ll1.txt");
    file.open("D:/8vo semestre/Compiladores/FILES/base/FINAL-PARSER/LL1-parser/ll1.txt");
    file2.open("D:/8vo semestre/Compiladores/FILES/ll1.txt");
    file3.open("D:/8vo semestre/Compiladores/FILES/build/Debug/ll1.txt");
    
    for (auto i : to_parser)
    {
        string data = i;
        std::for_each(data.begin(), data.end(), [](char &c)
                      { c = ::tolower(c); });
        /*if(i == "n")
            file << '\n';*/
        if (data == ";")
        {
            file << data;
            file2 << data;
            file3 << data;
        }
        else
        {
            file << data << ",";
            file2 << data << ",";
            file3 << data << ",";
        }
    }
    file.close();
    file2.close();
    file3.close();
}







int main()
{

    string source = "gramatica2.txt";
    // cout << "FUNCA "<< '\n';
    anLexico prg1;
    vector<string> rel;
    vector<string> to_parser;
    prg1.anTokens(source, rel, to_parser);
    cout << "ANALIZADOR COMPLETADO" << '\n';
    /*
    for (int i = 0; i < rel.size(); i++)
    {
        cout << rel[i] << '\n';
    }*/
    to_file(to_parser);

    string final;
    for (auto i : to_parser)
    {
        final += i;
    }

    cout << "final2" << '\n';
    
    string command = "pytest base/FINAL-PARSER/LL1-parser/tests/test_parsing.py ";
    // string command = "pytest D:/8vo semestre/Compiladores/FILES/base/LL1-parser/mainPar.py" + final;
    //string command = "pytest ../../base/FINAL-PARSER/LL1-parser/tests/test_parsing.py ";
    
    system(command.c_str());
    cout << "Datos enviados a python " << '\n';

    for (auto i : to_parser)
    {
        cout << i << '\t';
    }

    return 0;
}