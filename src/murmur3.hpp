#ifndef FARMBRAIN_MURMUR3_HPP
#define FARMBRAIN_MURMUR3_HPP
// The MIT License (MIT)
//
// Copyright (c) 2015, Tamás Szelei
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
#include <stdint.h>
#include "util.hpp"

namespace bp {

    constexpr uint32_t mm3_x86_32(blocked_string_view key, uint32_t seed) {
        uint32_t h1 = seed;

        const uint32_t c1 = 0xcc9e2d51;
        const uint32_t c2 = 0x1b873593;

        const int nblocks = key.size() / 4;
        for (int i = -nblocks; i; i++) {
            uint32_t k1 = key.get_block(i);

            k1 *= c1;
            k1 = (k1 << 15) | (k1 >> (32 - 15));
            k1 *= c2;

            h1 ^= k1;
            h1 = (h1 << 13) | (h1 >> (32 - 13));
            h1 = h1 * 5 + 0xe6546b64;
        }

        uint32_t k1 = 0;

        switch (key.size() & 3) {
            case 3:
                k1 ^= key.tail(2) << 16;
            case 2:
                k1 ^= key.tail(1) << 8;
            case 1:
                k1 ^= key.tail(0);
                k1 *= c1;
                k1 = (k1 << 15) | (k1 >> (32 - 15));
                k1 *= c2;
                h1 ^= k1;
        };

        h1 ^= key.size();

        h1 ^= h1 >> 16;
        h1 *= 0x85ebca6b;
        h1 ^= h1 >> 13;
        h1 *= 0xc2b2ae35;
        h1 ^= h1 >> 16;

        return h1;
    }
}
#endif //FARMBRAIN_MURMUR3_HPP
