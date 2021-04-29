#include "lexical.h"
#include "const.h"
Lexem *getOperator(string codeline, int &pos) {
    string subcodeline;
    for(int op = 0; op < OPER_LEN; ++op) {
        subcodeline = codeline.substr(pos, OPERTEXT[op].size());
        if(OPERTEXT[op] == subcodeline) {
            pos += OPERTEXT[op].size();
            if(OPERATOR(op) == GOTO || OPERATOR(op) == IF || OPERATOR(op) == ELSE ||
                OPERATOR(op) == WHILE || OPERATOR(op) == ENDWHILE || OPERATOR(op) == FUNCTION) {
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
        varTable.insert(make_pair(name, 0));
        return ptr;
    } else {
        return new Variable(search->first);
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
        if(infix[i - 1] -> getType() == FUNCTION) {
            ((Goto*)infix[i - 1])->setRow(row);
            int commaCounter = 0;
            for(int i = 0 ; i < infix.size(); ++i) {
                if(infix[i]->getType() == COMMA) {
                        commaCounter++;                   
                }
            }
            funTable[infix[i]->getName()].argNum = commaCounter + 1;
            cout << "argNum" << funTable[infix[i]->getName()].argNum << endl;
            funTable[infix[i]->getName()].row = row;
            for(int i = 0 ; i < infix.size(); ++i) {
                infix[i]->print(); cout << " ";
                if(funTable.find(infix[i]->getName()) != funTable.end()) {
                    infix.erase(infix.begin() + i); 
                    i--;
                }
                if((infix[i]->getType() == LBRACKET) || (infix[i]->getType() == RBRACKET) || (infix[i]->getType() == COMMA)) {
                    infix.erase(infix.begin() + i);
                    i--;
                }               
            }
            return;
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
