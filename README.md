# Base64

A C++ base64 encoder and decoder library.


## Usage

```
#include <iostream>

#include "base64.h"

int main(){
    // Encode a string.
    std::cout << "Encoded: " << base64::encode("Manu") << "\n";

    // Decode a string.
    auto dec = base64::decode("TWFudQ==");
    if(!dec.ok())
        std::cout << dec.status() << "\n";
    else
        std::cout << "Decoded: " << *dec << "\n";

    const uint32_t encode_buffer_sz{4500};

    // Read from a stream and output to a stream.
    auto status = base64::encode(std::cin, std::cout, encode_buffer_sz);
    if(!status.ok())
        std::cout << status.message() << std::endl;

    return 0;
}
```

```
> echo -n "Manu" | bazel-bin/main
Encoded: TWFudQ==
Decoded: Manu
TWFudQ==
```