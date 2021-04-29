#include "semantic.h"

int evaluatePostfix(vector <vector <Lexem *>> postfixLines, int row) {
    int result = 0;
    stack <Lexem*> evaluationStack;
    vector<Lexem *> postfix = postfixLines[row];
    Lexem *left, *right;
    for(int i = 0; i < postfix.size(); ++i) {
        if(postfix[i] == nullptr) {
            continue;
        } else if(postfix[i]->isOperator()) {
            Operators *lexemop = (Operators *)postfix[i];
            if(lexemop->getType() == GOTO ||
                lexemop->getType() == ELSE ||
                lexemop->getType() == ENDWHILE ) {
                Goto *lexemgoto = (Goto *)lexemop;
                return lexemgoto->getRow();
            } else if(lexemop->getType() == IF ||
                lexemop->getType() == WHILE ) {
                Goto *lexemgoto = (Goto *)lexemop ;
                int rvalue = evaluationStack.top()->getValue();
                evaluationStack.pop();
                if(!rvalue) {
                    return lexemgoto->getRow();
                }
            } else if(lexemop->getType() == SIZE) {
                right = evaluationStack.top();
                evaluationStack.pop();
                left = evaluationStack.top();
                evaluationStack.pop();
                ArrayTable[left->getName()] = new int[right->getValue()];
            } else if(lexemop->getType() == LSQRBRACKET) {
                right = evaluationStack.top();
                evaluationStack.pop();
                left = evaluationStack.top();
                evaluationStack.pop();
                evaluationStack.push(new Array(left, right));
            } else if(postfix[i]->getType() == RETURN) {
                cout << "IsReturn" << endl;
                if(!evaluationStack.empty()) {
                    programStack.push(evaluationStack.top()->getValue());
                    evaluationStack.pop();
                }
                return -1;
            } else if(postfix[i]->getType() == FUNCTION) {
                cout << "IsFunction" << endl;
                while(!programStack.empty()) {
                    evaluationStack.top()->setValue(programStack.top());
                    evaluationStack.pop();
                    programStack.pop();
                }
                continue;
            } else {               
                if(evaluationStack.size() >= 2) {
                    right = evaluationStack.top();
                    evaluationStack.pop();
                    left = evaluationStack.top();
                    evaluationStack.pop();
                    result = postfix[i]->getValue(left, right);
                    evaluationStack.push(new Number(result));
                }
            }
        } else {
            if(funTable.find(postfix[i]->getName()) != funTable.end()) {
                cout << "Found in FunTable" << endl;
                Lexem *func_name = postfix[i];
                for(int arg = 0; arg < funTable[func_name->getName()].argNum; arg++) {
                    programStack.push(evaluationStack.top()->getValue());
                    evaluationStack.pop();
                }
                map <string, int> oldVariables = varTable;
                map <string, int*> oldArrays = ArrayTable;
                varTable.clear();
                ArrayTable.clear();
                int tmp_row = funTable[func_name->getName()].row;
                while(tmp_row >= 0) {
                    tmp_row = evaluatePostfix(postfixLines, tmp_row);
                }
                varTable.clear();
                ArrayTable.clear();
                varTable = oldVariables;
                ArrayTable = oldArrays;
                if(!programStack.empty()) {
                    evaluationStack.push(new Number(programStack.top()));
                    programStack.pop();
                }
                continue;
            }
                evaluationStack.push(postfix[i]);               
            }
        }
    return row + 1;
}
