import argparse
from time import time

def problem_1():
    return 

def problem_2():
    return

def problem_3():
    return 
    
def problem_4():
    return

def problem_5():
    return
        
def problem_6():
    return

def problem_7():
    return

def problem_8():
    return

def problem_9():
    return

def problem_10():
    return

def problem_11():
    return

def problem_12():
    return

def problem_13():
    return

def problem_14():
    return

def problem_15():
    return

def problem_15():
    return

def problem_16():
    return

def problem_17():
    return

def problem_18():
    return

def problem_19():
    return

def problem_20():
    return

def main():

    # Arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('--num', '-n', required = True, type=int, help='Problem number')
    args = parser.parse_args()

    dic = {}
    for i in range(101):
        dic[i + 1] = f'problem_{i+1}'
    
    t1 = time()
    result = eval(dic[args.num])()
    taken = time() - t1

    print(f'Problem {args.num}: got {result} in {taken:8.6f} seconds')

if __name__ == '__main__':
    main()