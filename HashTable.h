#ifndef CS300_HW3_HASHTABLE_H
#define CS300_HW3_HASHTABLE_H

#include <list>
#include <vector>

//CS300-HW3 CAGRI CAN SARACAYDIN 30984

using namespace std;

bool isPrime( int n ) // from lecture notes
{
    if ( n == 2 || n == 3 )
        return true;

    if ( n == 1 || n % 2 == 0 )
        return false;

    for ( int i = 3; i * i <= n; i += 2 )
        if ( n % i == 0 )
            return false;

    return true;
}


int nextPrime( int n ) // from lecture notes
{
    if ( n % 2 == 0 )
        n++;

    for ( ; ! isPrime( n ); n += 2 )
        ;

    return n;
}


template <class Key, class Value>
class HashTable {
private:
    vector<list<pair<Key, Value>>> table; // storing LL's in key value pairs in vector
    unsigned long tableSize;
    unsigned long itemCount; // total

    unsigned long hashFunction(const Key& key) const { //sum each char in string
        unsigned long hashValue = 0;
        for (char ch : key) {
            hashValue += ch;
        }
        return hashValue % tableSize;
    }

    double loadFactor() const {
        return (double)itemCount / tableSize;
    }

    void rehash() {

        vector<list<pair<Key, Value>>> oldTable = table;
        tableSize = nextPrime(tableSize * 2);  // get the new table size
        table = vector<list<pair<Key, Value>>>(tableSize);  // create rehased new table
        itemCount = 0; // empty table
        // table stores list , list stores pairs
        for (const auto& Lists : oldTable) {  // for each list in the hashtable
            for (const auto& item : Lists) {  // insert each pair to the rehased table
                insert(item.first, item.second);
            }
        }
    }

    void checkLoadFactorAndRehash() { // printing and rehasing acoording to loadfactor
        if (loadFactor() > 0.9) {
            rehash();
        }
    }

public:
    HashTable(unsigned long size = 256) : tableSize(nextPrime(size)), itemCount(0) { //same size from samples
        table.resize(tableSize);
    }

    ~HashTable() {
        makeEmpty();
    }

    void insert(const Key& key, const Value& value) {

        unsigned long index = hashFunction(key);
        list<pair<Key, Value>>& theList = table[index];
        bool keyExists = false;

        for (auto& entry : theList) { // for each pair in that indexed LL' check if it is already inserted
            if (entry.first == key) {
                entry.second = value;
                keyExists = true;
                break;
            }
        }

        if (!keyExists) { // add the new word
            theList.emplace_back(key, value); // = list library insert_back
            itemCount++;
            checkLoadFactorAndRehash();
        }
    }


    Value search(const Key& key) const {
        unsigned long index = hashFunction(key);
        const list<pair<Key, Value>>& theList = table[index];

        for (const auto& item : theList) { // if exists return value
            if (item.first == key) {
                return item.second;
            }
        }

        return nullptr;
    }

    void makeEmpty() {
        for (unsigned int i = 0; i < table.size(); ++i) {
            table[i].clear();  // only deleting lists not vector
        }
        itemCount = 0;
    }
};

#endif // CS300_HW3_HASHTABLE_H
