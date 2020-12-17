import os
import time

"""
This is an executable version of an NLOC counter for a SWI-prolog program
*It probably works on windows and macOS
as long as you have a python interpreter installed

windows -> double-click
ubuntu -> $ python3 NLOC_Excutable.py
"""

program = "NLOC_Executable"

def countNloc(filename):
    countL,countC,countT = 0,0,0
    switch = True        
    
    with open(filename,'r') as f_in:
        for x in f_in:
            countT += 1
            if not switch:
                countC += 1
                if '*/' in x:
                    countC += 1
                    switch = True
                continue
            elif '/*' in x:
                switch = False
                continue
        
            if x[0] == '%':
                countC +=1
            elif ',' in x or '.' in x or '-' in x:
                countL +=1
                
    print('Total Lines: {}\nNLOC: {}\nComments: {}\n'.format(countT,countL,countC))
    return (countL,countC)

while True:
    file = input("{}: Prolog file ?: ".format(program))
    if os.path.isfile(file):
        break
    print("{}: File does not exist".format(program))
countNloc(file)

time.sleep(5)
