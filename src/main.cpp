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
    for(int i = 0; i < infixLines.size(); ++i) {
        for(int j = 0; j < infixLines[i].size(); ++j) {
            infixLines[i][j]->print();
            cout << " ";
        }
    }
    cout <<  "initLabels start" << endl;
    for(int row = 0; row < (int)infixLines.size(); ++row) {
        cout << "row: " << row << endl; 
        initLabels(infixLines[row], row);
    }
     cout <<  "initLabels end" << endl;
     cout <<  "initJumps start" << endl;
    initJumps(infixLines);
    cout <<  "initJumps end" << endl;
    cout <<  "PostfixLines start" << endl;
    for(int i = 0; i < (int)infixLines.size(); ++i) {
        postfixLines.push_back(buildPostfix(infixLines[i]));
    }
    cout <<  "PostfixLines end" << endl;
    int row = 0;
    cout << "Evaluate start " << endl;
    while(0 <= row && row < (int)postfixLines.size()) {
        row = evaluatePostfix(postfixLines, row);
    }
    cout << "Evaluate end " << endl;
    map <string, int> ::iterator it = varTable.begin();
    for (int i = 0; it != varTable.end(); it++, i++) {
        cout << i << ") Key " << it->first << ", value " << it->second << endl;
    }
    map <string, int *> :: iterator itt = ArrayTable.begin();
    for(int i = 0; itt != ArrayTable.end(); itt++, i++) {    
        int j = 0;
        cout << i << ") Key " << itt->first << ", value: [";
        for (j = 0; j < sizeof(itt->second) / sizeof(int) - 1; ++j)
            cout << itt->second[j] << ", ";
        cout << itt->second[j] << "]";
    }
    varTable.clear();
    ArrayTable.clear();

    return 0;
}


