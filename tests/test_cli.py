#!/usr/bin/env python3
"""Check the exact interactive transcript, including the END sentinel."""

import subprocess
import sys


def main() -> None:
    executable = sys.argv[1]
    inputs = [
        "connecting to 192.168.1.1 now",
        "server=10.0.0.255:8080end",
        "192a168.1.1.1",
        "192.168.1.1.",
        "192.168.01.1",
        "1.2.3.4:99999",
        "no number here",
        "END",
        "1.2.3.4",  # Must not be processed after END.
    ]
    results = [
        "Extracted IPv4 address: 192.168.1.1 (decimal value: 3232235777, port: none)",
        "Extracted IPv4 address: 10.0.0.255 (decimal value: 167772415, port: 8080)",
        "Extracted IPv4 address: 168.1.1.1 (decimal value: 2818638081, port: none)",
        "Invalid input: no valid IPv4 address found",
        "Invalid input: no valid IPv4 address found",
        "Invalid input: no valid IPv4 address found",
        "Invalid input: no valid IPv4 address found",
        "Program terminated.",
    ]
    prompt = "Enter a string (or 'END' to quit): "
    expected = "".join(prompt + result + "\n" for result in results)
    completed = subprocess.run(
        [executable],
        input="\n".join(inputs) + "\n",
        text=True,
        capture_output=True,
        check=True,
    )
    if completed.stdout != expected or completed.stderr:
        raise AssertionError(
            f"Unexpected transcript:\n{completed.stdout!r}\n"
            f"Expected:\n{expected!r}\nStderr:\n{completed.stderr!r}"
        )
    print("Passed exact CLI transcript and END handling.")


if __name__ == "__main__":
    main()
