#pragma once
#include <iostream>
#include <ostream>
#include <string>
namespace string_process {
    /***
     * Continuous memory
     * Small string buffer: in stack
     * Larger string: same like vector, dynamically on heap
     ***/
    void use_basic_string() {
        std::string res( "a");
        res.append("\tappend sth");
        res.reserve(20);
        std::cout << "capacity: " <<res.capacity() << std::endl;
        std::cout << "length: " <<res.length() << std::endl;
        std::cout << res.substr(1,2) << std::endl;  // O(N) return a copy
        std::cout << res << std::endl;
    }
    void use_regex() {

    }
}
