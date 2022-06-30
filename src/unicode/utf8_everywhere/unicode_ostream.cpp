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
#include <bitcoin/system/unicode/utf8_everywhere/unicode_ostream.hpp>

#include <cstddef>
#include <iostream>
#include <bitcoin/system/unicode/utf8_everywhere/unicode_streambuf.hpp>

namespace libbitcoin {
namespace system {

unicode_ostream::unicode_ostream(
#ifdef _MSC_VER
    std::ostream&, std::wostream& wide_stream,
    size_t wide_buffer_size) THROWS
#else
    std::ostream& narrow_stream, std::wostream&, size_t)
#endif
#ifdef _MSC_VER
  : std::ostream(new unicode_streambuf(wide_stream.rdbuf(), wide_buffer_size))
#else
  : std::ostream(narrow_stream.rdbuf())
#endif
{
}

unicode_ostream::~unicode_ostream() NOEXCEPT
{
#ifdef _MSC_VER
    delete rdbuf();
#endif
}

} // namespace system
} // namespace libbitcoin
