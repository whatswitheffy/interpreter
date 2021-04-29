#include "syntax.h"

void processBrackets(vector <Lexem *> &stackOfOperators, vector <Lexem *> &postfix) {
    while (!stackOfOperators.empty() && (stackOfOperators.back()->getType() != LBRACKET) && (stackOfOperators.back()->getType() != LSQRBRACKET)) {
        postfix.push_back(stackOfOperators.back());
        stackOfOperators.pop_back();
    }
}
//a = f(x + 2, f(y, 2 * (3 + 1)))
//a x 2 + y 2 3 1 + * f f =

void processLexem(vector <Lexem *> &stackOfOperators, vector <Lexem *> &postfix, Lexem *lexem, int i) {
    int priority = 0;
    if(lexem->getType() == RBRACKET) {
        processBrackets(stackOfOperators, postfix);
        stackOfOperators.pop_back();
    } else if (lexem->getType() == COMMA) {
        processBrackets(stackOfOperators, postfix);
    } 
    else if(lexem->getType() == RSQRBRACKET) {
        processBrackets(stackOfOperators, postfix);
        postfix.push_back(stackOfOperators.back());
        stackOfOperators.pop_back();  
    } else {
        priority = lexem->getPriority();
        while((priority != -1 ) && !stackOfOperators.empty() && (stackOfOperators.back()->getPriority() >= priority)) {
            postfix.push_back(stackOfOperators.back());
            stackOfOperators.pop_back();
        }
        stackOfOperators.push_back(lexem);
    } 
}


vector <Lexem *> buildPostfix(vector <Lexem *> &infix) {
    vector <Lexem *> postfix;
    vector <Lexem *> stackOfOperators;
    cout << "infix: " << endl;
    for(int i = 0; i < infix.size(); ++i) {
        infix[i]->print();
        cout << " ";
    }
    cout << endl;
    for(int i = 0; i < infix.size(); ++i) {      
        if(infix[i] == nullptr) {
            continue;
        }
        if(infix[i]->isOperator()) {
            cout << "IsOperator: " << i << endl;
            processLexem(stackOfOperators, postfix, infix[i], i);
            Operators *lexemoper = (Operators*)infix[i];
            if(lexemoper->getType() == ENDIF) {
                continue ;
            }
        } else if(infix[i]->isVariable()) {
            cout << "IsVariable: " << i << endl;
            if(funTable.find(infix[i]->getName()) != funTable.end()) {
                processLexem(stackOfOperators, postfix, infix[i], i);
            } else {
                Variable *lexemVar = (Variable *)infix[i];
                if(lexemVar->inLabelTable()) {
                    joinGotoAndLabel(lexemVar, stackOfOperators);
                } else {
                    postfix.push_back(infix[i]);
                }
            }
        } else {
            cout << "IsNumber: " << i << endl;
            postfix.push_back(infix[i]);
        }
    }
    while(!stackOfOperators.empty()) {
        postfix.push_back(stackOfOperators.back());
        stackOfOperators.pop_back();
    }
  
    cout << "postfix: ";
    for(int i = 0; i < postfix.size(); ++i) {
        postfix[i]->print();
    }
    cout << endl;
    return postfix;
}

void joinGotoAndLabel(Variable * lexemvar, vector <Lexem *> stack) {
    int row = 0;
    if(stack.empty()) {
        return;
    }  
    if(stack.back()->getType() == GOTO) {
        Goto * gotoLexem = (Goto *)stack.back();
        row = lableTable[lexemvar->getName()];
        gotoLexem->setRow(row);
    }
}
