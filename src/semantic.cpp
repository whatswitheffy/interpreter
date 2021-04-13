#include "semantic.h"

int evaluatePostfix(vector <Lexem *> postfix, int row) {
    int result = 0;
    stack <Lexem *> evaluationStack;
    Lexem *left, *right;
    for(int i = 0; i < postfix.size(); ++i) {
        if(postfix[i]->isOperator()) {
            Operators *lexemop = (Operators *)postfix[i];
            if(lexemop->getType() == COLON) {
                continue;
            }
            if(lexemop->getType() == GOTO ||
                lexemop->getType() == ELSE ||
                lexemop->getType() == ENDWHILE ) {
                Goto * lexemgoto = (Goto *)lexemop;
                return lexemgoto->getRow();
            } else if(lexemop->getType() == IF ||
                lexemop->getType() == WHILE ) {
                Goto * lexemgoto = (Goto *)lexemop ;
                int rvalue = evaluationStack.top()->getValue();
                evaluationStack.pop();
                if(!rvalue) {
                    return lexemgoto->getRow();
                }
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
                evaluationStack.push(postfix[i]);               
            }
        }
    return row + 1;
}
int main() {
    return 0;
}