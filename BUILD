cc_library(
  name = "base64",
  hdrs = [
      "base64.h",
  ],
  deps = [
    "@com_google_absl//absl/status",
    "@com_google_absl//absl/status:statusor",
    "@com_google_absl//absl/strings:str_format",
    ],
)

cc_binary(
  name = "base64_test",
  deps = [
    ":base64",
    "@com_google_absl//absl/status",
    "@com_google_googletest//:gtest_main",
    ],
  srcs = ["base64_test.cc"],
)
