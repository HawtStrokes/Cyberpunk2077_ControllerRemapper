#include "Hashing.h"

namespace HawtLib {
    namespace Crypto {
        unsigned long long DJB2(const std::string& str) {
            unsigned long long hashValue = 5381;
            for (const char c : str) {
                hashValue = ((hashValue << 5) + hashValue) + c;
            }
            return hashValue;
        }

        unsigned long long SDBM(const std::string& str) {
            unsigned long long hashValue = 0;
            for (const char c : str)
                hashValue = c + (hashValue << 6) + (hashValue << 16) - hashValue;
            return hashValue;
        }

    }
}

