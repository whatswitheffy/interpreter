#include "syntax.h"

void ifOperator(vector <Lexem *> &stackOfOperators, vector <Lexem *> &postfix_, vector <Lexem *> &infix, int i) {
    int priority = 0;
    if(infix[i]->getType() == RBRACKET) {
        while(!stackOfOperators.empty() && (stackOfOperators.back()->getType() != LBRACKET)) {    
            postfix_.push_back(stackOfOperators.back()); 
            stackOfOperators.pop_back();                
        }   
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
        while((priority != -1) && !stackOfOperators.empty() && (stackOfOperators.back()->getPriority() >= priority)) {
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
            ifOperator(stackOfOperators, postfix_, infix, i);
            Operators * lexemoper = (Operators *)infix[i];
            if(lexemoper->getType() == ENDIF) {
                continue ;
            } 
        } else if(infix[i]->isVariable()) {
            Variable *lexemVar = (Variable *)infix[i];
            if(lexemVar->inLabelTable()) {
                joinGotoAndLabel(lexemVar, stackOfOperators);
            } else {
                postfix_.push_back(infix[i]);
            }
        } else {
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
