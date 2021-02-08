
#
# Program to generate random expressions (and their result) and print them in 
# a text file as test input for an "eval()"-function written in C, C++, Java, js
# 

#
# code by Twareintor - Copyright (c) 2020 Claudiu Ciutacu
#
# see original version on onlinegdb.com here: https://onlinegdb.com/bSqLgD-9m
#

import operator
import random

USE_BLOCKS = False
USE_POWERS = False

# 
# This function will return a string of chars representing a mathematical expression
# consisting of numbers (between 0. and 100.) and operators (+-*/) with or wihtout
# spaces between them. The evaluated result is also printed. The result is in such
# a manner that will represend a two-column CSV double-delimited (, and ;), first
# being the expression and another one the result. They will be also separated
# through a C++ line-comment // in order to be ready to be used in C/C++ program 
# as test input. The use as CSV will require deleting of the // and of one of the
# delimiters (, or ; depending on system) and probably the adjacent spaces too.
# Revision: in order to generate sub-expressions: added arguments "mint" for min
# and "maxt" max number of terms with default values; the function calls itself
# recursively to also generate expressions inside a pair of "(...)" parantheses
# 
def randomExpr(mint = 3, maxt = 20): # between 3 and 20 terms, by default
    expr = "" # return value
    nTerms = random.randint(mint, maxt) # between min an max terms
    op0 = ""
    pw = "^"
    if not USE_POWERS: pw = "*" ## a multiplication more, not ignored!
    for i in range(0, nTerms):
        number = round(random.uniform(0.0, 99.999), 3)
        op = random.choice(["+", "+", "-", "-", "*", "/", pw]) # a sign before
        if pw == op: op = random.choice(["+", "-", "*", pw]) # reduce the ^s
        op0 = op;
        # to mitigate division to zero, do this:
        if "/"==op: number = round(random.uniform(0.1, 99.999), 3)
        if "^"==op: number = round(random.uniform(0.5, 2.0), 2)
        if i>0: expr += op #else can begin with a sign but not with */^
        else: expr += random.choice(["+", "-", " +", " -", " ", ""])
        nBlnks = random.randint(0, 3)  # random number of blaks between...
        for k in range(0, nBlnks): expr += " "
        if not random.randint(0, 12) and USE_BLOCKS and "^"!=op: 
            # one in 12 terms will be a block, if not power
            expr += ("(" + randomExpr(2, 5) + ")")
        else:
            expr += str(number) # append to number
        nBlnks = random.randint(1, 4)
        for k in range(1, nBlnks-1): expr += " "
    return expr
    
#
# prints "howMany" random epxressions in the file name "fName", 
#   in the mode "fMode"("w" or "a") using "randomExpr()"
#
def prnRandomExprIn(fName, fMode, howMany, mint = 3, maxt = 20):
    f = open(fName, fMode)
    for j in range(0, howMany):
        rndExpr = randomExpr(mint, maxt) # generate random expr.
        # print the expression in the file, evaluate it and print result too:
        #### print(rndExpr)
        #### print("\t\"" + rndExpr + "\", //; \"" + str(eval(rndExpr.replace("^", "**"))) + "\"", file = f)
        #### print("\t\"" + rndExpr + "\", //; \"" + str(eval(rndExpr.replace("^", "**"))) + "\"", file = f)
        print(rndExpr + "; " + str(eval(rndExpr.replace("^", "**"))), file = f)
    f.close()

# the exit function
def main():
    print("Starting generating random expressions...\n")
    # WARNING!!! if you put "w" as 2nd argument the file will be overwritten
    prnRandomExprIn("testExpr.txt", "a", 30)
    print("\nDone! See results in the output file\n")
    
##################################################
main()
##################################################

# end of program

