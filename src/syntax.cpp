#include "syntax.h"

void ifOperator(vector <Lexem *> &stackOfOperators, vector <Lexem *> &postfix_, vector <Lexem *> &infix, int i) {
    int priority = 0;
    if(infix[i]->getType() == RBRACKET) {
        bool found = false;
        for (int j = 0; j < stackOfOperators.size(); ++j)
            if (stackOfOperators[j]->getType() == LBRACKET)
                found = true;
        if (found)
            while (!stackOfOperators.empty() && (stackOfOperators.back()->getType() != LBRACKET)) {
                postfix_.push_back(stackOfOperators.back());
                stackOfOperators.pop_back();
            }
        else
            postfix_.push_back(stackOfOperators.back());
        stackOfOperators.pop_back();
    } else if(infix[i]->getType() == RSQRBRACKET) {
        while(!stackOfOperators.empty() && (stackOfOperators.back()->getType() != LSQRBRACKET)) {
            postfix_.push_back(stackOfOperators.back());
            stackOfOperators.pop_back();  
        }
        postfix_.push_back(stackOfOperators.back());
        stackOfOperators.pop_back();  
    } else {
        priority = infix[i]->getPriority();
        while((priority != -1 ) && !stackOfOperators.empty() && (stackOfOperators.back()->getPriority() >= priority)) {
            postfix_.push_back(stackOfOperators.back());
            stackOfOperators.pop_back();
        }
        stackOfOperators.push_back(infix[i]);
    } 
}

vector <Lexem *> buildPostfix(vector <Lexem *> &infix) {
    vector <Lexem *> postfix_;
    vector <Lexem *> stackOfOperators;
    for(int i = 0; i < infix.size(); ++i) {       
        if(infix[i] == nullptr) {
            continue;
        }
        if(infix[i]->isOperator()) {
            cout << "IsOperator: " << i << endl;
            ifOperator(stackOfOperators, postfix_, infix, i);
            Operators *lexemoper = (Operators*)infix[i];
            if(lexemoper->getType() == ENDIF) {
                continue ;
            }
        } else if(infix[i]->isVariable()) {
            cout << "IsVariable: " << i << endl;
            if(funTable.find(infix[i]->getName()) != funTable.end()) {
                cout << "IsFuntable: " << i << endl;
                // x = f ( 5 , g ( 2 ) )   
                // = f 5 g ( 2
                // x 2 ( g 5 ( f
                string nameOfFun = infix[i]->getName();
                while(infix[i]->getType() != RBRACKET) {
                    stackOfOperators.push_back(infix[i]);
                    i++;
                }
                while(stackOfOperators.back()->getName() != nameOfFun) {
                    postfix_.push_back(stackOfOperators.back());
                    stackOfOperators.pop_back();
                }
                postfix_.push_back(stackOfOperators.back());
                stackOfOperators.pop_back();
                continue;
            } else {
                Variable *lexemVar = (Variable *)infix[i];
                if(lexemVar->inLabelTable()) {
                    joinGotoAndLabel(lexemVar, stackOfOperators);
                } else {
                    postfix_.push_back(infix[i]);
                }
            }
        } else {
            cout << "IsSomething: " << i << endl;
            postfix_.push_back(infix[i]);
        }
    }
    while(!stackOfOperators.empty()) {
        postfix_.push_back(stackOfOperators.back());
        stackOfOperators.pop_back();
    }
  
    cout << "postfix: ";
    for(int i = 0; i < postfix_.size(); ++i) {
        postfix_[i]->print();
    }
    cout << endl;
    return postfix_;
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
