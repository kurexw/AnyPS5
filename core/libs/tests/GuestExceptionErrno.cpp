#include "prx/libc/include/GuestErrno.hpp"

#include <cstdio>
#include <cstdlib>
#include <source_location>
#include <stdexcept>

namespace {

void Require(bool condition, std::source_location location = std::source_location::current()) {
    if (condition) return;
    std::fprintf(stderr, "Exception errno check failed at line %u\n", location.line());
    std::abort();
}

template<typename TException>
void RequireErrnoKept(int value) {
    int& guestErrno = *__error_nid_postfix();
    guestErrno = value;
    try {
        throw TException("guest");
    } catch (const std::exception&) {
    }
    Require(guestErrno == value);
}

}

int main() {
    RequireErrnoKept<std::runtime_error>(71);
    RequireErrnoKept<std::logic_error>(72);
    RequireErrnoKept<std::invalid_argument>(73);
    std::puts("Exception errno tests passed");
}
