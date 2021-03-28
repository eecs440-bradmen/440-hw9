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
        int hashSize = 64 / num_hashes; // split into num_hashes sets of bits for each "hash function"
        // setup mask for getting each set of bits
        int mask = 0;
        int to_add = 1;
        for (int i = 0; i < hashSize; i++) {
            mask += to_add;
            to_add *= 2;
        }
        //Use double hashing to get unique bit, and repeat for each hash function
        for (int i = 0; i < num_hashes; i++) {
            // get lowest (hashSize) bits from both parts of hash
            int first = (hashes.first) & mask; 
            int second = (hashes.second) & mask; 
            // shift hashes 
            hashes.first = hashes.first >> (i * hashSize);
            hashes.second = hashes.second >> (i * hashSize);
            //compute index to set
            int to_set = (first + (i * second)) % container.size();
            container[to_set] = true;
        }
    }

    bool contains(const std::string& s) {
        //Hash the string into two unique hashes
        std::pair<uint64_t, uint64_t> hashes = hash(s);
        int hashSize = 64 / num_hashes; // split into num_hashes sets of bits for each "hash function"
        // setup mask for getting each set of bits
        int mask = 0;
        int to_add = 1;
        for (int i = 0; i < hashSize; i++) {
            mask += to_add;
            to_add *= 2;
        }
        //Use double hashing to get unique bit, and repeat for each hash function
        for (int i = 0; i < num_hashes; i++) {
            // get lowest (hashSize) bits from both parts of hash
            int first = (hashes.first) & mask; 
            int second = (hashes.second) & mask; 
            // shift hashes 
            hashes.first = hashes.first >> (i * hashSize);
            hashes.second = hashes.second >> (i * hashSize);
            //compute index to set
            int to_check = (first + (i * second)) % container.size();
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
