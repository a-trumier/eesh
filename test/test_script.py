#!/usr/bin/env python3

import subprocess
from colorama import Fore, Back, Style

def run_test(title: str, exp_code: int, exp_output: str, inp: str) -> None:
    p = subprocess.run(["../bin/eesh", "-c", inp], \
                       capture_output=True, text=True);
    if (p.stdout != exp_output):
        print(Fore.RED + "[FAIL]" + Style.RESET_ALL + \
              f"  -> Testing {title}. Given {inp}, expected {exp_output}, received {p.stdout}.")
        print(f"stderr: {p.stderr}")
    elif (p.returncode != exp_code):
        print(Fore.RED + "[FAIL]" + Style.RESET_ALL + f" -> Testing {title}. Given {inp}, expected code {exp_code}, received {p.returncode}") 
        print(f"stderr: {p.stderr}")
    else:
        print(Fore.GREEN + "[PASS]" + Style.RESET_ALL)

def main():
    print("--- RUNNING UNIT TESTS ---\n")
    # Basic tests
    run_test("Basic command execution", 0, "hello\n", "/bin/echo hello")
    run_test("Simple variable assignment", 0, "5\n", "export x 5; echo $x")
    # test_case "Command substitution" "echo \$(echo hello)" "hello" 0
    run_test("Redirection to file", 0, "test\n", "echo test > testfile; cat testfile; rm testfile")

    # test_case "Pipeline" "echo hello | tr a-z A-Z" "HELLO" 0
    # test_case "Exit command" "exit 42" "" 42

    #test_case "Subshell execution" "(echo subshell)" "subshell" 0

    #test_case "Conditional - true" "true && echo success" "success" 0
    #test_case "Conditional - false" "false || echo failure" "failure" 0

    #test_case "If statement" "if true; then echo yes; else echo no; fi" "yes" 0
    #test_case "For loop" "for i in 1 2 3; do echo $i; done" "1
    #   2
    #   3" 0

    #test_case "While loop" "i=1; while [ $i -le 3 ]; do echo $i; i=$((i + 1)); done" "1
    #2
    #3" 0

    # Edge cases
    run_test("empty command", 0, "", "")
    run_test("invalid command", 127, "", "invalidcommand")
    #test_case "Special characters" "echo \"*?[]\\\"" "*?[]\\" 0
    run_test("recursive variable expansion", 0, "hello\n", "export y hello; export x $y; echo $y")

    run_test("unterminated quote", 1, "", "echo \"unterminated")
    # run_test("Large input", 0, "10000", "yes | head -n 10000 | wc -l")
    run_test("actual command", 0, "Hello World\n", "echo Hello World")

if __name__ == "__main__":
    main()
