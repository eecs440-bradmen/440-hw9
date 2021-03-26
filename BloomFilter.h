#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

#include <vector>
#include <cmath>
#include <string>
#include <string.h>
#include <openssl/md5.h>

class Bloomfilter {
public:
    Bloomfilter(int num_objects, double false_positive_rate) {
        //Determine the size of bits of our data vector, and resize
        int size = -1 * num_objects * std::log(false_positive_rate) / std::pow(std::log(2), 2); //size in bits
        //Determine number of hash functions to use
        num_hashes = size / num_objects * std::log(2); 
        container.resize(size);
    }

    void insert(const std::string& s) {
        //Hash the string into two unique hashes

        //Use double hashing to get unique bit, and repeat for each hash function

    }

    bool contains(const std::string& s) {
        //Hash the string into two unqiue hashes

        //Use double hashing to get unique bit, and repeat for each hash function
        //If bit is false, we know for certain this unique string has not been inserted

        //If all bits were true, the string is likely inserted, but false positive is possible

        //This line is for compiling, replace this with own code
        return false;
    }

private:
    //Add any private member variables that may be neccessary
    int num_hashes;
    std::vector<bool> container;
    std::pair<uint64_t, uint64_t> hash(const std::string& datum) {
        //Use MD5 to hash the string into one 128 bit hash, and split into 2 hashes
        unsigned char val[16];
        MD5(datum.c_str(), datum.length(), val);
        uint64_t first, second;
        memcpy(&first, val, 8);
        memcpy(&second, val + 8, 8);
        return {first, second};
    }
};

#endif