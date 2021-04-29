#ifndef SYNTAX_H
#define SYNTAX_H
#include "lexem.h"
void processBrackets(vector <Lexem *> &stackOfOperators, vector <Lexem *> &postfix);
void joinGotoAndLabel(Variable * lexemvar, vector <Lexem *> stack);
void processLexem(vector <Lexem *> &stackOfOperators, vector <Lexem *> &postfix_, Lexem* lexem, int i);
vector <Lexem *> buildPostfix(vector <Lexem *> &infix);
#endif
