#ifndef LEXICAL_H
#define LEXICAL_H
#include "lexem.h"
vector <Lexem *> parseLexem(const string &codeline);
Lexem *getNumber(string codeline, int &pos);
Lexem *scanVariable(string codeline, int &pos);
bool isEmptyChar(char empty);
Lexem *getOperator(string codeline, int &pos);
void initLabels(vector <Lexem *> &infix, int row);
void initJumps(vector <vector <Lexem *>> &infixLines);
#endif