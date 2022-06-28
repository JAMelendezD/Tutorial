from time import time

def fib(n):
    if n == 1 or n == 0:
        return n
    else:
        return fib(n-1) + fib(n-2)

def fib_one_line(n):
    return 1 if n in (0, 1) else fib(n - 1) + fib(n - 2)

def fib_dyn_list(n):
    if n == 0: return [0]
    if n == 1: return [0, 1]
    seq = fib_dyn_list(n - 1)
    return seq + [seq[-1] + seq[-2]]

def fib_dyn_dic(n):
    dic = {0: 0, 1: 1}
    for i in range(2, n+1):
        value = dic[i - 1] + dic[i - 2]
        dic[i] = value 
    return dic

def time_func(func, num):
    t1 = time()
    result = func(num)
    print(f'Time: {time() - t1:6.4f} s')
    print(f'Result: ', result)

time_func(fib, 40)
time_func(fib_one_line, 40)
time_func(fib_dyn_list, 40)
time_func(fib_dyn_dic, 40)

t1 = time()
fib_dyn_list(500)
print(time() - t1)

t1 = time()
fib_dyn_dic(500)
print(time() - t1)