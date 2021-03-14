#include <string>
#include <vector>
#include <iostream>
#include "lexem.h"
using namespace std;
#include <stack>

vector <Lexem *> parseLexem(const string &codeline);
int evaluatePostfix(vector <Lexem *> postfix);
vector <Lexem *> buildPostfix(vector <Lexem *> infix); 
bool isOperator(Lexem *object);

Lexem *get_oper(string codeline, int &pos) {
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
Lexem *get_num(string codeline, int &pos) {
    int number = 0;
    int old_pos = pos;
    while(codeline[pos] >= '0' && codeline[pos] <= '9') {
        number = number * 10 + codeline[pos] - '0';
        pos++;
    }
    if (pos == old_pos) {
        return nullptr;
    }
    return new Number(number);
}
Lexem *scan_var(string codeline, int &pos) {
    string name;
    int old_pos = pos;
    Variable *ptr;
    while((codeline[pos] >= 'a' && codeline[pos] <= 'z') || (codeline[pos] >= 'A' && codeline[pos] <= 'Z') || 
          (codeline[pos] >= '0' && codeline[pos] <= '9') || codeline[pos] == '_') 
    {
        name.push_back(codeline[pos]);
        pos++;
    }
    if(pos == old_pos) {
        return nullptr;
    }
    auto search = varTable.find(name);
    if (search == varTable.end()) {
        ptr = new Variable(name);
        varTable.insert(make_pair(name, ptr));
        return ptr;
    } else {
        return search->second;
    }
}

vector <Lexem *> parseLexem(const string &codeline) {
    Lexem *lexem;
    vector <Lexem *> parsed;
    for(int pos = 0; pos < codeline.size();) {
        lexem = get_oper(codeline, pos);
        if(lexem != nullptr) {    
            parsed.push_back(lexem);
            continue;
        }
        lexem = get_num(codeline, pos);
        if(lexem != nullptr) {
            parsed.push_back(lexem);
            continue;
        }
        lexem = scan_var(codeline, pos);
        if(lexem != nullptr) {
            parsed.push_back(lexem);
            continue;
        }
    }
    return parsed;
}

int evaluatePostfix(vector <Lexem *> postfix) {
    int result = 0;
    Lexem *left, *right;
    stack <Lexem *> evaluationStack;
    for(int i = 0; i < postfix.size(); ++i) {
        if(!postfix[i]->isOperator()) {
            evaluationStack.push(postfix[i]);
        } else {
            right = evaluationStack.top();
            cout << "right ";
            right->print();
            cout << endl;
            evaluationStack.pop();
            left = evaluationStack.top();
            cout << "left ";
            left->print();
             cout << endl;
            evaluationStack.pop();
            result = postfix[i]->getValue(left, right);
            cout << "result = " << result << endl;
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
    cin >> codeline;
    while(codeline != "exit") {
        infix = parseLexem(codeline);
        postfix = buildPostfix(infix);
       /* for(int i = 0; i < postfix.size(); ++i) {
            postfix[i]->print();
            cout << "\n";
        }*/
        value = evaluatePostfix(postfix);
        cout << "value :" << value << endl ;
        cin >> codeline;
    }

    return 0;
}

