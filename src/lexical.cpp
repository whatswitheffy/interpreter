#include "lexical.h"
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

Lexem *getOperator(string codeline, int &pos) {
    string subcodeline;
    for(int op = 0; op < 29; ++op) {
        subcodeline = codeline.substr(pos, OPERTEXT[op].size());
        if(OPERTEXT[op] == subcodeline) {
            pos += OPERTEXT[op].size();
            if(OPERATOR(op) == GOTO || OPERATOR(op) == IF || OPERATOR(op) == ELSE ||
                OPERATOR(op) == WHILE || OPERATOR(op) == ENDWHILE) {
                return new Goto(op); 
            } else {
                return new Operators(op);
            }
        }
    }
    return nullptr;
}

Lexem *getNumber(string codeline, int &pos) {
    int number = 0;
    bool positionFlag = true;
    while(codeline[pos] >= '0' && codeline[pos] <= '9') {
        number = number * 10 + codeline[pos] - '0';
        pos++;
        positionFlag = false;
    }
     if(positionFlag) {
        return nullptr;
    }
    return new Number(number);
}

Lexem *scanVariable(string codeline, int &pos) {
    string name;
    Variable *ptr;
    bool positionFlag = true;
    while((codeline[pos] >= 'a' && codeline[pos] <= 'z') || (codeline[pos] >= 'A' && codeline[pos] <= 'Z') || 
          (codeline[pos] >= '0' && codeline[pos] <= '9') || codeline[pos] == '_') 
    {
        name.push_back(codeline[pos]);
        pos++;
        positionFlag = false;
    }
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
int main() {
    return 0;
}