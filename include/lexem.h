#ifndef LEXEM_H
#define LEXEM_H
#include "const.h"
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <stack>
using namespace std;
class Lexem;
class Number;
class Operators;
class Variable;
class Goto;
class Array;
struct function {
    int argNum;
    int row;
};
extern map <string, int> varTable;
extern map <string, int> lableTable;
extern map <string, int* > ArrayTable;
extern map <string, function> funTable;
extern stack <int> programStack;
class Lexem {
public:
    Lexem();
    virtual OPERATOR getType() {return RETURN;};
    virtual bool isOperator() {return false;};
    virtual bool isVariable() {return false;};
    virtual string getName() {return "";};
    virtual int getValue() const {return 0;};
    virtual int getValue(Lexem* left, Lexem* right) {return 0;};
    virtual int getPriority() {return 0;};
    virtual void setValue(int value) {};
    virtual void print() {}; 
};

class Number : public Lexem {
    int value;
public:
    Number();
    Number(int value);
    bool isOperator() {return false;};
    int getValue() const;
    void print();
};

class Operators : public Lexem {
    OPERATOR opertype;
public:
    Operators();
    Operators(int idx);
    Operators(vector <Lexem *> vec);
    OPERATOR getType();
    bool isOperator() {return true;};
    bool isVariable() {return false;};
    int getPriority();
    int getValue(Lexem *left, Lexem *right);
    void print();
};

class Variable : public Lexem {
    string name;
public:
    Variable();
    Variable(string name);
    bool isOperator() {return false;};
    bool isVariable() {return true;};
    int getPriority();
    void print();
    int getValue() const;
    void setValue(int value);
    string getName();
    bool inLabelTable();
};


class Goto : public Operators {
    int row;
public:
    bool isVariable() {return false;};
    bool isOperator() {return true;};
    enum { UNDEFINED = - INT32_MAX };
    Goto(OPERATOR optype): Operators(optype) { row = UNDEFINED; }
    Goto(int op = 0): Operators(op) {};
    void setRow(int row) { Goto :: row = row; }
    int getRow(){ return row; }
    void print() { cout << " [ < row " << row << ">" << OPERTEXT[(int)getType()] << " ] "; }
};

class Array: public Lexem {
    string arrayName;
    int elementNumber;
public:
    Array(Lexem *left, Lexem *right);
    void setValue(int number);
    int getValue() const;
};

#endif
