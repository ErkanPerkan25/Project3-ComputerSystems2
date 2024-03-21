#ifndef _BLOCK_HPP_
#define _BLOCK_HPP_

#include <iostream>

class Block{
private:
    std::string _name;
    int _ref_count;
    int _size;
    // int address or string address
    int _address;

public:
    Block() : _name(), _ref_count(1), _size(0), _address() {}

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
