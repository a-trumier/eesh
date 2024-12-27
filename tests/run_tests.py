#!/usr/bin/env python3

import subprocess
import os
import argparse
import sys

parser = argparse.ArgumentParser(prog="eesh test script")

parser.add_argument("-v", "--verbose", help="prints every test case, even if passed", action="store_true")
parser.add_argument("-c", "--clean", help="runs make clean after running script", action="store_true")
parser.add_argument("-s", "--silent", help="does not print anything", action="store_true")

args = parser.parse_args(sys.argv[1::])

# run make command for all the binaries needed for testing.
subprocess.run(["make all"], capture_output=True, shell=True)

dir_path = os.path.dirname(os.path.realpath(__file__))
c = 0
with open("test_cases/input_tests.txt", "r") as f:
    for line in f:
        cur_case = line.strip().split(",")
        p = subprocess.run([dir_path + '/build/read_input_test'], 
                           input=cur_case[0], capture_output=True, 
                           text=True, shell=True)

        if (p.stdout != cur_case[1] and (not args.silent)):
            print(f"Error on read_input_tests. Given {cur_case[0]}, output {p.stdout}. Expected {cur_case[1]}.\n")
            c += 1

with open("test_cases/tokenize_input_tests.txt", "r") as f:
    for line in f:
        cur_case = line.strip().split(",")
        arg = [dir_path + "/build/tokenize_input_test", cur_case[0]]
        p = subprocess.run(arg, capture_output=True, text=True)
        if (p.stdout != cur_case[1] and (not args.silent)):
            print(f"Error on tokenize_input_tests. Given {cur_case[0]}, output {p.stdout}. Expected {cur_case[1]}.\n")
            c += 1

if not args.silent:
    print(f"--- Test Cases Completed. Number of Errors: {c} ---")

if args.clean:
    subprocess.run(["make clean"], capture_output=True, shell=True)
