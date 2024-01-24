#pragma once

#include "opaque_vector.h"

#include <stdbool.h>

/**
 * @brief Advance the pointer by a given number of strides.
 *
 * @param p         Pointer to advance.
 * @param n         Number of strides to advance. Negative values are allowed.
 * @param stride    Size of each stride (element).
 * @return          Pointer that has been advanced.
 */
void* advance(void const* p, int n, int stride);

/**
 * @brief Check if the target is in the range [first, last).
 *
 * @param first     Pointer to the beginning of the range.
 * @param last      Pointer to the end of the range.
 * @param target    Pointer to the target element.
 * @return          True if the target is in the range, false otherwise.
 */
bool between(void const* first, void const* last, void const* target);

/**
 * @brief Calculate the number of strides between two pointers. May be negative.
 *
 * @param first     First pointer.
 * @param last      Second pointer.
 * @param stride    Size of each stride (element).
 * @return          The number of strides between the pointers.
 */
int distance(void const* first, void const* last, int stride);

/**
 * @brief  Find duplicate elements in a sorted range in ascending order.
 * @return Vector of void* containing elements that appear more than once, including the first ones.
 */
opaque_vector duplicates(void const* first, void const* last, int stride, int (*cmp)(void const*, void const*));

/**
 * @brief  Find excess duplicate elements in a sorted range in ascending order.
 * @return Vector of void* containing elements that appear more than once, excluding the first ones.
 */
opaque_vector excess_duplicates(void const* first, void const* last, int stride, int (*cmp)(void const*, void const*));

/**
 * @brief  Find the first element in the range [first, last) that satisfies the given comparison function.
 * @return Pointer to the first element that satisfies the comparison function, or last if not found.
 */
void* find_cmp(void const* first, void const* last, int stride, void const* value, int (*cmp)(void const*, void const*));

/**
 * @brief Find the first element in the range [first, last) that does not satisfy the given predicate function.
 */
void* find_if_not(void const* first, void const* last, int stride, bool (*pred)(void const*));

/**
 * @brief Find the first element in the range [first, last) that satisfies the given predicate function.
 */
void* find_if(void const* first, void const* last, int stride, bool (*pred)(void const*));

/**
 * @brief Find the first element in the range [first, last) that does not satisfy the given comparison function.
 */
void* find_not_cmp(void const* first, void const* last, int stride, void const* value, int (*cmp)(void const*, void const*));

/**
 * @brief Find the first element in the range [first, last) that is not equal to the given value.
 */
void* find_not_val(void const* first, void const* last, void const* value, int stride);

/**
 * @brief Find the first element in the range [first, last) that is equal to the given value.
 */
void* find_val(void const* first, void const* last, void const* value, int stride);

void reverse(void* first, void* last, int stride);

void swap(void* lhs, void* rhs, int stride);

/**
 * @brief Get a vector of unique elements from a sorted range in ascending order.
 * @return          Vector of unique elements.
 */
opaque_vector unique(void const* first, void const* last, int stride, int (*cmp)(void const*, void const*));

/**
 * @brief Get first element in the range [first, last) that is not greater than value.
 * @param cmp Comparison function: negative if lhs < rhs, zero if lhs == rhs, positive if lhs > rhs.
 * @return Pointer to the first element that is not greater than value, or last if not found.
 * @note   The range must be sorted as if by qsort using cmp
 */
void* upper_bound(void const* first, void const* last, int stride, void const* value, int (*cmp)(void const*, void const*));

/**
 * @brief Get first element in the range [first, last) that is not less than value.
 * @param cmp Comparison function: negative if lhs < rhs, zero if lhs == rhs, positive if lhs > rhs.
 * @return Pointer to the first element that is not less than value, or last if not found.
 * @note   The range must be sorted as if by qsort using cmp
 */
void* lower_bound(void const* first, void const* last, int stride, void const* value, int (*cmp)(void const*, void const*));
