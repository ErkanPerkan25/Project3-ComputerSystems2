/*******************************************
 * File: Token.cpp                         *
 * Author: Eric Hansson                    *
 * Date: 1/21/2023                         *
 * PURPOSE: implementation for Token       *
 *******************************************/

#include "Token.hpp"

#include <cctype>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

// DFA tabel
static int **DFA=nullptr;
#define ERROR -1
#define NONE 0
#define AND_BETWEEN 17
#define OR_BETWEEEN 18

// the promised global for string equivalents of TokenType enumeration
string TokStr[]=
{ "ERROR", "EOF_TOK", "NUM_INT", "NUM_REAL", "ADDOP", "MULOP", "ID", "RELOP", "ASSIGNOP", "LPAREN", "RPAREN",  "SEMICOLON",  "LBRACK", "RBRACK", "COMMA", "AND", "OR", "INTEGER", "FLOAT", "WHILE", "IF", "THEN", "ELSE", "VOID", "BEGIN", "END"};

// This is a "list" of the keywords. Note that they are in the same order
//   as found in the TokenType enumaration. 
static string reserved[]={"int" , "float", "while", "if", "then", "else", "void", "begin", "end" };


/******************************************************
 *  just prints out the info describing this Token    *
 *    to specified stream                             *
 *                                                    *
 *   os  - the stream to add the Token to             *
 *                                                    *
 *   returns: the updated stream                      *
 ******************************************************/
ostream&
Token::print(ostream& os) const
{
  os
     << "{ Type:"   << left << setw(10) << TokStr[_type] 
     << " Value:"   << left << setw(10) << _value
     << " Line Number:" << _line_num
     << " }";
  return os;
}

/******************************************************
 *  Fills in information about this Token by reading  *
 *    it from specified input stream                  *
 *                                                    *
 *   is  - the stream to read the Token from          *
 *                                                    *
 *   returns: nothing                                 *
 *                                                    *
 *     **** YOU MUST CODE THIS !!!!!! ****            *
 ******************************************************/
// Deterministic ... 2D-array 

void Token::get(istream &is)
{
  // you must write this code !!!!
    if (!DFA) { // if dfa tabel doesn't exist ... create one
        // we read a file the line num always start on 1
        _line_num = 1;

        // Create the rows, which are the different states
        DFA = new int*[OR+3]; 
        for (int state=NONE; state<=OR+2; state++) {
            // creates the columns, which are the characters
            DFA[state] = new int[256]; 
            for (int ch=0; ch<256; ch++) {
                DFA[state][ch] = ERROR; // initializes all the cells with error
            }
        }

        // all the transitions from start state (NONE or 0)        
        for (char ch='0'; ch<'9'; ch++)
            DFA[NONE][(int) ch] = NUM_INT;

        // all the transition form start state (NONE or 0)
        for (char ch='a'; ch<'z'; ch++)
            DFA[NONE][(int) ch] = ID;

        for (char ch='A'; ch<'Z'; ch++)
            DFA[NONE][(int) ch] = ID;
       
        // Final state from start state 
        DFA[NONE][(int) '+'] = ADDOP;
        DFA[NONE][(int) '-'] = ADDOP;
         
        // Final state from start state 
        DFA[NONE][(int) '*'] = MULOP;
        DFA[NONE][(int) '/'] = MULOP;
        
        // Final state from start state 
        DFA[NONE][(int) '<'] = RELOP;
        DFA[NONE][(int) '>'] = RELOP;

        // Final state from start state 
        DFA[RELOP][(int) '='] = RELOP;
        
        // Final state when assign operator, non-final for relation operator
        DFA[NONE][(int) '='] = ASSIGNOP;
        // Final state for relation operator
        DFA[ASSIGNOP][(int) '='] = RELOP;

        // Final state from start state 
        DFA[NONE][(int) '('] = LPAREN;
        DFA[NONE][(int) ')'] = RPAREN;
        DFA[NONE][(int) '['] = LBRACK;
        DFA[NONE][(int) ']'] = RBRACK;
        
        // Non-final state, gives error
        DFA[NONE][(int) '&'] = AND_BETWEEN;
        // Final state
        DFA[AND_BETWEEN][(int) '&'] = AND;

        // Non-final state, gives error
        DFA[NONE][(int) '|'] = OR_BETWEEEN;
        // Final state
        DFA[OR_BETWEEEN][(int) '|'] = OR;

        DFA[NONE][(int) ','] = COMMA;
        DFA[NONE][(int) ';'] = SEMICOLON;

        for (char ch='a'; ch<'z'; ch++)
            DFA[ID][(int) ch] = ID;

        for (char ch='A'; ch<'Z'; ch++)
            DFA[ID][(int) ch] = ID;

        for (char ch='0'; ch<'9'; ch++)
            DFA[ID][(int) ch] = ID;

        // all transitions from NUM_INT state (NUM_INT or 1)
        for(char ch='0'; ch<'9'; ch++)
            DFA[NUM_INT][(int) ch] = NUM_INT; 

        DFA[NUM_INT][(int) '.'] = NUM_REAL;

        // all transitions form NUM_REAL state (NUM_REAL)
        for(char ch='0'; ch<'9'; ch++)
            DFA[NUM_REAL][(int) ch] = NUM_REAL;

    }
    // fill _value from input file and _type as token type
    _value ="";
    char ch;
    // Check if comments is true or false
    bool comment = false;

    // skip white spacing here
    ch = is.get();

    // If at the end, then EOF_TOK
    if(!is)
        _type = EOF_TOK;

    // Skips white spacing and comments
    while (isspace(ch) || ch == '#' || comment == true ) {
        if(ch=='\n'){ // increments the number if line when there is also a blank line
            _line_num++;
            comment = false;
        }
        // ch is '#', then it is a comment
        if (ch == '#') {
            comment = true; 
        }
        ch = is.get();
    }
    // if at end of the file, token is EOF_TOK 
    if (!is) {
        _type = EOF_TOK;
        return; 
    }

    is.putback(ch);

    // Keeps track of the current state and previous
    int curr=NONE;
    int prev=ERROR;

    while (curr!=ERROR) {
        // get next character from input file
        ch = is.get();
        
        // move to the next state based on character read
        prev = curr;
        curr = DFA[curr][(int) ch];

        if (curr!=ERROR) { // if the character is a valid part of the token ... 
            _value+=ch;    // ... add char to lexeme's value
        }
    }
    // the type of the token
    _type = (TokenType) prev;

    // token is '&' or '|', then their are error
    if (_value == "&" || _value == "|") {
        _type = (TokenType) NONE; 
    }

    // if token is a keyword, change the the type
    if(_type == ID){
        for(int i=0; i < 9; i++){
            if(_value == reserved[i]){
                _type = (TokenType) (i+17);
            }
        }
    }

    // We read one more extra character ... put it back for the next get()
    if (is)
        is.putback(ch);
}
