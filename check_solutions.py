from time import time
from euler import *


GREEN = '\u001b[32m'
YELLOW = '\u001b[33m'
RED = '\u001b[31m'
RESET = '\x1b[0m' 

solutions = { 1: (      233168,  0.001),
              2: (     4613732, 0.0001),
              3: (        6857,    0.1),
              4: (      906609,   0.01),
              5: (   232792560,  0.001),
              6: (    25164150,  0.001),
              7: (      104743,   0.01),
              8: ( 23514624000,   0.01),
              9: (    31875000,    0.1),
             10: (142913828922,   0.05),
             11: (    70600674,    0.1),
             12: (    76576500,    0.5),
             13: (  5537376230,  0.001),
             14: (      837799,    2.0),
             15: (137846528820, 0.0005),
             16: (        1366, 0.0001),
             17: (       21124,   0.01),
             18: (        1074,    0.1),
             19: (         171,  0.001),
             20: (         648, 0.0001)}


for key in solutions.keys():
    t1 = time()
    result = eval('problem_' + str(key))()
    taken = time() - t1

    if result == solutions[key][0]:
        print(f'Problem {key:2d}: \t {GREEN} Passed {RESET}', end = '')
        if taken > solutions[key][1]:
            print(f'\t {YELLOW} Suboptimal {RESET} took {taken:6.4f} seconds could take {solutions[key][1]:6.4f} seconds')
        else:
            print(f'\t {GREEN} Optimal {RESET} took {taken:6.4f} seconds')
    else:
        print(f'Problem {key:2d}: \t {RED} Failed {RESET}')