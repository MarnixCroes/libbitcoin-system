/**
 * Copyright (c) 2011-2022 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBBITCOIN_SYSTEM_CONSTANTS_HPP
#define LIBBITCOIN_SYSTEM_CONSTANTS_HPP

#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <bitcoin/system/define.hpp>
#include <bitcoin/system/integrals.hpp>

namespace libbitcoin {

/// Avoid typed casts due to circular header inclusion.
/// All values converted and specified domain and reconverted to domain.
/// This isolates integral promotion, and caller controls operating domain.

template <typename Type>
constexpr bool is_zero(Type value) noexcept
{
    BC_PUSH_WARNING(NO_CASTS_FOR_ARITHMETIC_CONVERSION)
    return value == static_cast<Type>(zero);
    BC_POP_WARNING()
}

template <typename Type>
constexpr bool is_nonzero(Type value) noexcept
{
    return !is_zero(value);
}

template <typename Type>
constexpr bool is_one(Type value) noexcept
{
    BC_PUSH_WARNING(NO_CASTS_FOR_ARITHMETIC_CONVERSION)
    return value == static_cast<Type>(1);
    BC_POP_WARNING()
}

template <typename Type>
constexpr Type lo_bit(Type value) noexcept
{
    BC_PUSH_WARNING(NO_CASTS_FOR_ARITHMETIC_CONVERSION)
    return static_cast<Type>(value % 2);
    BC_POP_WARNING()
}

template <typename Type>
constexpr bool is_even(Type value) noexcept
{
    return is_zero(lo_bit(value));
}

template <typename Type>
constexpr bool is_odd(Type value) noexcept
{
    return !is_even(value);
}

template <typename Type>
constexpr bool is_null(Type value) noexcept
{
    return value == nullptr;
}

template <typename Type>
constexpr Type to_bits(Type bytes) noexcept
{
    BC_PUSH_WARNING(NO_CASTS_FOR_ARITHMETIC_CONVERSION)
    return static_cast<Type>(bytes * 8);
    BC_POP_WARNING()
}

constexpr uint8_t to_byte(char character) noexcept
{
    BC_PUSH_WARNING(NO_CASTS_FOR_ARITHMETIC_CONVERSION)
    return static_cast<uint8_t>(character);
    BC_POP_WARNING()
}

// floored
template <typename Type>
constexpr Type to_half(Type value) noexcept
{
    BC_PUSH_WARNING(NO_CASTS_FOR_ARITHMETIC_CONVERSION)
    return static_cast<Type>(value / 2);
    BC_POP_WARNING()
}

template <typename Type = int>
constexpr Type to_int(bool value) noexcept
{
    BC_PUSH_WARNING(NO_CASTS_FOR_ARITHMETIC_CONVERSION)
    return static_cast<Type>(value ? 1 : 0);
    BC_POP_WARNING()
}

template <typename Type>
constexpr bool to_bool(Type value) noexcept
{
    return is_nonzero(value);
}

template <typename Type>
constexpr Type add1(Type value) noexcept
{
    BC_PUSH_WARNING(NO_CASTS_FOR_ARITHMETIC_CONVERSION)
    return static_cast<Type>(value + 1);
    BC_POP_WARNING()
}

template <typename Type>
constexpr Type sub1(Type value) noexcept
{
    BC_PUSH_WARNING(NO_CASTS_FOR_ARITHMETIC_CONVERSION)
    return static_cast<Type>(value - 1);
    BC_POP_WARNING()
}

template <typename Type>
constexpr size_t width() noexcept
{
    // This is not always a logical size for non-integral types.
    // see comments in is_integral_size for expected integral sizes.
    return to_bits(sizeof(Type));
}

template <typename Type>
constexpr size_t width(Type value) noexcept
{
    // This is not always a logical size for non-integral types.
    // see comments in is_integral_size for expected integral sizes.
    return to_bits(sizeof(value));
}

/// Determine the bitcoin variable-serialized size of a given value.
constexpr size_t variable_size(uint64_t value) noexcept
{
    if (value < varint_two_bytes)
        return sizeof(uint8_t);

    if (value <= max_uint16)
        return sizeof(uint8_t) + sizeof(uint16_t);

    if (value <= max_uint32)
        return sizeof(uint8_t) + sizeof(uint32_t);

    return sizeof(uint8_t) + sizeof(uint64_t);
}

template <typename Left, typename Right>
constexpr bool is_same() noexcept
{
    // implies same size and signedness, independent of const and volatility.
    return std::is_same_v<Left, Right>;
}

template <typename Left, typename Right>
constexpr bool is_same_size() noexcept
{
    return sizeof(Left) == sizeof(Right);
}

template <typename Type>
constexpr bool is_signed() noexcept
{
    // bool is unsigned: bool(-1) < bool(0). w/char sign unspecified.
    // w/charxx_t types are unsigned. iostream relies on w/char.
    return std::is_signed_v<Type>;
}

template <typename Type>
constexpr bool is_integer() noexcept
{
    // numeric_limits may be specialized by non-integrals (such as uintx).
    return std::numeric_limits<Type>::is_integer && !is_same<Type, bool>();
}

constexpr bool is_integral_size(size_t bytes) noexcept
{
    // non-numbered integrals have unreliable sizes (as do least/fast types).
    // s/u char are one byte. bool and wchar_t are of unspecified size, as are
    // int, long, and long long. only minimum and relative sizes are assured.
    return bytes == sizeof(uint8_t)
        || bytes == sizeof(uint16_t)
        || bytes == sizeof(uint32_t)
        || bytes == sizeof(uint64_t);
}

// This is future-proofing against larger integrals or language features that
// promote 3, 5, 6, 7 byte-sized types to integral (see std::is_integral).
template <typename Type>
constexpr bool is_integral_size() noexcept
{
    return is_integral_size(sizeof(Type));
}

template <typename Type>
constexpr bool is_integral() noexcept
{
    // bool is integral, but excluded here.
    return std::is_integral_v<Type>
        && is_integral_size<Type>()
        && !is_same<Type, bool>();
}

// This is future-proofing against larger integrals or language features that
// promote 3, 5, 6, 7 byte-sized types to integral (see std::is_integral).
constexpr bool is_bytes_width(size_t bits) noexcept
{
    return !is_zero(bits) && is_zero(bits % byte_bits);
}

} // namespace libbitcoin

#endif
