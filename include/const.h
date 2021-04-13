#ifndef CONST_H
#define CONST_H
#include <string>
using namespace std;
enum OPERATOR {
    IF, THEN,
    ELSE, ENDIF,
    WHILE, ENDWHILE,
    GOTO, COLON,
    LBRACKET, RBRACKET,
    ASSIGN,
    OR,
    AND,
    BITOR,
    XOR,
    BITAND,
    EQ, NEQ,
    LEQ, LT,
    GEQ, GT,
    SHL, SHR,
    PLUS, MINUS,
    MULTIPLY, DIV, MOD
};

string OPERTEXT [] = {
    "if", "then",
    "else", "endif",
    "while", "endwhile",
    "goto", ":",
    "(", ")",
    "=",
    "or",
    "and",
    "|",
    "^",
    "&",
    "==", "!=",
    "<=", "<",
    ">=", ">",
    "<<", ">>",
    "+", "-",
    "*", "/", "%"
};
 int PRIORITY [] = {
    -3, -3,
    -3, -3,
    -3, -3,   
    -2, -2,
    -1, -1,
    0,
    1,
    2,
    3,
    4,
    5,
    6, 6,
    7, 7,
    7, 7,
    8, 8,
    9, 9,
    10, 10, 10
};
#endif