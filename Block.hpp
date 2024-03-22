#ifndef _BLOCK_HPP_
#define _BLOCK_HPP_

#include <iostream>

class Block{
private:
    std::string _var_name;
    int _ref_count;
    int _size;
    int _address;

public:
    Block() :  _var_name(), _ref_count(0), _size(0), _address() {}

    Block(std::string var_name, int ref_count, int size, int address) : _ref_count(ref_count), _size(size), _address(address) {}

    int ref_count() const {return _ref_count;}
    int& ref_count() {return _ref_count;}

    int size() const {return _size;}
    int& size() {return _size;}

    int address() const {return _address;}
    int& address() {return _address;}

    void show_info();
};

#endif
