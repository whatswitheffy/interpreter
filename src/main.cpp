#include <string>
#include <vector>
#include <iostream>
#include "lexem.h"
#include "lexical.h"
#include "syntax.h"
#include "semantic.h"
#include <stack>
using namespace std;

void initLabels(vector <Lexem *> &infix, int row) {
    for(int i = 1; i < infix.size(); ++i) {
        if(infix[i - 1]->isVariable() && infix[i]->isOperator()) {
            Variable *lexemvar = (Variable *)infix[i - 1];
            Operators *lexemop = (Operators *)infix[i];
            if(lexemop->getType() == COLON) {
                if(lableTable.find(lexemvar->getName()) == lableTable.end()) {
                    lableTable[lexemvar->getName()] = row;    
                       
                } else { 
                    try {
                        if(lableTable.find(lexemvar->getName()) != lableTable.end()) {
                            throw 0;
                        }
                    }
                    catch(int error) {
                        cout << "Error: " <<  error << endl;
                    }
                }
                i++;
            }
        }
    }
}

void initJumps(vector <vector <Lexem *>> &infixLines) {
    stack <Goto *> stackIfElse;
    stack <Goto *> stackWhile;
    for(int row = 0; row <  (int)infixLines.size(); ++row) {
        for(int i = 0; i < (int) infixLines[row].size(); ++i) {
            if(infixLines[row][i]->isOperator()) {
                Operators* lexemoper = (Operators *)infixLines[row][i];
                if(lexemoper->getType() == IF) {
                    stackIfElse.push((Goto *)lexemoper);
                }
                if(lexemoper->getType() == ELSE) {
                    stackIfElse.top()->setRow(row + 1);
                    stackIfElse.pop();
                    stackIfElse.push((Goto *)lexemoper);
                }
                if(lexemoper->getType() == ENDIF) {
                    stackIfElse.top()->setRow(row + 1);
                    stackIfElse.pop();
                }
                if(lexemoper->getType() == WHILE) {
                    Goto * lexemgoto = (Goto *)lexemoper ;
                    lexemgoto->setRow(row);
                    stackWhile.push(lexemgoto);
                }
                if(lexemoper->getType() == ENDWHILE) {
                    Goto * lexemgoto = (Goto *)lexemoper ;
                    lexemgoto->setRow(stackWhile.top()->getRow());
                    stackWhile.top()->setRow(row + 1);
                    stackWhile.pop();
                }
            }
        }
    }
}

int main () {
    string codeline;
    vector <vector <Lexem *>> infixLines, postfixLines;
    getline(cin, codeline);
    while(codeline != "exit") {
        infixLines.push_back(parseLexem(codeline));
        getline(cin, codeline);
    }
    for(int row = 0; row < (int)infixLines.size(); ++row) {
        initLabels(infixLines[row], row);
    }
    initJumps(infixLines);
    for(int i = 0; i < (int)infixLines.size(); ++i) {
        postfixLines.push_back(buildPostfix(infixLines[i]));
    }
    int row = 0;
    while(0 <= row && row < (int)postfixLines.size()) {
        row = evaluatePostfix(postfixLines[row], row);
    }
    return 0;
}


