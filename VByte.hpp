#include <vector>
#include <cstdint>
#include <iostream>
#include <bitset>
#include <deque>
#include "bitarray.hpp"

namespace pfp {
    template <typename T>
    
    class VByte {
        private:
        std::deque<T> arr;
        pfp::BitArray bit_array;
        std::vector<uint8_t> index_cache;
        uint64_t numbers_count = 0;
        uint64_t previous_num = 0;
        T bit_size = 7;
        T max_size = 127;
        T continuation_bit = 128;
        uint32_t byte_amount = 0;
        
        public:
        //std::ofstream log{"debug.log", std::ios::app};
        VByte(uint64_t n) : bit_array(n * 10) {}
        void encode(uint64_t num) {
            if (numbers_count % 64 == 0) {
                index_cache.push_back(arr.size());
            }
            T x = 0;
            while (true) {
                x = num & max_size;
                num >>= bit_size;
                if (num == 0) [[unlikely]] {
                    x |= continuation_bit;
                    arr.push_back(x);
                    bit_array.set(arr.size()-1, 1);
                    //std::cout << "Continuation bit set: " << arr.size()-1 << std::endl;
                    byte_amount++;
                    numbers_count++;
                    break;
                }
                arr.push_back(x);
                byte_amount += 1;
            }
        }

        void encode_sorted(uint64_t num) {
            uint64_t difference = num - previous_num;
            T x = 0;
            while (true) {
                x = difference & max_size;
                difference >>= bit_size;
                if (difference == 0) [[unlikely]] {
                    arr.push_back(x);
                    bit_array.set(arr.size()-1, 1);
                    //std::cout << "Continuation bit set: " << arr.size()-1 << std::endl;
                    byte_amount += 1;
                    break;
                }
                byte_amount += 1;
                arr.push_back(x);
            }
            
            previous_num = num;
        }

        uint64_t scan(uint64_t i) {
            uint64_t numbers_iterator = 0;
            uint64_t bytes_iterator = 0;
            while (numbers_iterator < i) {
                if (bit_array.get(bytes_iterator)) {
                    numbers_iterator++;
                    bytes_iterator++;
                }
                else {
                    bytes_iterator++;
                }
            }
            uint64_t x = 0;
            uint64_t shift = 0;
            while (true) {
                x |= (bit_array.get(bytes_iterator)) << shift;
                shift += bit_size;
                if (arr[bytes_iterator] & continuation_bit) {
                    break;
                }
                bytes_iterator++;
            }
            return x;
        }

        uint64_t scan_sorted(uint64_t i) {
            previous_num = 0;
            uint64_t numbers_iterator = 0;
            uint64_t bytes_iterator = 0;
            uint64_t x = 0;
            uint64_t shift = 0;
            while (numbers_iterator < i) {
                x = 0;
                shift = 0;
                while (true) {
                    uint64_t chunk = arr[bytes_iterator];
                    x |= (chunk & max_size) << shift;
                    shift += bit_size;
                    if (bit_array.get(bytes_iterator)) {
                        //std::cout << "Previous number: " << previous_num << " New Number: " << x << std::endl;
                        previous_num += x;
                        bytes_iterator++;
                        numbers_iterator++;
                        break;
                    }
                    bytes_iterator++;
                }
            }
            x = 0;
            shift = 0;
            while (true) {
                uint64_t chunk = arr[bytes_iterator];
                x |= (chunk & max_size) << shift;
                shift += bit_size;
                if (bit_array.get(bytes_iterator)) {
                    break;
                }
                bytes_iterator++;
            }
            return x + previous_num;
        }


        uint64_t get_size() {
            return byte_amount;
        }

        void decode_all() {
            uint64_t i = 0;
            while (i < arr.size()) {
                uint64_t x = 0;
                uint64_t shift = 0;

                while (true) {
                    uint64_t chunk = arr[i];
                    //std::cout << "Iterator: " << i << " Continuation bit: " << bit_array.get(i) << std::endl;
                    x |= (chunk & max_size) << shift;
                    shift += bit_size;
                    if (bit_array.get(i)) {
                        i++;
                        break;
                    }
                    i++;
                }
                std::cout << x << "  ";
            }
        }

        void decode_all_sorted() {
            previous_num = 0;
            uint64_t i = 0;
            while (i < arr.size()) {
                uint64_t x = 0;
                uint64_t shift = 0;
                while (true) {
                    uint64_t chunk = arr[i];
                    x |= (chunk & max_size) << shift;
                    shift += bit_size;
                    if (bit_array.get(i)) {
                        i++;
                        break;
                    }
                    i++;
                }
                std::cout << x + previous_num << "  ";
                previous_num = x + previous_num;
            }
        }

        void operator[](uint64_t i) {
            // Closest cached position
            uint64_t loc = index_cache[i / 64];
            //log << "Closest position: " << loc << " Index searched: " << i << std::endl;
            uint64_t offset = i % 64;

            while (offset > 0) {
                if (arr[loc] & continuation_bit) {
                    offset--;
                }
                loc++;
            }
            //log << "Position found: " << loc << std::endl;
            uint64_t shift = bit_size;
            uint64_t x = 0;
            uint64_t chunk;
            do {
                chunk = arr[loc++];
                x |= (chunk & 127) << shift;
                shift += bit_size;
            } while(!(chunk & 128));
            std::cout << x << "\n";
        }

        void set_byte_size(int size) {
            bit_size = size;
            max_size = (1ULL << size) -1;
            continuation_bit = 1ULL << size;
        }
    };
}
