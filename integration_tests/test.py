from sys import argv
from subprocess import check_output, CalledProcessError, STDOUT
from json import load as jsonLoad
from itertools import product
import re
import unittest
from secrets import token_hex

COMPILED_CPLUS = "../rshell"
REDIRECTION_TYPES = ("<", "<<", ">", ">>")


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

class BaseCommand():
    command = ""
    outputType = ""
    POSSIBLE_OUTPUTS = ["Pass", "Fail", "End"]

    def getOutput(self, command):

        def generateStringNotInString(givenString):
            foundString = token_hex(16)
            while foundString in givenString:
                foundString = token_hex(16)
            return foundString

        def testPass(command, lookFor):
            newCommand = command + " && " + "echo " + lookFor
            if lookFor in runCommand(newCommand):
                return True
            return False
        def testFail(command, lookFor):
            newCommand = command + " || " + "echo " + lookFor
            if lookFor in runCommand(newCommand):
                return True
            return False

        if command[-1:] == ";":
            self.command = command[:-1]
            command = self.command


        lookFor = generateStringNotInString(command)
        if testPass(command, lookFor):
            self.outputType = "Pass"
        elif testFail(command, lookFor):
            self.outputType = "Fail"
        else:
            self.outputType = "End"
        return self.outputType

    def __repr__(self):

        return "[" + self.outputType + "]" + " " + self.command


    def __init__(self, command):
        self.command = command
        self.getOutput(command)
    
    



def getCombinations(givenList): # Get all possible combinations of a list

    def combinationGenerator(givenList):
        yield from product(givenList)
        yield from product(givenList, givenList)

    return [x for x in combinationGenerator(givenList) if x]

def dynamicTestingGenerator(input): #Where a list of tuples becomes an actual command
    assert isinstance(input, tuple)

    def do_test_expected_creator(givenInput, extraCommands):
        def do_test_expected(self):
            if extraCommands:
                [runCommand(x) for x in extraCommands]
            result = runCommand(stringInput)
            if extraCommands:
                [runCommand(x) for x in extraCommands]
            self.assertEqual(result.count('ping'), outputPings) # the two pings test
            Gilmour = re.split("(#/)", result)
            Gilmour = [x for x in Gilmour if "Gilmour" in x and "#" not in x] #the no gilmies test
            self.assertFalse(Gilmour)
            
        return do_test_expected


    outputPings = 0
    for x in input:
        outputPings += 2
        if x.outputType == "End":
            break
            


    stringInput = ""

    for x in input:
        command = x.command
        if not stringInput:
            stringInput += command
        else:
            if command[-1:] == " ":
                self.command = command[:-1]
            if x.outputType == "Pass":
                stringInput += " && " + command
            elif x.outputType == "Fail":
                stringInput += " || " + command
            else:
                stringInput += " ; " + command



    while ";;" in stringInput: #ive been spending too long trying to figure out why ;; is happening. This should fix it.
        stringInput = stringInput.replace(";;", ";")

    extraCommands = []
    if any(x for x in REDIRECTION_TYPES if x in command):
        splitboi = command.split(" ")
        files = [x for x in splitboi if ".txt" in x]
        extraCommands = ["rm " + x.replace(";", "") + ";" for x in files]

    return {
        "test" : do_test_expected_creator(stringInput, extraCommands),
        "command" : stringInput
    }


def addCommand(givenDict):
    assert isinstance(givenDict, dict)
    testObj = givenDict['test']
    commandName = givenDict['command']
    testObj.__name__ = "test_expected%d: " + commandName # If you try to change the name from test_expected%d IT BREAKS! MY GOD THIS TOOK ME AN HOUR TO FIGURE OUT
    setattr(DynamicTest, testObj.__name__, testObj)

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

    print("\n" + str(len(INPUTS)) + " total inputs found")

    REDIRECTIONS = [x for x in INPUTS if any(y for y in REDIRECTION_TYPES if y in x)]
    INPUTS = [BaseCommand(x) for x in INPUTS if (x not in REDIRECTIONS) and x]
    REDIRECTIONS = [BaseCommand(x) for x in REDIRECTIONS]
    
    while len(argv) > 1: # unittest.main() gets mad if you pass in argv, so I pop all them off
        argv.pop()


    COMBINATIONS = getCombinations(INPUTS)
    print(str(len(INPUTS)) + " of the inputs generated " + str(len(COMBINATIONS)) + " combinations.")

    INPUTS = [] # free dat ram

    for x in COMBINATIONS: #Actually dynamically creates the unit tests
        generated = dynamicTestingGenerator(x)
        addCommand(generated)

    for x in REDIRECTIONS:
        generated = dynamicTestingGenerator((x,))
        addCommand(generated)


    unittest.main()

if __name__ == "__main__":
    main()