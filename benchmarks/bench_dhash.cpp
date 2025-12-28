#include "fo/providers/dhash.hpp"
#include <iostream>

int main() {
    fo::providers::DHash dhash;
    // Download a test image to the root of the project before running this.
    // wget https://upload.wikimedia.org/wikipedia/commons/c/ca/1x1.png -O test.png
    std::cout << "dHash of test.png: " << dhash.fast64("test.png") << std::endl;
    return 0;
}
