import sys
import os
import tempfile
import subprocess
import difflib

tests = open("tests/tests").read().split("\n")[:-1]
for i, testfilename in enumerate(tests):
    testfile = open("tests/%s"%testfilename)
    testname = None
    exitcode = None
    testinput = None
    testcode = None
    testoutput = None
    
    for line in testfile:
        if testname == None:
            if line == "\n":continue
            if line[:6] != "#name ":
                sys.exit("Invalid name line '%s' in test file '%s'"%(line[:-1], testfilename))
            testname = line[6:-1]
            continue
        if exitcode == None:
            if line == "\n":continue
            if line[:6] != "#exit ":
                sys.exit("Invalid exit code line '%s' in test file '%s'"%(line[:-1], testfilename))
            exitcode = int(line[6:-1])
            continue
        if testinput == None:
            if line == "#input\n":
                testinput = ""
            continue
        if testcode == None:
            if line == "#code\n":
                testcode = ""
            else:
                testinput += line
            continue
        if testoutput == None:
            if line == "#output\n":
                testoutput = ""
            else:
                testcode += line
            continue
        testoutput += line
    testfile.close()
    
    if None in (testname, exitcode, testinput, testcode, testoutput):
        sys.exit("Not all sections specified in test file '%s'"%testfilename)
    
    infoline = "Test #%s: %s"%(i, testname)
    print(infoline + " " * (50 - len(infoline)), end="")
    
    codefile = open("tests/current", "w")
    codefile.write(testcode)
    codefile.close()
    
    result = subprocess.run(["./build/oper", codefile.name], stdout=subprocess.PIPE, input=testinput.encode("utf-8"))
    if result.stdout.decode("utf-8") == testoutput and result.returncode == exitcode:
        print("\033[92m✓\033[0m")
    else:
        print("\033[91mX\033[0m")
        print("Expected exit code: %s, actual exit code: %s"%(exitcode, result.returncode))
        print("Output:\n%s"%(
            "\n".join(
                [actual if actual == expected else 
                    "\033[91m%s \033[0m(expected \033[94m%s\033[0m)"%(actual, expected) for actual, expected in zip(
                        result.stdout.decode("utf-8").split("\n"),
                        testoutput.split("\n")
                    )
                ]
            )
        ))
        sys.exit("Test failed: '%s'"%testname)
os.remove("tests/current")
            
            
            
            
