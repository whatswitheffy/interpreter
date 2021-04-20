#include <string>
#include <vector>
#include <iostream>
#include "lexem.h"
#include "semantic.h"
#include "lexical.h"
#include "syntax.h"
#include "const.h"
#include <stack>

using namespace std;

int main () {
    string codeline;
    vector <vector <Lexem *>> infixLines, postfixLines;
    getline(cin, codeline);
    while(codeline != "exit") {
        infixLines.push_back(parseLexem(codeline));
        getline(cin, codeline);
    }
    for(int row = 0; row < (int)infixLines.size(); ++row) {
        initLabels(infixLines[row], row);
    }
    initJumps(infixLines);
    for(int i = 0; i < (int)infixLines.size(); ++i) {
        postfixLines.push_back(buildPostfix(infixLines[i]));
    }
    map <string, Variable *> :: iterator it = varTable.begin();
    for(int i = 0; it != varTable.end(); it++, i++) {  
        cout << i << ") Ключ " << it->first << ", значение " << it->second << endl;
    }
    int row = 0;
    while(0 <= row && row < (int)postfixLines.size()) {
        row = evaluatePostfix(postfixLines[row], row);
    }
    map <string, int *> :: iterator itt = ArrayTable.begin();
    for(int i = 0; itt != ArrayTable.end(); itt++, i++) {    
        cout << i << ") Ключ " << itt->first << ", значение " << itt->second << endl;
    }
    return 0;
}


