#!/usr/bin/env python3

import subprocess
from colorama import Fore, Back, Style

def run_test(title: str, exp_code: int, exp_output: str, inp: str) -> None:
    p = subprocess.run(["../bin/eesh", "-c", inp], \
                       capture_output=True, text=True);
    if (p.stdout[0:-1] != exp_output):
        print(Fore.RED + "[FAIL]" + Style.RESET_ALL + \
              f" -> Testing {title}. Given [{inp}], expected [{exp_output}], received [{p.stdout[0:-1]}].")
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

    try:
        with open("test_cases.txt", "r") as file:
            counter = 1
            for line in file:
                if (len(line) == 1):
                    # newline char only, ignore
                    continue
                if (line[0] != "#"): # pound designates comment. groups it
                    print(f"Test {counter}: ", end="")
                    unit_test = line.strip().split(", ")
                    run_test(unit_test[0][1:-1], int(unit_test[1]),  
                             unit_test[2][1:-1], unit_test[3][1:-1])
                    counter += 1            

    except FileNotFoundError:
        print(Fore.RED + "Error:" + Style.RESET_ALL + " Unit test file not found.\n")

if __name__ == "__main__":
    main()
