#include <vector>
namespace pfp {
class BitArray {
    private:
    std::vector<uint64_t> arr;
    std::vector<uint64_t> sum_cache;
    bool cache_built = false;
    uint64_t size;

    public:
    BitArray(uint64_t n) {
        size = n;
        //std::cout << size << std::endl;
        arr.resize((n + 63) / 64, 0);
        sum_cache.resize(arr.size(), 0);
    }
    void set(uint64_t i, uint64_t b){
        if (i < size) {
            if (!(arr[i/64] & (1ULL << (i%64)))) {
                sum_cache[i/64]++;
            }
            arr[i/64] |= (b << (i%64));
            //std::cout << sum_cache[i/64]<<" sum cache" << i/64 << " updated "<< std::endl;
        }
    }

    bool get(uint64_t i) {
        if (i < size) {
            return arr[i/64] & (1ULL << (i%64));
        }
        return 0;
    }

    uint64_t sum(uint64_t i) {
        // since sum commands come after all set commands we can build the cache only once.
        if (!cache_built) {
            create_sum_cache();
        }

        // improved this to use new sum cache.
        int amount = (i/64);
        int offset = (i%64);
        uint64_t value = sum_cache[amount];
        uint64_t mask = (1ULL << offset) - 1;
        value += __builtin_popcountll(arr[amount] & mask);
        return value;
    }


    uint64_t location(uint64_t i) {
        uint64_t low = 0;
        uint64_t high = size;
        //std::cout << "Finding position of: " << i << std::endl;
        while (low <= high) {
            uint64_t mid = ((high - low) / 2) + low;

            uint64_t value = sum(mid);

            if (value == i) [[unlikely]] {
                //std::cout << "index that we got: " << value << " : ";
                if (mid > 0) {
                    uint64_t check = mid - 1;
                    if (arr[check/64] & (1ULL << (check%64))) {
                        return mid;
                    }
                    return previous(mid);
                }
                return 0;
            }
            else if (value < i) {
                low = mid  + 1;
            }
            else {
                high = mid - 1;
            }
        }
        std::cout << "Couldn't find size" << std::endl;
        return size;
    }
    uint64_t previous(uint64_t i) {
        uint64_t current = i - 1;
        uint64_t check = current - 1;
        if (arr[check/64] & (1ULL << (check%64))) {
            return current;
        }
        return previous(current);
    }
    void create_sum_cache() {
        sum_cache[0] = 0;
        for (size_t i = 1; i < sum_cache.size(); i++) {
            sum_cache[i] = sum_cache[i-1] + __builtin_popcountll(arr[i-1]);
            //log << "Iterator: " << i << "Sum cache size: " << (int)sum_cache[i] << std::endl;
        }
        cache_built = true;
        //log << "Total sum cache: " << total << std::endl;
    }

};
}