# Exact prompts and clarification

These are the user's messages in this conversation, in order. Assistant replies
are not reproduced. The complete assignment supplied with message 5 appears
below so that the requirements-review input can be reconstructed.

## 1. Initial request

~~~text
Help me write a C++ program that finds an IPv4 address and optional port number in a line of text. It should skip surrounding junk, reject invalid addresses or ports, and keep asking for input until I type END. Parse the numbers manually without built-in conversion functions or regex. Include comments explaining how it works and some tests for edge cases.
~~~

## 2. Packaging request

~~~text
give me a downloadable zip file
~~~

## 3. Multiple-match request

~~~text
I would like it to find every valid ip in the message not just the first valid ip
~~~

## 4. Correction to one match per line

~~~text
Ok i miss read the instructions only one valid ip address can be extracted per line
~~~

## 5. Requirements review request

The attached file was named Pasted text.txt. Its contents are included at the end.

~~~text
Now does this meet all the requirements
~~~

## 6. Revision request

~~~text
lets update it to meet the requirements
~~~

## 7. Leading-zero clarification

Assistant's clarification question:

~~~text
The assignment says “no leading zero unless the value is exactly 0,” which is ambiguous for `00` and `000`. Has your instructor clarified this? I’ll use the usual rule—zero must be written as a single `0`—unless you specify otherwise.
~~~

User's exact selected answer:

~~~text
Use a single 0 only
~~~

## Full assignment supplied with the requirements-review request

~~~text

Objectives
This assignment has two layers. The first is technical: develop proficiency in C/C++ string parsing and validation of structured, numeric-like data embedded in unstructured text. The second is professional and is the primary point of this exercise: develop and demonstrate disciplined use of generative AI (GAI) tools in software development.

Specifically, you will:

Develop skills in GAI-assisted code generation, including writing precise prompts for a constrained C/C++ parsing task.
Strengthen your ability to critically review AI-generated output — identifying logical errors, missed edge cases, and invalid assumptions rather than accepting code because it compiles and looks plausible.
Improve your skills in testing and validation by designing your own test cases for malformed input, ambiguous input, and numeric limits.
Practice professional documentation and AI disclosure: clearly explaining how AI was used, what you changed, and why.
A word of honest expectation-setting: this exact style of problem (structured-token extraction from garbage text, implemented without library conversion functions) reliably produces AI-generated code with subtle bugs on the first attempt. That is not a flaw in the assignment — it is the point. Expect to need several rounds of prompting and manual correction before your program handles every case correctly. If your AI tool gets everything right on the first prompt, look harder at your test cases before you believe it.

Motivation (real-world use cases)
Problems like this arise in practice whenever structured data must be recovered from noisy, unstructured text — for example, extracting a device's IP address from firewall logs, router diagnostic output, IoT device status messages, or packet-capture summaries that mix the address with timestamps, hostnames, free-text status codes, and transmission noise. Because the surrounding text cannot be trusted and the value may be embedded anywhere within it, software must scan carefully, ignore irrelevant characters, strictly validate the structure of what looks like an address, and reject anything malformed rather than guessing at intent.

As with numeric parsing, real-world address-like text often contains near-misses that must be rejected: an octet out of range, a malformed port, a truncated address, or a stray punctuation mark that turns what looks like a clean address into something structurally invalid. Defensive software must never assume such tokens are well-formed, and must never silently "fix" or guess at a corrupted value — it must validate and reject.

What to turn in
A URL to your GitHub repository containing your source code, your test cases, and your AI-disclosure document (see below).

Problem description
Write a C or C++ program that reads a line of text and extracts a single valid IPv4 address — optionally followed by a port number — embedded anywhere in that text. Only digits, periods (.), and colons (:) are ever part of a valid token; every other character is garbage and is skipped. A candidate token must match the address grammar in full — no partial matches, no truncating to find a valid piece inside a longer run.

An address is four octets separated by periods (octet.octet.octet.octet), each octet 1–3 digits, value 0–255, no leading zero unless the value is exactly 0. An optional :port may follow the fourth octet: 1–5 digits, value 0–65535, same leading-zero rule. If a colon is present, the port must be fully valid or the entire match — address included — is rejected.

Function prototype
The function returns whether a valid address was found, and delivers the address and port through two additional parameters.

C:
// Returns 1 if a valid address was found, 0 otherwise.
// On success: *outAddress holds the 32-bit value, and
// *outPort holds the port number, or -1 if no port was present.
// On failure: *outAddress is set to 0 and *outPort is set to -1.
int extractIPv4(const char* str, unsigned long* outAddress, int* outPort);

