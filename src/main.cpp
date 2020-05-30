#include <hash/mmh3_hash_factory.h>
#include <membership/bloom_filter.h>
#include <membership/bloom_filter_calculator.h>
#include <membership/counting_bloom_filter.h>
// #include <membership/quotient_filter.h>
#include <table/quotient_table.h>

#include <algorithm>
#include <iostream>
#include <string>
class MyCustomClass {
   public:
    std::string first_name;
    std::string last_name;
};

std::ostream& operator<<(std::ostream& out, const MyCustomClass& c) {
    out << c.first_name << ", " << c.last_name;
    return out;
}

template <>
uint32_t pdstl::mmh3_hash<MyCustomClass>::value(const MyCustomClass& input) const {
    pdstl::mmh3_hash<std::string> string_hash(seed_);
    uint32_t output = string_hash.value(input.first_name) ^ string_hash.value(input.last_name);
    return output;
}

int main(int /* argc */, char** /*argv*/) {
    MyCustomClass c;
    pdstl::bloom_filter<4, 64, pdstl::mmh3_hash_factory, MyCustomClass> a_bloom_filter;
    a_bloom_filter.insert(c);
    if (a_bloom_filter.contains(c)) {
        std::cout << "Found!" << std::endl;
    } else {
        std::cout << "Not Found!" << std::endl;
    }

    std::vector<std::string> urls{
        "https://google.com",
        "https://instagram.com",
        "https://youtube.com",
        "https://facebook.com",
        "https://twitter.com"};
    pdstl::bloom_filter<4, 64> url_bloom_filter;
    std::for_each(urls.begin(), urls.end(), [&url_bloom_filter](const std::string& item) {
        url_bloom_filter.insert(item);
    });

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
    float fp_probability = 0.01f;
    size_t expected_number_of_elements = 1000000;
    size_t number_of_hash_functions, number_of_memory_bits;
    pdstl::bloom_filter_calculator::optimal_params(expected_number_of_elements, fp_probability, number_of_hash_functions, number_of_memory_bits);
    std::cout << "Optimal params for " << expected_number_of_elements
              << " elements with false-positive probability of " << fp_probability
              << std::endl;
    std::cout << "\tnumber of hash functions: " << number_of_hash_functions << std::endl;
    std::cout << "\tnumber of memory bits: " << number_of_memory_bits << std::endl;

    float fp_prob_computed = pdstl::bloom_filter_calculator::false_positive_probability(
        expected_number_of_elements,
        number_of_hash_functions,
        number_of_memory_bits);
    std::cout << "False-positive porbability of bloom filter with "
              << number_of_hash_functions << " hash functions and "
              << number_of_memory_bits << " memory bits for inserting "
              << expected_number_of_elements << " elements is: "
              << fp_prob_computed << std::endl;

    pdstl::counting_bloom_filter<4, 64> a_counting_bloom_filter;
    std::for_each(urls.begin(), urls.end(), [&a_counting_bloom_filter](const std::string& item) {
        a_counting_bloom_filter.insert(item);
    });

    if (a_counting_bloom_filter.contains("https://gmail.com")) {
        std::cout << "FOUND!!!!!" << std::endl;
    } else {
        std::cout << "NOT FOUND" << std::endl;
    }
    if (a_counting_bloom_filter.contains(urls[0])) {
        std::cout << "FOUND" << std::endl;
    } else {
        std::cout << "NOT FOUND!!!!!" << std::endl;
    }
    a_counting_bloom_filter.erase(urls[0]);
    if (a_counting_bloom_filter.contains(urls[0])) {
        std::cout << "FOUND!!!!!" << std::endl;
    } else {
        std::cout << "NOT FOUND" << std::endl;
    }

    // pdstl::quotient_filter<16, 4> a_quotient_filter;
    // std::for_each(urls.begin(), urls.end(), [&a_quotient_filter](const std::string& item) {
    //     a_quotient_filter.insert(item);
    // });
    // if (a_quotient_filter.contains("https://gmail.com")) {
    //     std::cout << "FOUND!!!!!" << std::endl;
    // } else {
    //     std::cout << "NOT FOUND" << std::endl;
    // }
    // if (a_quotient_filter.contains(urls[0])) {
    //     std::cout << "FOUND" << std::endl;
    // } else {
    //     std::cout << "NOT FOUND!!!!!" << std::endl;
    // }

    pdstl::quotient_table<pdstl::quotient_bucket<uint16_t, 12>> table(8);
    std::cout << sizeof(table) << std::endl;
    table.insert(0, 10);
    // table.print_internals();
    table.insert(0, 11);
    // table.print_internals();
    table.insert(1, 12);
    // table.print_internals();
    table.insert(1, 13);
    // table.print_internals();
    table.insert(7, 14);
    // table.print_internals();
    table.insert(7, 15);
    // table.print_internals();
    table.insert(0, 9);
    // table.print_internals();

    pdstl::quotient_table<pdstl::quotient_bucket<uint16_t, 12>> table2(8);
    table2.insert(0, 12);
    // table2.print_internals();
    table2.insert(0, 10);
    // table2.print_internals();
    table2.insert(0, 8);
    // table2.print_internals();
    table2.insert(0, 9);
    // table2.print_internals();
    table2.insert(0, 11);
    // table2.print_internals();
    table2.insert(6, 20);
    // table2.print_internals();
    table2.insert(6, 18);
    // table2.print_internals();
    table2.insert(6, 19);
    // table2.print_internals();
    table2.contains(6, 19);
    table2.contains(6, 20);
    table2.clear();
    // std::cout << table.contains(0, 11) << std::endl;
    // std::cout << table.contains(0, 12) << std::endl;

    pdstl::quotient_table<pdstl::quotient_counting_bucket<uint16_t, uint16_t, 12>> counting_table(8);
}
