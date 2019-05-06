import sys
import os

file = sys.argv[1]


"""
This is a terminal version of an NLOC counter for a SWI-prolog program
usage: python3 NLOC_Terminal.py <prolog_project.pl>
"""

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


if not os.path.isfile(file):
	print("NLOC_Terminal: File does not exist")
	quit()

countNloc(file)
