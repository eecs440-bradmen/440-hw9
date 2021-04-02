#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

#include <vector>
#include <cmath>
#include <string>
#include <string.h>
// #include "/opt/homebrew/opt/openssl@1.1"
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
        std::pair<uint64_t, uint64_t> hashes = hash(s);
        for (int i = 0; i < num_hashes; i++) {
            //compute index to set
            int to_set = (hashes.first + (i * hashes.second)) % container.size();
            container[to_set] = true;
        }
    }

    bool contains(const std::string& s) {
        //Hash the string into two unique hashes
        std::pair<uint64_t, uint64_t> hashes = hash(s);
        for (int i = 0; i < num_hashes; i++) {
            //compute index to set
            int to_check = (hashes.first + (i * hashes.second)) % container.size();
            if (!container[to_check])
                return false;
        }
        //all hash function indexes true, search term probably in bloom filter
        return true;
    }

private:
    int num_hashes;
    std::vector<bool> container;
    std::pair<uint64_t, uint64_t> hash(const std::string& datum) {
        //Use MD5 to hash the string into one 128 bit hash, and split into 2 hashes
        unsigned char val[16];
        MD5((const unsigned char *)datum.c_str(), datum.length(), val);
        uint64_t first, second;
        memcpy(&first, val, 8);
        memcpy(&second, val + 8, 8);
        return {first, second};
    }
};

#endif
