#include "lexem.h"
#include "const.h"

map <string, Variable *> varTable;
map <string, int> lableTable;

Lexem::Lexem() {  
}
Number::Number(int value) {
    this->value = value;
}
int Number::getValue() const {
    return value;
}
void Number::print() {
    cout << value << " ";
}


Operators::Operators() {}

OPERATOR Operators::getType() {
    return opertype;
}
Operators::Operators(int idx) {
    opertype = (OPERATOR)idx;
}
void Operators::print() {
    cout << OPERTEXT[(int)getType()];
}
int Operators::getPriority() {
    int priority = 0;
    for(int i = 0; i < OPER_LEN; ++i) {
        if(getType() == i) {
            priority = PRIORITY[i]; 
        }
    }
    return priority;
}
int Operators::getValue(Lexem *left, Lexem *right) {  
    if(opertype == PLUS) {
        return left->getValue() + right->getValue();
    }
    if(opertype == MINUS) {
        return left->getValue() - right->getValue();
    }
    if(opertype == MULTIPLY) {
        return left->getValue() * right->getValue();
    }
    if(opertype == DIV) {
        return left->getValue() / right->getValue();
    }
    if(opertype == MOD) {
        return left->getValue() % right->getValue();
    }
    if(opertype == AND) {
        return left->getValue() & right->getValue();
    }
    if(opertype == OR) {
        return left->getValue() || right->getValue();
    }
    if(opertype == BITOR) {
        return left->getValue() | right->getValue();
    }
    if(opertype == XOR) {
        return left->getValue() ^ right->getValue();
    }
    if(opertype == BITAND) {
        return left->getValue() & right->getValue();
    }
    if(opertype == EQ) {
        return left->getValue() == right->getValue();
    }
    if(opertype == NEQ) {
        return left->getValue() != right->getValue();
    }
    if(opertype == LEQ) {
        return left->getValue() <= right->getValue();
    }
    if(opertype == LT) {
        return left->getValue() < right->getValue();
    }
    if(opertype == GEQ) {
       return left->getValue() >= right->getValue();
    }
    if(opertype == GT) {
        return left->getValue() > right->getValue();
    }
    if(opertype == ASSIGN) {
        int val = right->getValue();
        left->setValue(val);
        return val;
    }
}


string Variable::getName() {
    return this->name;
}
Variable::Variable() {
    name = nullptr;
}
Variable::Variable(string name) {
    value = 0;
    this->name = name;
}
void Variable::setValue(int value) {
    this->value = value;
}
int Variable::getValue() const {
    return value;
}
void Variable::print() {
    cout << name;
}
bool Variable::inLabelTable() {  
    if(lableTable.find(this->getName()) == lableTable.end()) {
        return false;
    } else {
        return true;
    }
}
