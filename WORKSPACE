load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "bazel_skylib",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/bazel-skylib/releases/download/1.2.1/bazel-skylib-1.2.1.tar.gz",
        "https://github.com/bazelbuild/bazel-skylib/releases/download/1.2.1/bazel-skylib-1.2.1.tar.gz",
    ],
    sha256 = "f7be3474d42aae265405a592bb7da8e171919d74c16f082a5457840f06054728",
)
load("@bazel_skylib//:workspace.bzl", "bazel_skylib_workspace")
bazel_skylib_workspace()


http_archive(
  name = "rules_cc",
  urls = ["https://github.com/bazelbuild/rules_cc/archive/2f8c04c04462ab83c545ab14c0da68c3b4c96191.zip"],
  strip_prefix = "rules_cc-2f8c04c04462ab83c545ab14c0da68c3b4c96191",
)

http_archive(
  name = "com_google_absl",
  urls = ["https://github.com/abseil/abseil-cpp/archive/e517aaf499f88383000d4ddf6b84417fbbb48791.zip"],
  strip_prefix = "abseil-cpp-e517aaf499f88383000d4ddf6b84417fbbb48791",
)

http_archive(
  name = "com_google_googletest",
  urls = ["https://github.com/google/googletest/archive/bea621c3c39d8a7f71f07bd543c3a58bfa684f92.zip"],
  strip_prefix = "googletest-bea621c3c39d8a7f71f07bd543c3a58bfa684f92",
)
