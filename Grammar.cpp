/*****************************************************
 * Author: Eric Hansson
 * File: Grammar.cpp
 * Date: 2/13/2024
 * Purpose: Implementation of grammar for parsing code
 * 
*******************************************************/
#include "Grammar.hpp"
#include "Token.hpp"
#include <iostream>
#include <string>

using namespace std;

// Grammar for the program 
string program(std::istream &is){
    return slist(is);
}

string slist(std::istream &is){
    string stmtVal = stmt(is);

    int pos = is.tellg();
    Token tok;
    tok.get(is);

    if (tok.type()==SEMICOLON) {
        return stmtVal + ";" + " " + slist(is) + ";";
    } 
    else {
        is.seekg(pos);
        return stmtVal;
    }
}

string stmt(std::istream &is){
    //string rhsVal = rhs(is);

    int pos = is.tellg();

    Token tok;
    tok.get(is);

    string idVal;
    if (tok.type()==ID) {
        idVal = tok.value();

        tok.get(is);

        if (tok.type()==LPAREN) {
            string parenVal; 
            tok.get(is);

            if (tok.type()==RPAREN) {
                is.seekg(pos);
                return idVal + "()"; 
            }

            if (tok.type()!=ID) {
                is.seekg(pos);
                cerr << "Expected a type of ID, got: " << tok << endl;
                return 0;
            }
            if (tok.type()==ID) { 
                parenVal = tok.value();
            }

            tok.get(is);

            if (tok.type()!=RPAREN) {
                is.seekg(pos);
                cerr << "Expected ')', got: " << tok << endl;
                return 0;
            }
            
            return idVal + "("+ parenVal +")"; 
        }
        else if (tok.type()==ASSIGNOP) {
            return idVal + "=" + rhs(is); 
        }
        else {
            cerr << "Unexpected token: " << tok << endl;
            return 0;
        }
    }
    else {
        cerr << "Error stmt!!!" << endl;
        return 0;
    }
}

string rhs(std::istream &is){
    // Read token
    Token tok;    
    tok.get(is);
    
    string idVal;
    // if ID, then ...
    if(tok.type()==ID){
        int pos = is.tellg();
        
        idVal = tok.value();
        
        tok.get(is);

        if (tok.type()!=LPAREN) {
            is.seekg(pos);
            return idVal;
        }
        else if (tok.type()==LPAREN) {
            string intVal; 
            tok.get(is);

            if (tok.type()!=NUM_INT) {
                is.seekg(pos);
                cerr << "ERROR: no NUM_INT" << endl;
            }
            if (tok.type()==NUM_INT) {
                intVal = tok.value();
            }

            tok.get(is);

            if (tok.type()!=RPAREN) {
                cerr << "Expected ')', got: " << tok << endl;
            }

            return idVal + "(" + intVal + ")";
        }
        else {
            is.seekg(pos);
            return idVal;
        }
    }
    else {
        cerr << "Unexpected token: " << tok << endl;
        return 0;
    }
}
