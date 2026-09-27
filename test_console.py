"""Check a freshly compiled program's exact console output (Python 3).

Usage: python test_console.py ./ipv4_finder.exe
Input is piped, so the terminal does not echo it into the captured output.
This checks the actual C++ executable; it does not reimplement the parser.
"""

from pathlib import Path
import difflib
import subprocess
import sys

PROMPT = "Enter a string (or 'END' to quit): "
INVALID = "Invalid input: no valid IPv4 address found"
TERMINATED = "Program terminated."


def run(executable, arguments, input_text):
    return subprocess.run(
        [str(executable), *arguments], input=input_text,
        capture_output=True, text=True, timeout=10, check=False,
    )


def main():
    if len(sys.argv) != 2:
        print("Usage: python test_console.py PATH_TO_COMPILED_PROGRAM")
        return 2
    executable = Path(sys.argv[1]).resolve()
    if not executable.is_file():
        print(f"Executable not found: {executable}")
        return 2

    unit = run(executable, ["--test"], "")
    if (unit.returncode != 0 or unit.stderr or
            unit.stdout != "79/79 parser tests passed.\n"):
        print("Parser tests failed, or this is an outdated executable:")
        print(unit.stdout, end="")
        print(unit.stderr, end="")
        return 1
    print(unit.stdout, end="")

    # Each pair supplies a complete input line and its exact required response.
    scenarios = [
        ("assignment sample", [
            ("connecting to 192.168.1.1 now",
             "Extracted IPv4 address: 192.168.1.1 (decimal value: 3232235777, port: none)"),
            ("server=10.0.0.255:8080end",
             "Extracted IPv4 address: 10.0.0.255 (decimal value: 167772415, port: 8080)"),
            ("192a168.1.1.1",
             "Extracted IPv4 address: 168.1.1.1 (decimal value: 2818638081, port: none)"),
            ("192.168.1.1.", INVALID),
            ("Connection from 192.168.1.1 refused",
             "Extracted IPv4 address: 192.168.1.1 (decimal value: 3232235777, port: none)"),
            ("192.168.01.1", INVALID),
            ("1.2.3.4:99999", INVALID),
            ("12.34.56", INVALID),
            ("no number here", INVALID),
        ]),
        ("only first valid address", [
            ("999.2.3.4 1.2.3.4:0 8.8.8.8:53",
             "Extracted IPv4 address: 1.2.3.4 (decimal value: 16909060, port: 0)"),
        ]),
        ("numeric limits", [
            ("0.0.0.0:0", "Extracted IPv4 address: 0.0.0.0 (decimal value: 0, port: 0)"),
            ("255.255.255.255:65535",
             "Extracted IPv4 address: 255.255.255.255 (decimal value: 4294967295, port: 65535)"),
        ]),
        ("END is exact and case sensitive", [
            ("end", INVALID), (" END ", INVALID), ("", INVALID),
        ]),
        ("invalid punctuation and zeros", [
            (":1.2.3.4", INVALID), ("1.2.3.4.5", INVALID),
            ("00.0.0.0", INVALID), ("1.2.3.4:00", INVALID),
            ("1.2.3.4:65536", INVALID),
        ]),
        ("huge malformed candidates", [
            ("9" * 10000 + ".2.3.4", INVALID),
            ("1.2.3.4:" + "9" * 10000, INVALID),
        ]),
    ]

    failures = 0
    for name, pairs in scenarios:
        # A line after END must never be processed or produce another prompt.
        input_text = "".join(line + "\n" for line, _ in pairs)
        input_text += "END\n8.8.8.8\n"
        expected = "".join(PROMPT + response + "\n" for _, response in pairs)
        expected += PROMPT + TERMINATED + "\n"
        actual = run(executable, [], input_text)
        if actual.returncode != 0 or actual.stderr or actual.stdout != expected:
            failures += 1
            print(f"FAIL: {name}")
            print("".join(difflib.unified_diff(
                expected.splitlines(True), actual.stdout.splitlines(True),
                fromfile="expected", tofile="actual",
            )), end="")
            print(actual.stderr, end="")
        else:
            print(f"PASS: {name}")

    # Empty input should exit cleanly on EOF after the initial prompt.
    eof = run(executable, [], "")
    if eof.returncode != 0 or eof.stderr or eof.stdout != PROMPT:
        failures += 1
        print("FAIL: EOF exits cleanly")
    else:
        print("PASS: EOF exits cleanly")

    total = len(scenarios) + 1
    print(f"{total - failures}/{total} console scenarios passed.")
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
