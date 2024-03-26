/***************************************************************
 * Author: Eric Hansson
 * File: Block.hpp
 * Date: 3/26/2024
 * Purpose: To have a class object which store information for
 * both a allocated block and unallocated on
*************************************************************/
#ifndef _BLOCK_HPP_
#define _BLOCK_HPP_

#include <iostream>

class Block{
private:
    // Name for the block
    std::string _name;
    // References it has
    int _ref_count;
    // The size of it
    int _size;
    // The address of the memory block
    int _address;

public:
    // Deafult contructor
    Block() :  _name(), _ref_count(0), _size(0), _address() {}
   
    // Othe Constructors
    Block(std::string name, int size) : _name(name), _ref_count(1), _size(size) {}

    Block(int ref_count, int size) : _ref_count(ref_count), _size(size) {}

    Block(std::string name, int size, int address) : _name(name), _ref_count(1), _size(size), _address(address) {}

    // Functions to get and change the data of the memory blocks
    std::string name() const {return _name;}
    std::string& name() {return _name;}

    int ref_count() const {return _ref_count;}
    int& ref_count() {return _ref_count;}

    int size() const {return _size;}
    int& size() {return _size;}

    int address() const {return _address;}
    int& address() {return _address;}
};

#endif
