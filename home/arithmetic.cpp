#include <string>
#include <vector>
#include <iostream>
#include "lexem.h"
#include <stack>
using namespace std;

vector <Lexem *> parseLexem(const string &codeline);
vector <Lexem *> buildPostfix(vector <Lexem *> infix); 
int evaluatePostfix(vector <Lexem *> postfix);
bool isOperator(Lexem *object);

Lexem *getOperator(string codeline, int &pos) {
    string subcodeline;
    for(int op = 20; op >= 0; --op) {
        subcodeline = codeline.substr(pos, OPERTEXT[op].size());
        if(OPERTEXT[op] == subcodeline) {
            pos += OPERTEXT[op].size();
            return new Operators(op);
        }
    }
    return nullptr;
}

Lexem *getNumber(string codeline, int &pos) {
    int number = 0;
    //int old_pos = pos;
    bool positionFlag = true;
    while(codeline[pos] >= '0' && codeline[pos] <= '9') {
        number = number * 10 + codeline[pos] - '0';
        pos++;
        positionFlag = false;
    }
    /*if(pos == old_pos) {
        return nullptr;
    }*/
     if(positionFlag) {
        return nullptr;
    }
    return new Number(number);
}

Lexem *scanVariable(string codeline, int &pos) {
    string name;
    //int old_pos = pos;
    bool positionFlag = true;
    Variable *ptr;
    while((codeline[pos] >= 'a' && codeline[pos] <= 'z') || (codeline[pos] >= 'A' && codeline[pos] <= 'Z') || 
          (codeline[pos] >= '0' && codeline[pos] <= '9') || codeline[pos] == '_') 
    {
        name.push_back(codeline[pos]);
        pos++;
        positionFlag = false;
    }
   /* if(pos == old_pos) {
        return nullptr;
    }*/
    if(positionFlag) {
        return nullptr;
    }
    auto search = varTable.find(name);
    if(search == varTable.end()) {
        ptr = new Variable(name);
        varTable.insert(make_pair(name, ptr));
        return ptr;
    } else {
        return search->second;
    }
}

bool isEmptyChar(char empty) {
    if(empty == ' ' || empty == '\t') {
        return true;
    } else {
        return false;
    }
}

vector <Lexem *> parseLexem(const string &codeline) {
    Lexem *lexem;
    vector <Lexem *> parsed;
    for(int pos = 0; pos < codeline.size();) {
        if(isEmptyChar(codeline[pos])) {
            pos++;
        }
        lexem = getOperator(codeline, pos);
        if(lexem != nullptr) {    
            parsed.push_back(lexem);
            continue;
        }
        lexem = getNumber(codeline, pos);
        if(lexem != nullptr) {
            parsed.push_back(lexem);
            continue;
        }
        lexem = scanVariable(codeline, pos);
        if(lexem != nullptr) {
            parsed.push_back(lexem);
            continue;
        }
    }
    return parsed;
}

//delete tmp element in evalpost
int evaluatePostfix(vector <Lexem *> postfix) {
    int result = 0;
    Lexem *left, *right;
    stack <Lexem *> evaluationStack;
    for(int i = 0; i < postfix.size(); ++i) {
        if(!postfix[i]->isOperator()) {
            evaluationStack.push(postfix[i]);
        } else {
            right = evaluationStack.top();
            evaluationStack.pop();
            left = evaluationStack.top();
            evaluationStack.pop();
            result = postfix[i]->getValue(left, right);
            evaluationStack.push(new Number(result));
        }
    }
    return evaluationStack.top()->getValue();
}

vector <Lexem *> buildPostfix(vector <Lexem *> infix) {
    vector <Lexem *> postfix_;
    vector <Lexem *> stackOfOperators;
    int priority = 0;
    for(int i = 0; i < infix.size(); ++i) {
        if(infix[i]->isOperator()) {
            if(infix[i]->getType() == RBRACKET) {
                while(!stackOfOperators.empty() && (stackOfOperators.back()->getType() != LBRACKET)) {    
                    postfix_.push_back(stackOfOperators.back()); 
                    stackOfOperators.pop_back();                
                }   
                stackOfOperators.pop_back();
            } else {
                priority = infix[i]->getPriority();
                while((priority != -1) && !stackOfOperators.empty() && (stackOfOperators.back()->getPriority() >= priority)) {
                    postfix_.push_back(stackOfOperators.back());
                    stackOfOperators.pop_back();
                }
                stackOfOperators.push_back(infix[i]);
            }
        } else {
            postfix_.push_back(infix[i]);
        }
    }
    while(!stackOfOperators.empty()) {
        postfix_.push_back(stackOfOperators.back());
        stackOfOperators.pop_back();
    }
    return postfix_;
}

int main () {
    string codeline;
    vector <Lexem *> infix;
    vector <Lexem *> postfix;
    int value = 0;
    getline(cin, codeline);
    while(codeline != "exit") {
        infix = parseLexem(codeline);
        postfix = buildPostfix(infix);
        value = evaluatePostfix(postfix);
        cout << "value: " << value << endl ;
        getline(cin, codeline);
    }
    return 0;
}

