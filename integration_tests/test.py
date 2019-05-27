from sys import argv
from subprocess import check_output, CalledProcessError, STDOUT
from json import load as jsonLoad
from itertools import product
import re
import unittest


COMPILED_CPLUS = "../rshell"


def runCommand(command): #takes in an a command and runs it in the base compiled c++ code as an argv
    #examples
    # runCommand('echo ping') - > return ping


    commands = [COMPILED_CPLUS, command]
    try:
        output = check_output(commands, stderr=STDOUT).decode()
    except Exception as e:
        output = e.output.decode()

    return output

class DynamicTest(unittest.TestCase):
    ...


def getCombinations(givenList): # Get all possible combinations of a list

    def combinationGenerator(givenList):
        yield from product(givenList)
        yield from product(givenList, givenList)

    return [x for x in combinationGenerator(givenList)]

def dynamicTestingGenerator(input): #Where a list of tuples becomes an actual command

    def do_test_expected(self):
        result = runCommand(stringInput)
        self.assertEqual(result.count('ping'), outputPings) # the two pings test

        Gilmour = re.split("(#/)", result)
        Gilmour = [x for x in Gilmour if "Gilmour" in x and "#" not in x] #the no gilmies test
        self.assertFalse(Gilmour)


    outputPings = len(input) * 2

    stringInput = ""

    for x in input:

        if not stringInput:
            stringInput += x
        elif x[-1:] == ";":
            stringInput += " " + x
        else:
            stringInput += "; " + x

    while ";;" in stringInput: #ive been spending too long trying to figure out why ;; is happening. This should fix it.
        stringInput = stringInput.replace(";;", ";")


    return do_test_expected

def main():
    jsonInputs = iter(argv)
    next(jsonInputs)
    INPUTS = [] # list which contains 
    for input in jsonInputs: # reads argv json files
        print("Reading " + input + " ...")
        assert ".json" in input, "Must input json files!"
        data = jsonLoad(open(input))
        INPUTS.extend(data)

        print("Found " + str(len(data)) + " inputs.")


    
    while len(argv) > 1: # unittest.main() gets mad if you pass in argv, so I pop all them off
        argv.pop()

    COMMENTS = [x for x in INPUTS if "#"  in x] #Comments should not be in a chain of commands, because comments ruin everything
    PARENS = [x for x in INPUTS if "(" or ")" in x] #Parens should not be in a chain of commands, because they dont work
    COMBINATIONS = getCombinations(list(set(INPUTS) - set(COMMENTS) - set(PARENS)))
    [COMBINATIONS.append(tuple([x]),) for x in COMMENTS]
    [COMBINATIONS.append(tuple([x]),) for x in PARENS]
    

    print("\n" + str(len(INPUTS)) + " inputs found")

    INPUTS = [] #free some of dat ram
    COMMENTS = []
    
    for x in COMBINATIONS: #Actually dynamically creates the unit tests
        test_method = dynamicTestingGenerator(x)
        test_method.__name__ = "test_expected%d" + str(x) # If you try to change the name from test_expected%d IT BREAKS! MY GOD THIS TOOK ME AN HOUR TO FIGURE OUT
        setattr(DynamicTest, test_method.__name__, test_method)



    unittest.main()

if __name__ == "__main__":
    main()