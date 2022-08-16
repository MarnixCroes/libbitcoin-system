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
#ifndef LIBBITCOIN_SYSTEM_INTRINSICS_XCPU_FUNCTIONAL256_HPP
#define LIBBITCOIN_SYSTEM_INTRINSICS_XCPU_FUNCTIONAL256_HPP

#include <bitcoin/system/define.hpp>
#include <bitcoin/system/intrinsics/xcpu/defines.hpp>

namespace libbitcoin {
namespace system {

#if defined(HAVE_AVX2)

using xint256_t = __m256i;

// AVX2 primitives (for all element widths).
// ----------------------------------------------------------------------------
// Bitwise functions are idependent of element widths.

template <typename Word, if_same<Word, xint256_t> = true>
INLINE Word and_(Word a, Word b) NOEXCEPT
{
    return _mm256_and_si256(a, b);
}

template <typename Word, if_same<Word, xint256_t> = true>
INLINE Word or_(Word a, Word b) NOEXCEPT
{
    return _mm256_or_si256(a, b);
}

template <typename Word, if_same<Word, xint256_t> = true>
INLINE Word xor_(Word a, Word b) NOEXCEPT
{
    return _mm256_xor_si256(a, b);
}

// AVX2 primitives (for 32 bit elements).
// ----------------------------------------------------------------------------
// TODO: 64 bit elements.

template <auto B, typename Word, if_same<Word, xint256_t> = true>
INLINE Word shr_(Word a) NOEXCEPT
{
    return _mm256_srli_epi32(a, B);
}

template <auto B, typename Word, if_same<Word, xint256_t> = true>
INLINE Word shl_(Word a) NOEXCEPT
{
    return _mm256_slli_epi32(a, B);
}

template <auto B, auto S, typename Word, if_same<Word, xint256_t> = true>
INLINE Word ror_(Word a) NOEXCEPT
{
    // TODO: S will become unnecessary as the function set must be 32/64.
    return or_(shr_<B>(a), shl_<S - B>(a));
}

template <auto B, auto S, typename Word, if_same<Word, xint256_t> = true>
INLINE Word rol_(Word a) NOEXCEPT
{
    // TODO: S will become unnecessary as the function set must be 32/64.
    return or_(shl_<B>(a), shr_<S - B>(a));
}

template <typename Word, if_same<Word, xint256_t> = true>
INLINE Word add_(Word a, Word b) NOEXCEPT
{
    return _mm256_add_epi32(a, b);
}

template <auto K, typename Word, if_same<Word, xint256_t> = true>
INLINE Word add_(Word a) NOEXCEPT
{
    // Broadcast 32-bit integer to all elements.
    return add_(a, _mm256_set1_epi32(K));
}

// AVX2 set/get (for all element widths).
// ----------------------------------------------------------------------------

// Lane zero is lowest order word.
template <typename To, auto Lane, if_integral_integer<To> = true>
INLINE To get(xint256_t a) NOEXCEPT
{
    if constexpr      (is_same_type<To, uint8_t>)
    {
        return _mm256_extract_epi8(a, Lane);
    }
    else if constexpr (is_same_type<To, uint16_t>)
    {
        return _mm256_extract_epi16(a, Lane);
    }
    else if constexpr (is_same_type<To, uint32_t>)
    {
        return _mm256_extract_epi32(a, Lane);
    }
    else if constexpr (is_same_type<To, uint64_t>)
    {
        return _mm256_extract_epi64(a, Lane);
    }
}

// Low order word to the left.
template <typename To, if_same<To, xint256_t> = true>
INLINE To set(
    uint64_t x01 = 0, uint64_t x02 = 0,
    uint64_t x03 = 0, uint64_t x04 = 0) NOEXCEPT
{
    // Low order word to the right.
    return _mm256_set_epi64x(
        x04, x03, x02, x01);
}

template <typename To, if_same<To, xint256_t> = true>
INLINE To set(
    uint32_t x01 = 0, uint32_t x02 = 0,
    uint32_t x03 = 0, uint32_t x04 = 0,
    uint32_t x05 = 0, uint32_t x06 = 0,
    uint32_t x07 = 0, uint32_t x08 = 0) NOEXCEPT
{
    return _mm256_set_epi32(
        x08, x07, x06, x05, x04, x03, x02, x01);
}

template <typename To, if_same<To, xint256_t> = true>
INLINE To set(
    uint16_t x01 = 0, uint16_t x02 = 0,
    uint16_t x03 = 0, uint16_t x04 = 0,
    uint16_t x05 = 0, uint16_t x06 = 0,
    uint16_t x07 = 0, uint16_t x08 = 0,
    uint16_t x09 = 0, uint16_t x10 = 0,
    uint16_t x11 = 0, uint16_t x12 = 0,
    uint16_t x13 = 0, uint16_t x14 = 0,
    uint16_t x15 = 0, uint16_t x16 = 0) NOEXCEPT
{
    return _mm256_set_epi16(
        x16, x15, x14, x13, x12, x11, x10, x09,
        x08, x07, x06, x05, x04, x03, x02, x01);
}

template <typename To, if_same<To, xint256_t> = true>
INLINE To set(
    uint8_t x01 = 0, uint8_t x02 = 0,
    uint8_t x03 = 0, uint8_t x04 = 0,
    uint8_t x05 = 0, uint8_t x06 = 0,
    uint8_t x07 = 0, uint8_t x08 = 0,
    uint8_t x09 = 0, uint8_t x10 = 0,
    uint8_t x11 = 0, uint8_t x12 = 0,
    uint8_t x13 = 0, uint8_t x14 = 0,
    uint8_t x15 = 0, uint8_t x16 = 0,
    uint8_t x17 = 0, uint8_t x18 = 0,
    uint8_t x19 = 0, uint8_t x20 = 0,
    uint8_t x21 = 0, uint8_t x22 = 0,
    uint8_t x23 = 0, uint8_t x24 = 0,
    uint8_t x25 = 0, uint8_t x26 = 0,
    uint8_t x27 = 0, uint8_t x28 = 0,
    uint8_t x29 = 0, uint8_t x30 = 0,
    uint8_t x31 = 0, uint8_t x32 = 0) NOEXCEPT
{
    return _mm256_set_epi8(
        x32, x31, x30, x29, x28, x27, x26, x25,
        x24, x23, x22, x21, x20, x19, x18, x17,
        x16, x15, x14, x13, x12, x11, x10, x09,
        x08, x07, x06, x05, x04, x03, x02, x01);
}


BC_PUSH_WARNING(NO_ARRAY_INDEXING)
INLINE xint256_t byteswap(xint256_t value) NOEXCEPT
{
    constexpr std_array<uint32_t, 4> mask32
    {
        0x0c0d0e0f_u32, 0x08090a0b_u32, 0x04050607_u32, 0x00010203_u32
    };

    static const auto mask = set<xint256_t>(
        mask32[0], mask32[1], mask32[2], mask32[3],
        mask32[0], mask32[1], mask32[2], mask32[3]);

    return _mm256_shuffle_epi8(value, mask);
}
BC_POP_WARNING()

#else

// Symbol is defined but not usable.
struct xint256_t {};

#endif // HAVE_AVX2

} // namespace system
} // namespace libbitcoin

#endif

////INLINE xint256_t align(const bytes256& word) NOEXCEPT
////{
////    return _mm256_loadu_epi32(pointer_cast<const xint256_t>(word.data()));
////}
////
////INLINE bytes256 unalign(xint256_t value) NOEXCEPT
////{
////    bytes256 word{};
////    _mm256_storeu_epi32(pointer_cast<xint256_t>(word.data()), value);
////    return word;
////    ////return *pointer_cast<bytes256>(&value);
////}
////
////INLINE xint256_t native_to_big_endian(xint256_t value) NOEXCEPT
////{
////    return *pointer_cast<xint256_t>(unalign(byteswap(value)).data());
////}
////
////INLINE xint256_t native_from_big_endian(xint256_t value) NOEXCEPT
////{
////    return byteswap(align(*pointer_cast<bytes256>(&value)));
////}
