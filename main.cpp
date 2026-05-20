#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include "VByte.hpp"
#include <fcntl.h>
//#include <windows.h>

//uint64_t get_time_ms() {
//    LARGE_INTEGER frequency, counter;
//    QueryPerformanceFrequency(&frequency);
//    QueryPerformanceCounter(&counter);
//    return (counter.QuadPart * 1000) / frequency.QuadPart;
//}



void test(std::istream& in, bool sorted, int bits, bool timed, bool naive) {
    std::ofstream log("debug.log", std::ios::trunc);
    uint64_t n = 0;
    in.read(reinterpret_cast<char*>(&n), sizeof(uint64_t));
    //log << n << " Amount of numbers" << "\n";
    uint64_t i = 0;
    uint64_t start = 0;
    //if (timed) {
    //    start = get_time_ms();
    //}
    if (bits < 8) {
        pfp::VByte<uint8_t> vbyte(n);
        vbyte.set_byte_size(bits);
        while (i < n) {
            uint64_t val;
            in.read(reinterpret_cast<char*>(&val), sizeof(uint64_t));
            //log << val << std::endl;
            vbyte.encode(val);
            i++;
        }
        //log << "ENCODING DONE" << " Time taken: " << get_time_ms() - start << "ms" << "\n";
        i = 0;
        uint64_t query_amount;
        in.read(reinterpret_cast<char*>(&query_amount), sizeof(uint64_t));
        //log << query_amount << "\n";
        while (i < query_amount) {
            uint64_t target = 0;
            in.read(reinterpret_cast<char*>(&target), sizeof(uint64_t));
            //log << "target: " << target << " vbyte size: " << vbyte.get_size() << std::endl;
            vbyte[target];
            //log << value << std::endl;
            i++;
        }
    } 
    //if (timed) {
    //    uint64_t end = get_time_ms();
    //    log << "Time: " << end-start << "ms" << "\n";
    //}
    //log.flush();
}

int main(int argc, char const* argv[]) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    bool sorted = false;
    bool timed = true;
    bool naive = false;
    int bits = 7;
    int input_file = 0;
    int i = 1;
    while (i < argc) {
        std::string s(argv[i++]);
        if (s.compare("-s") == 0) {
            sorted = true;
        }
        else if (s.compare("-k") == 0) {
            if (i < argc) {
                bits = std::stoi(argv[i]);
                i++;
            }
        }
        else if (s.compare("-t") == 0) {
            timed = true;
        }
        else if (s.compare("-n") == 0) {
            naive = true;
        }
        else if (s.compare("-q") == 0) {
            //std::cout << "Whoah" << std::endl;
        }
        else if (s.compare("-l") == 0) {
            //std::cout << "Whoah" << std::endl;
        }
        else {
            input_file = i - 1;
        }
    }
    //std::cerr << "Args given: " << bits << " Sorted: " << sorted << " input file: "<< input_file << "\n";
    if (input_file > 0) {
        std::ifstream in(argv[input_file], std::ios::binary);
        test(in, sorted, bits, timed, naive);
    }
    else {
        test(std::cin, sorted, bits, timed, naive);
    }
    return 0;
}
