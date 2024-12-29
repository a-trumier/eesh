#!/usr/bin/env python3

import subprocess
from colorama import Fore, Back, Style

def run_test(title: str, exp_code: int, exp_output: str, inp: str) -> None:
    p = subprocess.run(["../bin/eesh", "-c", inp], \
                       capture_output=True, text=True);
    if (p.stdout != exp_output):
        print(Fore.RED + "[FAIL]" + Style.RESET_ALL + \
              f" -> Testing {title}. Given [{inp}], expected [{exp_output}], received [{p.stdout.strip()}].")
    elif (p.returncode != exp_code):
        print(Fore.RED + "[FAIL]" + Style.RESET_ALL + f" -> Testing {title}. Given {inp}, expected code {exp_code}, received {p.returncode}") 
    else:
        print(Fore.GREEN + "[PASS]" + Style.RESET_ALL)

def main():

    makecmd = subprocess.run(["make", "all"], capture_output=True, text=True, cwd="..")
    if (makecmd.returncode != 0):
        print(Fore.RED + "Error:" + Style.RESET_ALL + \
              " make command to create shell failed.\n")
        exit(1);

    print("--- RUNNING UNIT TESTS ---\n")
    # Basic tests
    run_test("Basic command execution", 0, "hello\n", "/bin/echo hello")
    run_test("Simple variable assignment", 0, "5\n", "export x 5; echo $x")
    run_test("Redirection to file", 0, "test\n", "echo test > testfile; cat testfile; rm testfile")
    
    # Edge cases
    run_test("empty command", 0, "", "")
    run_test("invalid command", 127, "", "invalidcommand")
    run_test("recursive variable expansion", 0, "hello\n", "export y hello; export x $y; echo $y")

    # simple errors
    run_test("unterminated quote", 1, "", "echo \"unterminated")
    run_test("actual command", 0, "Hello World\n", "echo Hello World")

if __name__ == "__main__":
    main()
