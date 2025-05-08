"""
dp[i] = maksymalna suma kwadratów długości słów na które da się poprawine podzielić 
        i pierwszych liter w wierszu 
"""

dict = dict()
def fillDict():
    #with open('words_for_ai1.txt') as file:
    file = open("words_for_ai1.txt","r")
    for line in file:
        dict[line.rstrip()] = True
    """
    with open('words_for_ai1.txt') as file:
        contents = file.readlines()
        for line in contents:
            dict[line.rstrip()] = True
    """

def checkIfWordExists(word):
    return word in dict 

def solveForLine(line):
    n = len(line)
    dp, prev   = [-1]*(n+5), [0]*(n+5)

    dp[0] = 0

    for i in range(0,n):
        for k in range(0,i+1):
            if checkIfWordExists(line[k:i+1])==True and dp[k]>-1:
                cost = dp[k] + (i-k+1)*(i-k+1)
                if cost > dp[i +1]:
                    dp[i+1]   = cost 
                    prev[i] = (i-k+1)
    
    resultLine, index = "", n-1

    while index > 0:
        resultLine = line[index - prev[index]+1:index+1] + ' ' + resultLine
        index = index - prev[index]
    return resultLine +'\n'

def solve( ):

    #fillDict()

    f = open("zad2_output.txt", "w")
    with open('zad2_input.txt') as file:
        for line in file:
            f.write(solveForLine(line.rstrip()))

solve()