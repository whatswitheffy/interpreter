#ifndef SYNTAX_H
#define SYNTAX_H
#include "lexem.h"
void joinGotoAndLabel(Variable * lexemvar, vector <Lexem *> stack);
void ifOperator(vector <Lexem *> &stackOfOperators, vector <Lexem *> postfix_, vector <Lexem *> infix, int i);
vector <Lexem *> buildPostfix(vector <Lexem *> &infix);
#endif