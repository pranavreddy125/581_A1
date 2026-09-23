# IPv4 address extractor

This C++17 program scans each input line for a valid IPv4 address with an optional
port. It stops when the entire input line is `END`.

## Build and run

```sh
make
./build/ipv4_extractor
```

## Test

```sh
make test
```

The tests check 35 focused cases, every numeric port from 0 through 99999,
each octet from 0 through 999 in each position, and the exact interactive
output. The boundary sweeps also check that failed calls reset both output
parameters.

## How the parser works

1. `extractIPv4` skips every character outside `0`–`9`, `.` and `:`. Each
   uninterrupted run of those characters is one complete candidate token.
2. `parseToken` reads exactly four octets. `readNumber` accumulates decimal
   digits by multiplying the current value by 10 and adding the next digit.
   It checks digit count, range, and leading zeroes.
3. A colon after the fourth octet triggers the same checks for a port. The
   candidate succeeds only if every character has been consumed.
4. On success, the four octets are packed into a 32-bit value. `main` unpacks
   that value for the required dotted display and prints the port or `none`.

No numeric conversion, address-parsing, or regular-expression library is used
in the program. `tests/test_ipv4.cpp` uses `std::to_string` only to construct
test inputs; it does not parse them.

