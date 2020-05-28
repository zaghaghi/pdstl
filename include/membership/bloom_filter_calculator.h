#ifndef INCLUDE_MEMBERSHIP_BLOOM_FILTER_CALCULATOR_H_
#define INCLUDE_MEMBERSHIP_BLOOM_FILTER_CALCULATOR_H_

#include <cmath>

namespace pdstl {

/*! \brief Bloom Filter Calculator
 *
 *  BloomFilterCalculator calculates near-optimal number of hash functions, length of memory bits and false positive probability.
 */
class bloom_filter_calculator {
   public:
    /*! \brief compute near optimal parameters for bloom filter
     * \param expected_number_of_elements -  [in] Expected number of elements will be inserted into bloom filter.
     * \param false_positive_probability - [in] Desiered false-positive probability
     * \param number_of_hash_functions - [out] near-optimal number of hash functions.
     * \param number_of_memory_bits - [out] near-optimal number of memory bits.
     */
    static void optimal_params(
        size_t expected_number_of_elements, float false_positive_probability,
        size_t& number_of_hash_functions, size_t& number_of_memory_bits) {
        number_of_hash_functions = -std::ceil(std::log(false_positive_probability) / std::log(2));
        number_of_memory_bits = -(expected_number_of_elements * std::log(false_positive_probability) / (log(2) * log(2)));
    }

    /*! \brief compute false-positive porbability of bloom filter
     *
     * \param expected_number_of_elements -  [in] Expected number of elements will be inserted into bloom filter.
     * \param number_of_hash_functions -  [in] number of hash functions.
     * \param number_of_memory_bits - [in] number of memory bits.
     * 
     * \return false-positive porbability of bloom filter
     */
    static float false_positive_probability(
        size_t expected_number_of_elements,
        size_t number_of_hash_functions,
        size_t number_of_memory_bits) {
        return std::pow(
            1 - std::exp(
                    -float(number_of_hash_functions * expected_number_of_elements / number_of_memory_bits)),
            number_of_hash_functions);
    }
};

}   // namespace pdstl

#endif   // INCLUDE_MEMBERSHIP_BLOOM_FILTER_CALCULATOR_H_