C++

// Returns true if a valid address was found, false otherwise.
// On success: outAddress holds the 32-bit value,
// and outPort holds the port number, or -1 if no port was present.
// On failure: outAddress is set to 0 and outPort is set to -1.
bool extractIPv4(const std::string& str, unsigned long& outAddress, int& outPort);

Built-in functions/libraries you may not use
Any string-to-number conversion function: atoi, atol, atoll, strtol, strtoul, strtod, stoi, stol, stoul, sscanf, scanf with numeric conversions.
Any address-parsing library function: inet_aton, inet_pton, inet_addr, or equivalents.
Any regular-expression facility (std::regex, POSIX regex.h, or similar) — the parsing and validation logic must be your own character-by-character code, not a pattern matched by a library.
Standard character-classification functions (isdigit, etc.) are fine to use.
Other requirements
Exactly one valid address may be extracted per input line; everything else in the line is either garbage (skipped) or part of a candidate token that fails validation.
Reject anything that does not exactly match the grammar above — wrong octet count, empty octet, out-of-range octet or port, a disallowed leading zero, a second colon, a colon not immediately after the fourth octet, or a stray period/colon directly adjacent to an otherwise-valid address.
On success, print: Extracted IPv4 address: A.B.C.D (decimal value: N, port: P) where N is the 32-bit decimal value and P is the port number or the literal text none.
Program requirements
Input loop: main continuously prompts the user for input until the user enters END (case-sensitive), then prints Program terminated. and exits.
Extraction function: implement extractIPv4 exactly as prototyped above. All digit accumulation must be done by hand.
Display: main receives the result from extractIPv4 and formats the output exactly as specified above.
AI usage and disclosure requirements
You are required to use a generative AI tool for all or at least part of this assignment's code generation, and to document that usage professionally. The purpose is not to test whether you can write this parser unassisted — it is to test whether you can direct an AI tool effectively, evaluate its output critically, and communicate that process clearly.

General disclosure
Clearly state which GAI tool was used (e.g., ChatGPT, Copilot, Gemini, etc.), and specify the version if known (e.g., GPT-4, Claude 3.5).
Indicate the date(s) the tool was consulted.
Code attribution
Copy the exact prompt(s) you used to generate the code.
Document which parts of the code were AI-generated versus student-written.
Note any modifications you made to the AI-generated output, and why.
Verification statement
Confirm that you understand every line of the submitted code — no blind copying.
State that the code has been tested and works as intended.
Acknowledge any known bugs, limitations, or unexpected behavior you did not resolve.
Grading
Accuracy (20 pts) depends on two skills working together: writing a good prompt, and critically evaluating what the AI produces.

If the AI produces incorrect code and you accept it without catching the error, you lose these points.
If the AI produces correct code and you modify it in a way that introduces an error, you also lose these points.
GAI disclosure quality (20 pts) rewards transparency and thoughtful engagement — it should make your learning process visible to the instructor. A disclosure that just says "used ChatGPT to write the code" earns little; a disclosure that shows what you asked, what came back wrong, and how you diagnosed and fixed it earns full credit.

User experience (10 pts) four-decimal-equivalent formatting for this assignment is the fixed output format above; clear feedback; continuous input loop until END.

Sample run

Enter a string (or 'END' to quit): connecting to 192.168.1.1 now
Extracted IPv4 address: 192.168.1.1 (decimal value: 3232235777, port: none)
Enter a string (or 'END' to quit): server=10.0.0.255:8080end
Extracted IPv4 address: 10.0.0.255 (decimal value: 167772415, port: 8080)
Enter a string (or 'END' to quit): 192a168.1.1.1
Extracted IPv4 address: 168.1.1.1 (decimal value: 2818638081, port: none)
Enter a string (or 'END' to quit): 192.168.1.1.
Invalid input: no valid IPv4 address found
Enter a string (or 'END' to quit): Connection from 192.168.1.1 refused
Extracted IPv4 address: 192.168.1.1 (decimal value: 3232235777, port: none)
Enter a string (or 'END' to quit): 192.168.01.1
Invalid input: no valid IPv4 address found
Enter a string (or 'END' to quit): 1.2.3.4:99999
Invalid input: no valid IPv4 address found
Enter a string (or 'END' to quit): 12.34.56
Invalid input: no valid IPv4 address found
Enter a string (or 'END' to quit): no number here
Invalid input: no valid IPv4 address found
Enter a string (or 'END' to quit): END
Program terminated.
~~~
