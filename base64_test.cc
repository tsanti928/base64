#include "base64.h"

#include "absl/status/status.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::base64::encode;
using ::base64::decode;
using ::testing::Eq;

TEST (Base64, encode) {
    EXPECT_THAT(encode(""), Eq(""));
    EXPECT_THAT(encode("Man"), Eq("TWFu"));
    EXPECT_THAT(encode("Ma"), Eq("TWE="));
    EXPECT_THAT(encode("M"), Eq("TQ=="));
    EXPECT_THAT(encode("Manu"), Eq("TWFudQ=="));
    EXPECT_THAT(encode("Manuk"), Eq("TWFudWs="));
    
}

TEST (Base64, non_ascii_chars) {
    std::string bytes = {0x00,  0x00, static_cast<char>(0xe8)};
    EXPECT_THAT(encode(bytes), Eq("AADo"));
}

TEST (Base64, decode) {
    EXPECT_THAT(absl::IsInvalidArgument(decode("TWFum").status()), Eq(true));
    EXPECT_THAT(absl::IsInvalidArgument(decode("TWFum===").status()), Eq(true));
    EXPECT_THAT(absl::IsInvalidArgument(decode("TWF-").status()), Eq(true));
    EXPECT_THAT(*decode("TWFu"), Eq("Man"));
    EXPECT_THAT(*decode("TWE="), Eq("Ma"));
    EXPECT_THAT(*decode("TQ=="), Eq("M"));
    EXPECT_THAT(*decode("TWFudQ=="), Eq("Manu"));
    EXPECT_THAT(*decode("TWFudWs="), Eq("Manuk"));
}

TEST (Base64, stream_encode) {
    std::istringstream in("Manu");
    std::ostringstream out;
    auto status = encode(in, out, 6);
    EXPECT_THAT(out.str(), Eq("TWFudQ=="));
}

TEST (Base64, stream_decode) {
    std::istringstream in("TQ==");
    std::ostringstream out;
    auto status = decode(in, out, 8);
    EXPECT_THAT(out.str(), Eq("M"));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
