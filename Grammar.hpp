/*********************************************************
 * Author: Eric Hansson
 * File: Grammar.hpp
 * Date: 2/13/2024 
 * Purpose: Implementing grammar for parser
**********************************************************/
#ifndef _GRAMMAR_HPP_
#define _GRAMMAR_HPP_

#include <istream>

// Grammar for program
std::string program(std::istream &);

// Grammar for slist
std::string slist(std::istream &);

// Grammar for stmt
std::string stmt(std::istream &);

// Grammar for rhs
std::string rhs(std::istream &);

#endif
