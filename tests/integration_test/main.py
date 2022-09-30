import sys
import os
from typing import Callable
import http.client
import config
import test
#test_path
test_path = "./test_case/"

# testname testfile
test.run("TEST", test_path + "a.txt")
test.run("HTTP VERSION 11 ", test_path + "http.txt")