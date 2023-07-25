#include <iostream>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "base64.h"

ABSL_FLAG(bool, d, false, "When set, the input is decoded from base64.");

int main(int argc, char *argv[])
{
    absl::ParseCommandLine(argc, argv);

    absl::Status status;
    if (absl::GetFlag(FLAGS_d))
    {
        status = base64::decode(std::cin, std::cout);
    }
    else
    {
        status = base64::encode(std::cin, std::cout);
    }
    if (!status.ok())
        std::cout << status.message() << std::endl;

    return 0;
}