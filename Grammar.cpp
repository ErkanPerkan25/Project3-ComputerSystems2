/*****************************************************
 * Author: Eric Hansson
 * File: Grammar.cpp
 * Date: 3/26/2024
 * Purpose: Implementation of grammar for parsing code
 * and execute the commands.
*******************************************************/
#include "Grammar.hpp"
#include "Block.hpp"
#include "Token.hpp"
#include <iostream>
#include <list>
#include <string>

using namespace std;

// Gets the list from main.cpp
extern list<Block*> block_list;

// Function to sort the blocks from lowest to highest address
bool compareAddress(Block* a, Block* b){
    return  a->address() < b->address();
}

// Global variabe
string ptrId;

// Grammar for the program 
string program(std::istream &is){
    return slist(is);
}

// Grammar fro slist, which return list of stmts
string slist(std::istream &is){
    // reads a stmt
    string stmtVal = stmt(is);

    int pos = is.tellg();

    // Read the next token
    Token tok;
    tok.get(is);
    
    // If SEMICOLON, execute the code
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
    // Read the next token
    Token tok;
    tok.get(is);

    string result;
    // If token type ID, then ...
    if (tok.type()==ID) {
        string funcCall = tok.value();
        // Store data which is used to execute commands
        ptrId = tok.value();
        result += tok.value(); 
        // Read the next token
        tok.get(is);
        
        // If token type LPAREN, then ...
        if (tok.type()==LPAREN) {
            result += tok.value();
            // Read the next token
            tok.get(is);
           
            // If token tpye RPAREN, then ...
            if(tok.type()==RPAREN){
                // Sort the list of blocks
                block_list.sort(compareAddress);
                // if the command is dump();
                if (funcCall=="dump") {
                    // Prints out all the variable blocks
                    cout << "Variables:" << endl;
                    for(auto& i : block_list){
                        if (i->ref_count() >= 1) {
                            cout << (i->name()) << ":" << i->address() << "(" << i->size() << ") " << "[" << i->ref_count() << "]" << endl;
                        }
                    }
                    // prints out all the free blocks
                    cout << "Free List:" << endl;
                    for (auto& i : block_list) {
                        if (i->ref_count() == 0) {
                            cout << i->address() << "(" << i->size() << ") " << "[" << i->ref_count() << "] ";
                        }
                    }
                    cout << endl;
                    cout << "==================================================================" << endl;

                }
                // if the command is compress();
                if (funcCall=="compress") {
                    list<Block*>::iterator it;
                    // Iterats through the list of blocks
                    for (it=block_list.begin(); it!=block_list.end(); ++it) {
                        auto x = it;
                        it++;
                        // If we have reached the end of the list, break
                        if (it == block_list.end()) {
                            break; 
                        }
                        // Look at the next item
                        auto y = it; 
                        it--;
                        // it the block has reference count of 0. then ...
                        if ((*x)->ref_count()==0) {
                            // if the blocks are adjecent, then ...
                            if ((*y)->address() == (*x)->address() + (*x)->size() && (*y)->ref_count()==0) {
                                // add the sizes toghether
                                (*x)->size() += (*y)->size();
                                // and remove one of the blocks
                                block_list.remove(*y);
                                it--;
                            } 
                        }
                    }
                }
                // returns a string of the stmt
                return result += tok.value();
            }

            else if (tok.type()==ID){
                // stores the ID value
                string varId = tok.value();

                result += tok.value();

                // reads the next token
                tok.get(is);

                // it not type RPAREN, give error
                if (tok.type()!=RPAREN) {
                    is.seekg(pos);
                    cerr << "Expected '(', got: " << tok << endl; 
                }
                int address;
                // execute free(var)
                // loops through the list of block
                for (auto& var : block_list) {
                    // if it find the right block
                    if (varId == var->name()) {
                        address = var->address();
                        // if the block has a reference count of 1, then ...
                        if (var->ref_count() == 1) {
                            // make it to free block
                            Block* ptr = new Block(0,var->size());
                            ptr->name() = "";
                            ptr->address() = address;
                            block_list.push_front(ptr);
                        }
                        // remove the old block
                        block_list.remove(var);
                        break;
                    } 
                }
                // check if there is block with the same address 
                for (auto& var : block_list) {
                    // if it has the same address, remove a reference count
                    if (address == var->address() && var->ref_count()>1) {
                        var->ref_count()--; 
                        break;
                    } 
                }
                // return the stmt
                return result+=tok.value();
            }
            else {
                is.seekg(pos);
                return result;
            }
        }
        else if (tok.type()==ASSIGNOP) {
            // execute a = b or a = a
            result += "=";
            // gets the rhs variable
            string id = rhs(is);

            // goes throughs and see if the block already exits
            for (auto& var : block_list) {
                // if var = var, then don't do anything
                if (ptrId == id) {
                   break; 
                }
                // if newVar = var, then ...
                if (var->name() == id) {
                    // make new block
                    Block* ptr = new Block(ptrId, var->size());
                    // give every detail to it
                    ptr->address() = var->address();
                    ptr->ref_count() = var->ref_count();
                    var->ref_count()++;
                    ptr->ref_count()++;
                    block_list.push_front(ptr);
                } 
            }
            
            // return the stmt
            result += id; 
            return result;
        }
        else {
            // returns the current result string
            is.seekg(pos);
            return result;
        }
    }
    // return the var ID
    else {
        is.seekg(pos);
        return result;
    }

}

// Grammar for rhs 
string rhs(std::istream &is){
    // Read token
    Token tok;    
    tok.get(is);

    // declared variables
    string idVal;
    string result;
    int blockSize;
    // if ID, then ...
    if(tok.type()==ID){
        int pos = is.tellg();
        result+=tok.value();
        
        // stores the ID token
        idVal = tok.value();
        
        // reads the next token
        tok.get(is);
        
        // if token is not type LPAREN, return the ID
        if (tok.type()!=LPAREN) {
            is.seekg(pos);
            return result;
        }
        // if token is tpye LPAREN, then ...
        else if (tok.type()==LPAREN) {
            string intVal; 
            result+=tok.value();
            
            // read the next token
            tok.get(is);

            // If not type of NUM_INT, give error
            if (tok.type()!=NUM_INT) {
                is.seekg(pos);
                cerr << "ERROR: no NUM_INT" << endl;
            }
            // If type NUM_INT
            if (tok.type()==NUM_INT) {
                // store the token
                result+=tok.value();
                intVal = tok.value();
                // convert the string to integer
                blockSize = stoi(intVal);
            }

            // Read the next token
            tok.get(is);

            if (tok.type()!=RPAREN) {
                cerr << "Expected ')', got: " << tok << endl;
            }
            // Create a new block and add it to the freeList
            for (auto& var : block_list) {
                if (var->name() == ptrId) {
                    var->ref_count()--; 
                    var->name() = "";
                }
                // the there is enough space in freelist block, then add the new allocated block
                if (var->size() >= blockSize && var->ref_count()==0) {
                    Block* ptr = new Block(ptrId,blockSize,var->address());
                    var->size() = var->size()-blockSize;
                    var->address() = var->address()+blockSize;
                    // push the new block to the list
                    block_list.push_front(ptr);
                    break;
                } 
            }

            return result+=")";
        }
        else {
            // return the idVal
            is.seekg(pos);
            return idVal;
        }
    }
    // otherwise we read a unexpected token, give error message
    else {
        cerr << "Unexpected token: " << tok << endl;
        return 0;
    }
}
