#ifndef BASE_64_H
#define BASE_64_H

#include <math.h>
#include <unordered_map>
#include <string>
#include <string_view>

#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/str_format.h"

namespace base64 {

    // Translates 6 bits of src data to 8 bits
    // of encoded ASCII dest data.
    constexpr char src_to_dest[] = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

    // Translates 8 bits of encoded ASCII dest data
    // to 6 bits of src data.
    inline const std::unordered_map<char, char> dest_to_src {
        {'A', 0}, {'B', 1}, {'C', 2}, {'D', 3}, {'E', 4},
        {'F', 5}, {'G', 6}, {'H', 7}, {'I', 8}, {'J', 9},
        {'K', 10}, {'L', 11}, {'M', 12}, {'N', 13},
        {'O', 14}, {'P', 15}, {'Q', 16}, {'R', 17},
        {'S', 18}, {'T', 19}, {'U', 20}, {'V', 21},
        {'W', 22}, {'X', 23}, {'Y', 24}, {'Z', 25},
        {'a', 26}, {'b', 27}, {'c', 28}, {'d', 29},
        {'e', 30}, {'f', 31}, {'g', 32}, {'h', 33},
        {'i', 34}, {'j', 35}, {'k', 36}, {'l', 37},
        {'m', 38}, {'n', 39}, {'o', 40}, {'p', 41},
        {'q', 42}, {'r', 43}, {'s', 44}, {'t', 45},
        {'u', 46}, {'v', 47}, {'w', 48}, {'x', 49},
        {'y', 50}, {'z', 51}, {'0', 52}, {'1', 53},
        {'2', 54}, {'3', 55}, {'4', 56}, {'5', 57},
        {'6', 58}, {'7', 59}, {'8', 60}, {'9', 61},
        {'+', 62}, {'/', 63}
    };

    constexpr int32_t bits_per_src_digit = 8;
    constexpr int32_t src_digits_per_seq = 3;
    constexpr int32_t bits_per_dest_digit = 6;
    constexpr int32_t dest_digits_per_seq = 4;
    constexpr int32_t dest_mask = 0b111111;
    constexpr int32_t src_mask = 0xff;
    constexpr int32_t max_padding = 2;

    inline std::string encode(std::string_view s) {
        // 'view' may be smaller than src_digits_per_seq when input is not
        // a multiple of it.
        auto encode_upto_three_octets = [](std::string_view view, char* dest){
            int32_t bits{0};
            for(int i = 0 ; i < view.size(); i++){
                int32_t shift = bits_per_src_digit * (src_digits_per_seq-1-i);
                bits |= (view[i] & src_mask) << shift;
            }

            // Non-padded digits.
            int32_t content_digits = dest_digits_per_seq - (src_digits_per_seq - view.size());
            for(int i = 0; i < content_digits; i++){
                int32_t shift = bits_per_dest_digit * (dest_digits_per_seq-1-i);
                dest[i] = src_to_dest[(bits >> shift) & dest_mask];
            }
        };

        // Can allocate the encoded length ahead of time.
        const int32_t src_sz = s.size();
        const int32_t dest_sz = ceil(static_cast<double>(src_sz)/src_digits_per_seq) * dest_digits_per_seq;

        std::string encoded(dest_sz, '=');
        for(int i = 0, j = 0; i < src_sz; i += src_digits_per_seq, j += dest_digits_per_seq){
            int32_t end = std::min(i+src_digits_per_seq, src_sz);
            std::string_view view(&s[i], end-i);
            encode_upto_three_octets(view, &encoded[j]);
        }

        return encoded;
    }

    inline absl::StatusOr<std::string> decode(std::string_view s) {
        const int dest_sz = s.size();

        if((dest_sz % dest_digits_per_seq) != 0)
            return absl::InvalidArgumentError("input length must be a multiple of 4");
        
        int num_padding{max_padding+1};
        for(int i = 0; i < max_padding+1; i++){
            if(s[dest_sz-1-i] != '='){
                num_padding = i;
                break;
            }
        }
        if(num_padding > max_padding)
            return absl::InvalidArgumentError("padding is larger than 2 digits");

        // 'view' may be smaller than dest_digits_per_seq when input
        // has padding.
        auto decode_upto_four_sextets = [](std::string_view view, char* dest) -> absl::Status {
            int32_t bits{0};
            for(int i = 0; i < view.size(); i++){
                int32_t shift = bits_per_dest_digit * (dest_digits_per_seq-1-i);
                auto hit = dest_to_src.find(view[i]);
                if(hit == dest_to_src.end())
                    return absl::InvalidArgumentError(absl::StrFormat("illegal character %c", view[i]));
                bits |= (hit->second & dest_mask) << shift;
            }

            // Non-padded digits.
            int32_t content_digits = src_digits_per_seq - (dest_digits_per_seq - view.size());
            for(int i = 0; i < content_digits; i++){
                int32_t shift = bits_per_src_digit * (src_digits_per_seq-1-i);
                dest[i] = (bits >> shift) & src_mask;
            }

            return absl::OkStatus();
        };

        // Can allocate the decoded length ahead of time.
        const int src_sz = dest_sz/dest_digits_per_seq * src_digits_per_seq - num_padding;
        std::string decoded(src_sz, ' ');
        const int dest_sz_effective = dest_sz-num_padding;

        for(int i = 0, j = 0; i < dest_sz_effective; i += dest_digits_per_seq, j += src_digits_per_seq){
            int32_t end = std::min(i+dest_digits_per_seq, dest_sz_effective);
            std::string_view view(&s[i], end-i);
            auto status = decode_upto_four_sextets(view, &decoded[j]);
            if(!status.ok())
                return status;
        }

        return decoded;
    }

} // namespace base64

#endif // BASE_64_H