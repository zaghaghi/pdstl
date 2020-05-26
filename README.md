# PDSTL
Probabilistic Data Structure Template Library, is the template library for probabilistic data structures.
Most part of data structures and algorithms come from this book:

[Probabilistic Data Structures and Algorithms for Big Data Applications](https://pdsa.gakhov.com/) by Andrii Gakhov

2019, ISBN: 978-3748190486 (paperback) ASIN: B07MYKTY8W (e-book)

# Implemented Data Structures
## Membership - Bloom Filter
Simple usage:
```c++
#include <membership/bloom_filter.h>
#include <vector>
#include <string>

int main() {
    std::vector<std::string> urls{
        "https://google.com",
        "https://instagram.com",
        "https://youtube.com",
        "https://facebook.com",
        "https://twitter.com"};
    // Define bloom filter with 4 hash functions and 128 memory bits
    BloomFilter<4, 128> url_bloom_filter;
    std::for_each(urls.begin(), urls.end(), [&url_bloom_filter](auto& item) {
        // Insert items into bloom filter
        url_bloom_filter.insert(item);
    });

    // Check that bloom filter contains an item or not
    if (url_bloom_filter.contains("https://gmail.com")) {
        std::cout << "FOUND!!!!!" << std::endl;
    } else {
        std::cout << "NOT FOUND" << std::endl;
    }
    if (url_bloom_filter.contains("https://facebook.com")) {
        std::cout << "FOUND" << std::endl;
    } else {
        std::cout << "NOT FOUND!!!!!" << std::endl;
    }
}
```

## Membership - Counting Bloom Filter
Simple usage:
```c++
#include <membership/counting_bloom_filter.h>
#include <vector>
#include <string>

int main() {
    std::vector<std::string> urls{
        "https://google.com",
        "https://instagram.com",
        "https://youtube.com",
        "https://facebook.com",
        "https://twitter.com"};
    // Define bloom filter with 4 hash functions and 128 memory bits
    CountingBloomFilter<4, 128> url_bloom_filter;
    std::for_each(urls.begin(), urls.end(), [&url_bloom_filter](auto& item) {
        // Insert items into bloom filter
        url_bloom_filter.insert(item);
    });

    // Check that bloom filter contains an item or not
    if (url_bloom_filter.contains("https://gmail.com")) {
        std::cout << "FOUND!!!!!" << std::endl;
    } else {
        std::cout << "NOT FOUND" << std::endl;
    }
    
    if (url_bloom_filter.contains(urls[0])) {
        std::cout << "FOUND" << std::endl;
    } else {
        std::cout << "NOT FOUND!!!!!" << std::endl;
    }
    // Erase an item and check for existence
    url_bloom_filter.erase(urls[0]);
    if (url_bloom_filter.contains(urls[0])) {
        std::cout << "FOUND!!!!!" << std::endl;
    } else {
        std::cout << "NOT FOUND" << std::endl;
    }
}
```

## Calculators - Bloom Filter Calculator
This class helps to find near-optimal parameters for the number of hash functions and memory bits.

To find optimal parameters with false-positive probability of 1% and 1,000,000 expected items.
```c++
float fp_probability = 0.01f;
size_t expected_number_of_elements = 1000000;
size_t number_of_hash_functions, number_of_memory_bits;
BloomFilterCalculator::OptimalParams(expected_number_of_elements, fp_probability, number_of_hash_functions, number_of_memory_bits);
```
Or to find false-positive probability for the given parameters and expected number of items.
```c++
size_t expected_number_of_elements = 1000000;
size_t number_of_hash_functions = 6;
size_t number_of_memory_bits = 100000;

float fp_prob_computed = BloomFilterCalculator::FalsePositiveProbability(
        expected_number_of_elements,
        number_of_hash_functions,
        number_of_memory_bits);
```

# Use with Custom Classes
To use these data structures for custom classes, you should override Value method of the hash class.
e.g.:
```c++
#include <hash/mmh3_hash_factory.h>
#include <string>

class MyCustomClass {
   public:
    std::string first_name;
    std::string last_name;
};

template <>
uint32_t MMH3Hash<MyCustomClass>::Value(const MyCustomClass& input) const {
    MMH3Hash<std::string> string_hash(seed_);
    uint32_t output = string_hash.Value(input.first_name) ^ string_hash.Value(input.last_name);
    return output;
}

int main() {
    MyCustomClass c;
    // Define bloom filter with 4 hash functions (MMH3Hash) with 128 memory bits
    // and MMH3HashFactory class for hash function generator
    // and MyCustomClass type
    BloomFilter<4, 128, MMH3HashFactory, MyCustomClass> bloom_filter;
    bloom_filter.insert(c);
}
```
