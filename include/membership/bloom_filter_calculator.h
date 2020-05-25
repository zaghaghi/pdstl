#ifndef INCLUDE_MEMBERSHIP_BLOOM_FILTER_CALCULATOR_H_
#define INCLUDE_MEMBERSHIP_BLOOM_FILTER_CALCULATOR_H_

#include <cmath>

/*
 * Bloom Filter Calculator
 *
 *  BloomFilterCalculator calculates near-optimal number of hash functions, length of memory bits and false positive probability.
 */
class BloomFilterCalculator {
   public:
    /* OptimalParams
    * @expected_number_of_elements Expected number of elements will be inserted into bloom filter.
    * @false_positive_probability Desiered false-positive probability
    * @number_of_hash_functions [Output] near-optimal number of hash functions.
    * @number_of_memory_bits [Output] near-optimal number of memory bits.
    */
    static void OptimalParams(
        size_t expected_number_of_elements, float false_positive_probability,
        size_t& number_of_hash_functions, size_t& number_of_memory_bits) {
        number_of_hash_functions = -std::ceil(std::log(false_positive_probability) / std::log(2));
        number_of_memory_bits = -(expected_number_of_elements * std::log(false_positive_probability) / (log(2) * log(2)));
    }

    static float FalsePositiveProbability(
        size_t expected_number_of_elements,
        size_t number_of_hash_functions,
        size_t number_of_memory_bits) {
        return std::pow(
            1 - std::exp(
                    -float(number_of_hash_functions * expected_number_of_elements / number_of_memory_bits)),
            number_of_hash_functions);
    }
};

#endif   // INCLUDE_MEMBERSHIP_BLOOM_FILTER_CALCULATOR_H_
