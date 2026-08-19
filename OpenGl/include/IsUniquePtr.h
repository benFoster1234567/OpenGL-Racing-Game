#pragma once
#include <iostream>
#include <memory>
#include <type_traits>

template <typename T>
struct is_unique_ptr_struct : std::false_type {};

template <typename T, typename D>
struct is_unique_ptr_struct<std::unique_ptr<T, D>> : std::true_type {};

template <typename T>
inline constexpr bool is_unique_ptr_v = is_unique_ptr_struct<std::remove_cvref_t<T>>::value;

template <typename T>
concept IsUniquePtr = is_unique_ptr_v<T>;