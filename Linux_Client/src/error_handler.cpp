#include <iostream>

void error_handler() {
    std::cout << "Exception: Entered Error Handler" << std::endl;
    while (0) {
        // Error?
    }
}