/*****************************************************
 * Author: Eric Hansson
 * File: Grammar.cpp
 * Date: 2/13/2024
 * Purpose: Implementation of grammar for parsing code
 * 
*******************************************************/
#include "Grammar.hpp"
#include "Block.hpp"
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

   if(tok.type()==SEMICOLON) {
       return stmtVal + "; " + slist(is);
   }
   else {
       is.seekg(pos);
       return "";
   } 
   
}

string stmt(std::istream &is){
    int pos = is.tellg();
    Token tok;
    tok.get(is);

    string result;
    if (tok.type()==ID) {
        string funcCall = tok.value();
        result += tok.value(); 
        tok.get(is);

        if (tok.type()==LPAREN) {
            result += tok.value();
            tok.get(is);
            
            if(tok.type()==RPAREN){
                if (funcCall=="dump") {
                    // cout << "Variables:" << endl;
                    // for(auto i : freeList){
                    // cout << varName << ":" << block.address() << "(" << block.size() << ") " << "[" << block.ref_count() << "]" << endl;
                    // }
                    // cout << "Free List" << endl;
                }
                if (funcCall=="compress") {
                     
                }
                // execute dump();
                // or 
                // comprees();
                return result += tok.value();
            }

            else if (tok.type()==ID){
                result += tok.value();
                tok.get(is);
                if (tok.type()!=RPAREN) {
                    is.seekg(pos);
                    cerr << "Expected '(', got: " << tok << endl; 
                }
                // execute free(var)
                return result+=tok.value();
            }
            else {
                is.seekg(pos);
                return result;
            }
        }
        else if (tok.type()==ASSIGNOP) {
            // execute var = alloc(INT);
            // or
            // execute a = b or a = a
            result += "=";
            return result += rhs(is); 
        }
        else {
            is.seekg(pos);
            return result;
        }
    }
    else {
        is.seekg(pos);
        return result;
    }

}

string rhs(std::istream &is){
    // Read token
    Token tok;    
    tok.get(is);
    
    string idVal;
    int blockSize;
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
                blockSize = stoi(intVal);
            }

            tok.get(is);

            if (tok.type()!=RPAREN) {
                cerr << "Expected ')', got: " << tok << endl;
            }
            // Create a new block and add it to the freeList
            //Block* allocBlock = new Block(1,blockSize,0);

            return idVal + "(" + intVal + ")";

        }
        else {
            is.seekg(pos);
            // execute var = newVar;
            return idVal;
        }
    }
    else {
        cerr << "Unexpected token: " << tok << endl;
        return 0;
    }
}
