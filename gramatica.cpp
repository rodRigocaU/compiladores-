#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
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
    if(*abs == '\n' || *abs == '\t')
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
    string reserved[16] = {"FUNCTION", "IF", "ELSEIF", "ELSE", "RETURN", "AddNoise", "ReduceNoise", "ColorRi",
                           "compare", "Read", "Save", "Rotate", "Combine", "Solar", "main:", "PRINT"};
    string id = "|";
    string str[2] = {"'", "%"};
    string num[10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

public:
    void readFile(string);
    bool isDigit(const char *);
    bool isWord(const char *);
    void anTokens(string, vector<string> &);
    char peekChar(const char *s, int ss);
};

//CPP

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

void anLexico::anTokens(string source, vector<string> &result)
{
    string line;
    int cont;
    ifstream lectureF(source);
    cout << "FLAG " << '\n';
    bool salto = false;
    std::string str((std::istreambuf_iterator<char>(lectureF)), std::istreambuf_iterator<char>());
    cout << "FLAG " << '\n';
    if (lectureF.is_open())
    {
        const char *ch;
        ch = str.c_str();
        int line;
        while (*ch != lectureF.eof())
        {
            //cout << *ch << " "; 
            cont = result.size();
            //lectureF >> ch;
            if (*ch == ' ' || *ch == '\t')
            {
                ch++;
                continue;
            }
            else if (*ch == '\n')
            {
                ch++;
                line++;
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
                        salto = true;
                    }
                    else if (peekChar(ch, 1) == ' ' || isDigit(ch) || isWord(ch))
                    {
                        ch += 1;
                        result.push_back("<TOKEN: OP, LEXEMA: + >");
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
                        result.push_back("<TOKEN: OP, LEXEMA: - >");
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
                    }
                    else if (peekChar(ch, 1) == ' ' || isDigit(ch) || isWord(ch))
                    {
                        ch++;
                        result.push_back("<TOKEN: ?, LEXEMA: < ");
                    }
                    else
                    {
                        ch++;
                        result.push_back("ERROR: " + line);
                    }
                    break;
                case '=':
                    if (peekChar(ch, 1) == '=')
                    {
                        ch += 2;
                        result.push_back("<TOKEN: OP, LEXEMA: ++ >");
                    }
                    else if (peekChar(ch, 1) == ' ' || isDigit(ch) || isWord(ch))
                    {
                        ch += 1;
                        result.push_back("<TOKEN: OP, LEXEMA: = >");
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
                            //cout << "ENTRE FINAL "<< '\n';
                            //cout << aux << '\n';
                            aux += *ch;
                            ch++;
                            result.push_back("<TOKEN: STRING, LEXEMA: " + aux + "  >");
                            fnd = false;
                            break;
                        }
                        else if (*ch == ' ' || *ch == lectureF.eof() || isF(ch))
                        {
                            result.push_back("<ERROR: " + to_string(line) + ">");
                            fnd = false;
                        }
                        aux += *ch;
                    }
                    break;

                case '%':
                    //cout << "ENTRE" << '\n';
                    fnd = true;
                    aux.clear();
                    aux = *ch;
                    
                    while (fnd)
                    {
                        //cout << *ch << '\n';
                        ch++;
                        if (*ch == '%')
                        {
                            cout << aux << '\n';
                            aux += *ch;
                            ch++;
                            result.push_back("<TOKEN: COMENTARIO, LEXEMA: " + aux + "  >");
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
                        //cout << "ELSEF:" << *ch << "-" << isDigit(ch) << '\n';
                        aux.clear();
                        if (isDigit(ch))
                        {
                            
                            string auxS = "";
                            bool isD = true;
                            bool isR = false;
                            bool erroR = false;

                            while (isD)
                            {
                                //cout << "ENTRE " << *ch << '\n';
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
                                    auxS= auxS.substr(0, auxS.length() - 1);
                                    isD = false;
                                }
                                else
                                {
                                    ch++;
                                   // cout << "EERROR" << *(ch + 1) << '\n';
                                    while(*ch != ' ' || isF(ch))
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
                                    auxS= auxS.substr(0, auxS.length() - 1);
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
                            }
                            auxS.clear();
                            cont = result.size();
                            continue;
                            
                        }
                        else if (isWord(ch))
                        {
                            
                            bool fnd = true;
                            //cout << *ch << '\n';
                            aux = *ch;
                            while (fnd)
                            {
                                //cout << "IS WORDA-"<< *ch <<"-"<< '\n';
                                
                                if (isWord(ch))
                                {
                                  //   cout << "IS WORDD-"<< *ch <<"-"<< '\n';
                                   // cout << "ENTRE AL POSRESERVED" <<'\n';
                                    if (compare(reserved, 16, aux))
                                    {
                                        //cout << *ch << '\n';
                                        result.push_back("<TOKEN: RESERVED, LEXEMA: " + aux + "  >");
                                        fnd = false;
                                    }
                                }
                                else if (*ch == ' ' || *ch == lectureF.eof() || isF(ch))
                                {
                                    //cout << "AQUI "<< '\n';
                                    result.push_back("<TOKEN: ID, LEXEMA: " + aux + "  >");
                                    fnd = false;
                                }
                                ch++;
                                aux += *ch;
                            }
                            continue;
                        }
                    }

                break;
            }
        }
    }
}

int main()
{

    string source = "gramatica2.txt";
    anLexico prg1;
    vector<string> rel;
    prg1.anTokens(source, rel);
    cout << "ANALIZADOR COMPLETADO" << '\n';
    for (int i = 0; i < rel.size(); i++)
    {
        cout << rel[i] << '\n';
    }

    return 0;
}