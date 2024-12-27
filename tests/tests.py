#!/usr/bin/env python3

import subprocess
import os

# run make command for all the binaries needed for testing.
subprocess.run(["make all"], capture_output=True, shell=True)

dir_path = os.path.dirname(os.path.realpath(__file__))
with open("test_cases/input_tests.txt", "r") as f:
    for line in f:
        cur_case = line.strip().split(",")
        p = subprocess.run([dir_path + '/build/read_input_test'], 
                           input=cur_case[0], capture_output=True, 
                           text=True, shell=True)

        if (p.stdout != cur_case[1]):
            print(f"Error on read_input_tests. Given {cur_case[0]}, output {p.stdout}. Expected {cur_case[1]}.\n")

