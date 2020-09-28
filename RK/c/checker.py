import subprocess
import shutil
import os
import sys
import time

inputFileName = "ahocorasick.in"

numTests = 19
numPassed = 0

header = "====== RK ======"
print("\n" + header + "\n")

for n in range(1, numTests + 1):
	shutil.copy("../../util/tests/grader_test" + str(n) + ".in", inputFileName)

	start_time = time.time()
	proc = os.popen("./rk")
	proc.close()
	elapsed_time = time.time() - start_time
	result = open("ahocorasick.out").read()

	expectedResult = open("../../util/tests/grader_test" + str(n) + ".ok").read()
	if expectedResult == result:
		result = "passed"
		numPassed += 1
	else:
		result = "failed"

	start = "Test " + str(n)
	print(start + "." * (len(header) - len(result) - len(start)) + result + " (" + str(elapsed_time) + ")")

totalScore = "Result: " + str(numPassed) + "/" + str(numTests)
print("\n" + " " * (len(header) - len(totalScore)) + totalScore)

os.remove(inputFileName)
os.remove("ahocorasick.out")
