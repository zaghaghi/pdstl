#include <hash/mmh3_hash_factory.h>
#include <membership/bloom_filer.h>

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
uint32_t MMH3Hash<MyCustomClass>::Value(const MyCustomClass& input) const {
    MMH3Hash<std::string> string_hash(seed_);
    uint32_t output = string_hash.Value(input.first_name) ^ string_hash.Value(input.last_name);
    return output;
}

int main(int /* argc */, char** /*argv*/) {
    MyCustomClass c;
    BloomFilter<4, 64, MMH3HashFactory, MyCustomClass> bloom_filter;
    bloom_filter.insert(c);
    if (bloom_filter.contains(c)) {
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
    BloomFilter<4, 64> url_bloom_filter;
    std::for_each(urls.begin(), urls.end(), [&url_bloom_filter](auto& item) {
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
    // bloom_filter.erase(c);
}
