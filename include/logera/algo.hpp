#pragma once

#include <memory>
#include <type_traits>
#include <vector>

namespace algo
{

/**
 * @brief  Find duplicate elements in a sorted range in ascending order.
 * @param  pred  Binary predicate that returns true if the elements are equal.
 * @return Vector of pointers to elements that appear more than once, including
 * the first ones.
 */
template <typename T, typename Pred>
auto duplicates(T first, T last, Pred&& pred) -> std::vector<T>;

/**
 * @brief Find duplicate elements in a sorted range in ascending order.
 * @param  pred  Binary predicate that returns true if the elements are equal.
 * @return Vector of copied elements that appear more than once, including the
 * first ones.
 */
template <typename T, typename Pred>
auto duplicates_v(T first, T last, Pred&& pred) -> std::vector<typename T::value_type>;

/**
 * @brief  Find excess duplicate elements in a sorted range in ascending order.
 * @param  pred  Binary predicate that returns true if the elements are equal.
 * @return Vector of pointers to elements that appear more than once, excluding
 * the first ones.
 */
template <typename T, typename Pred>
auto excess_duplicates(T first, T last, Pred&& pred) -> std::vector<T>;

/**
 * @brief  Find excess duplicate elements in a sorted range in ascending order.
 * @param  pred  Binary predicate that returns true if the elements are equal.
 * @return Vector of copied elements that appear more than once, excluding the
 * first ones.
 */
template <typename T, typename Pred>
auto excess_duplicates_v(T first, T last, Pred&& pred) -> std::vector<typename T::value_type>;

/*
    This function shuffles the elements of the range [data_begin, data_end)
    according to the indices in the range [indices_begin, indices_end).

    Because it is intended to be used in indirect sorting, where the indices
    are often calculated once and then reused to sort many ranges, the indices
    are stored in a buffer and reused if provided.
*/
template <typename T, typename U>
    requires(std::is_integral_v<typename U::value_type>)
auto indirect_rearrange(T data_begin, T data_end, U indices_begin, std::unique_ptr<typename U::value_type[]> buf);

} // namespace algo

#include "algo-inl.hpp"
