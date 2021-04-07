#include <string>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

// t = token, l = lexeme, ct = current token pointer, fl = flag
bool isDeclaritive(vector<string> t, vector<string> l, int ct);
bool isType(vector<string> t, vector<string> l, int ct);
bool isIdentifier(vector<string> t, vector<string> l, int ct);
bool isStatement(vector<string> t, vector<string> l, int ct, int& fl);
bool isAssign(vector<string> t, vector<string> l, int ct);
bool isExpression(vector<string> t, vector<string> l, int ct);
bool isExpressionPrime(vector<string> t, vector<string> l, int ct);
bool isTerm(vector<string> t, vector<string> l, int ct);
bool isTermPrime(vector<string> t, vector<string> l, int ct);
bool isFactor(vector<string> t, vector<string> l, int ct);

int main(){
    //constructors
    vector<string> token, lexeme;
    string literal;
    ifstream tokenFile, lexemeFile;

    //token vector creation
    tokenFile.open("token.txt");
    if (!tokenFile.is_open()){
        cout << "error opening token file" << endl;
        return 1;
    }
    while (tokenFile.good()){
        tokenFile >> literal;
        token.push_back(literal);
    }

    //matching lexeme vector creation
    lexemeFile.open("lexeme.txt");
    if (!lexemeFile.is_open()){
        cout << "error opening lexeme file" << endl;
        return 1;
    }
    while (lexemeFile.good()){
        lexemeFile >> literal;
        lexeme.push_back(literal);
    }

    //syntax analyzer starting first with arithmetic, then assignment, then declarative
    for (int currentToken=0, expressionFlag=0, statementFlag=0; currentToken<token.size(); currentToken++){
        cout << "Token: " << token[currentToken] << "\t\t\tLexeme: " << lexeme[currentToken] << endl;
        
        //test for expression
        if (isExpression(token, lexeme, currentToken));

        //test for assignment
        else {
            isStatement(token, lexeme, currentToken, statementFlag);
        }
        
        //test to see if previous production rule was an assignment or declarative statement
        if (statementFlag == 1){
            //output next token/lexeme and complete production rules with new token
            currentToken++;
            if (currentToken<token.size()){
                cout << "Token: " << token[currentToken] << "\t\t\tLexeme: " << lexeme[currentToken] << endl;
                //clear flag before leaving statement
                statementFlag = 0;
            }
        }
    }

    return 0;
}

//test for single declarative non-terminal
bool isDeclaritive(vector<string> t, vector<string> l, int ct){
    bool flag = false;
    if (isType(t, l, ct)){
        ct++;
        if (isIdentifier(t, l, ct)){
            cout << "<Declarative> -> <Type> <ID>" << endl;
            flag = true;
        }
    }
    return flag;
}

//test for type non-terminal
bool isType(vector<string> t, vector<string> l, int ct){
    bool flag = false;
    if (l[ct] == "bool"){
        cout << "<Type> -> bool" << endl;
        flag = true;
    }
    else if (l[ct] == "float"){
        cout << "<Type> -> float" << endl;
        flag = true;
    }
    else if (l[ct] == "int"){
        cout << "<Type> -> int" << endl;
        flag = true;
    }
    return flag;
}

//test for identifier non-terminal
bool isIdentifier(vector<string> t, vector<string> l, int ct){
    bool flag = false;
    if (t[ct] == "IDENTIFIER"){
        cout << "<ID> -> id" << endl;
        flag = true;
    }
    return flag;
}

//test for assignment non-terminal
bool isAssign(vector<string> t, vector<string> l, int ct){
    bool flag = false;
    if (isIdentifier(t, l, ct)){
        ct++;
        if (l[ct] == "="){
            ct++;
            if (isExpression(t, l, ct)){
                cout << "<Assign> -> <ID> = <Expression>" << endl;
                flag = true;
            }
        }
    }
    return flag;
}

//test for statemenet non-terminal
bool isStatement(vector<string> t, vector<string> l, int ct, int& fl){
    bool flag = false;
    if (isAssign(t, l, ct)){
        cout << "<Statement> -> <Assign>" << endl;
        flag = true;
        fl = 1;
    }
    else if (isDeclaritive(t, l, ct)){
        cout << "<Statement> -> <Declarative>" << endl;
        flag = true;
        fl = 1;
    }
    else{
        cerr << "Statement is not assignment or declarative statement.." << endl;
        cerr << "Failed on line " << ct << "." << endl;
    }
    return flag;
}

//test for expression non-terminal
bool isExpression(vector<string> t, vector<string> l, int ct){
    bool flag = false;
    
    if (isTerm(t, l, ct)){
        ct++;
        if (isExpressionPrime(t, l, ct)){
            cout << "<Expression> -> <Term> <Expression Prime>" << endl;
            flag = true;
        }
    }
    return flag;
}

//test for expression prime non-terminal
bool isExpressionPrime(vector<string> t, vector<string> l, int ct){
    bool flag = false;
    if (l[ct] == "+"){
        ct++;
        if (isTerm(t, l, ct)){
            ct++;
            if (isExpressionPrime(t, l, ct)){
                cout << "<Expression Prime> -> + <Term> <Expression Prime>" << endl;
                flag = true;
            }
        }
    }
    else if (l[ct] == "-"){
        ct++;
        if (isTerm(t, l, ct)){
            ct++;
            if (isExpressionPrime(t, l, ct)){
                cout << "<Expression Prime> -> - <Term> <Expression Prime>" << endl;
                flag = true;
            }
        }
    }
    else if (t[ct] == "SEPARATOR"){
        cout << "<Expression Prime> -> epsilon" << endl;
        flag = true;
    }
    return flag;
}

//test for term non-terminal
bool isTerm(vector<string> t, vector<string> l, int ct){
    bool flag = false;
    if (isFactor(t, l, ct)){
        ct++;
        if (isTermPrime(t, l, ct)){
            cout << "<Term> -> <Factor> <Term Prime>" << endl;
            flag = true;
        }
    }
    return flag;
}

//test for term prime non-terminal
bool isTermPrime(vector<string> t, vector<string> l, int ct){
    bool flag = false;
    if (l[ct] == "*"){
        ct++;
        if (isFactor(t, l, ct)){
            ct++;
            if (isTermPrime(t, l, ct)){
                cout << "<Term Prime> -> * <Factor> <Term Prime>" << endl;
                flag = true;
            }
        }
    }
    else if (l[ct] == "/"){
        ct++;
        if (isFactor(t, l, ct)){
            ct++;
            if (isTermPrime(t, l, ct)){
                cout << "<Term Prime> -> / <Factor> <Term Prime>" << endl;
                flag = true;
            }
        }
    }
    else if (t[ct] == "SEPARATOR"){
        cout << "<Term Prime> -> epsilon" << endl;
        flag = true;
    }
    return flag;
}

//test for factor non-terminal
bool isFactor(vector<string> t, vector<string> l, int ct){
    bool flag = false;
        if (l[ct] == "("){
            ct++;
            if (isExpression(t, l, ct)){
                ct++;
                if (l[ct] == ")"){
                    cout << "<Factor> -> ( <Expression> )" << endl;
                    flag = true;
                }
            }
        }
        else if (isIdentifier(t, l, ct)){
            cout << "<Factor> -> <ID>" << endl;
            flag = true;
        }
    return flag;
}