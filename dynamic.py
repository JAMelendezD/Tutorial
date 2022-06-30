from time import time

'''
Here we are going to see two more examples about dynamic
programing and how to use dictionaries to improve our
algorithms.

The example is going to be about one of the many open
questions in math known as the Collatz conjecture which is
very simple. Then you will have to solve an exercise related
to the fibonacci sequence.
'''


'''
The Collatz conjecture can be explained as follows. Pick an integer number
let say 3. Now the algorithm says if your number is even divide it by
two, it it is odd then multiply it by 3 and add 1. Lets try it on our
number

3 is odd -> 3 * 3 + 1 = 10
10 is even -> 10 // 2 = 5
5 is odd -> 3 * 5 + 1 = 16
16 is even -> 16 // 2 = 8
8 is even -> 8 // 2 = 4
4 is even -> 4 // 2 = 2
2 is even -> 2 // 2 = 1

Once we reach 1 the algorithm stops. The conjecture consists that for any number
you pick this process will end up at 1 in a finite number of steps. The sequence 
we generated from 3 was

3 -> 5 -> 16 -> 8 -> 4 -> 2 -> 1

This produced a chain of size 7.
'''


# Alright lets implement it


def collatz_recursive(n, size):
    '''
    Recursive collatz algorithm
    '''
    if n == 1:
        print(f'{n}') # Extreme case 
        return size
    else:
        if n % 2 == 0:
            print(f'{n}->', end='') # we use end = '' so we dont get a new line
            size += 1
            return collatz_recursive(n // 2, size)

        else:
            print(f'{n}->', end='')
            size += 1
            return collatz_recursive(3 * n + 1, size)


# In our output we get the expected chain and the size of it
# Note: Uncomment the lines you want to excecute as you go you can also comment the line you dont what to excecute 

#size = collatz_recursive(3, 0)
#print(size)

'''
Before we complicate the problem lets see a non recursive implementation
since we know from experience recursion can be quite slow
'''

def collatz_naive(n):
    '''
    Non recursive non dynamic approach to Collatz
    '''
    size = 0
    i = n
    while i != 1: # Case when we should break
        if i % 2 == 0:
            print(f'{i}->', end='')
            i = i // 2
            size += 1
        else:
            print(f'{i}->', end='')
            i = 3 * i + 1
            size += 1
    print(f'{1}')
    return size

#size = collatz_naive(3)
#print(size)

'''
Lets make a function to measure and print the time to evaluate the performance
of our algorithms
'''

print(80*'#')

def time_func(func, *args):
    t1 = time()
    result = func(*args)
    print(f'Time: {time() - t1:6.4f} s')
    print(f'Result: ', result)

#time_func(collatz_recursive, 8377999413116613164973131316549, 0)
#time_func(collatz_naive, 8377999413116613164973131316549)

'''
We see again that the naive approach is slighly faster than our recursion
but let us know complicate the problem a little bit more to better evaluate the
performance. We want to know what number below a threshold produces the longest
chain. 
'''

def largest_chain(threshold):
    '''
    Calculates the largest Collatz chain up to a threshold
    '''
    largest = 0 # Assume the largest is 0 
    for i in range(1, threshold): # Go over all numbers up to the threshold (naive no recursion)
        n = i
        size = 0
        while n != 1:
            if n % 2 == 0:
                n  = n // 2
            else:
                n = 3 * n + 1
            size += 1 
        if size > largest: # we found even a larger one
            largest = size # modify largest to this larger one
            target = i # change the target to the number that seems to have the largest one
    return target

print(80*'#')

#time_func(largest_chain, 100)
#time_func(largest_chain, 1_000)
#time_func(largest_chain, 10_000)
#time_func(largest_chain, 100_000)
#time_func(largest_chain, 1_000_000)

'''
It is taking nearly 10 seconds to find the answer for 1 million !!! and this
is suppose to be our fastest approach. Let see what we can do if we add
a dictionary and use dynamic programming. The reason the program becomes slow
is that for every number we have to go down along the entire sequence until we 
reach 1. However notice that for 3 we had

3 -> 5 -> 16 -> 8 -> 4 -> 2 -> 1

Which has size 7 but not only that we also now that if we ever get to 5 regardless
of were we came from the sequence will be 

5 -> 16 -> 8 -> 4 -> 2 -> 1

and has size 6. We also know that if we reach 8 we need 3 more steps to get to 1.
In summary when computing 3 we also get all the following information

number      steps to get to 1

3               6
5               5
16              4
8               4
2               1


This reduces the number of computations significantly since for large numbers
we probably have already computed most of the sequence and instead of doing
all 1 million iterations we migh need to do a few hundred or less. So the main
idea is to create a dictionary and save the number as the key and the number of
steps to one as the value. Then we can count the size and then if we find a number
we had visited we go straight to 1 and add the value of the dictioanry to the size.
'''


def largest_chain_dyn(threshold):
    '''
    Calculates the largest Collatz chain up to a threshold using a memory
    of the previous visited values and their sizes
    '''
    largest = 0
    dic = {}
    for i in range(1, threshold):
        n = i
        size = 0
        tmp = [] # We need a temporary list to later fill our dictionary 
        while n != 1:
            tmp.append(n)
            try: # see if n is in the dictionary
                size += dic[n] # if it is then we add the remaining steps to size
                n = 1 # We go straight down to 1
            except:
                if n % 2 == 0: # was not in dictionary do the regular procedure
                    n = n // 2
                else:
                    n = 3 * n + 1
                size += 1 # add one to our size

        for index in range(len(tmp)): # update the dictionary with all the new numbers we explored
            dic[tmp[index]] = size - index

        if size > largest:
            largest = size
            target = i
    return target

#time_func(largest_chain_dyn, 100)
#time_func(largest_chain_dyn, 1_000)
#time_func(largest_chain_dyn, 10_000)
#time_func(largest_chain_dyn, 100_000)
#time_func(largest_chain_dyn, 1_000_000)

'''
We have now reduce the time to nearly 1 second for 1 million. There are
two more optimizations based on the math that can take the time barely
under a second (this is left to the reader).
'''

print(80*'#')


'''
The exercise is to do something similar to the fibonacci sequence
which consist of the following it starts at 0 which value is 0 then the value for
one is one then fibonacci of two is the sum of the previous number in the sequence
so

sequence = 0, 1

fib(2) = 1 + 0 = 1

Then our enlarged sequence is 

sequence = 0, 1, 1

fib(3) = 1 + 1 = 2

sequence = 0, 1, 1, 2

fib(4) = 2 + 1 = 3

sequence = 0, 1, 1, 2, 3


The first 10 numbers of the sequence are

0, 1, 1, 2, 3, 5, 8, 13, 21, 34

- Write down the recursive version and compute fibonacci of 30 also see how long it takes.
- Write down the naive and the dynamic versions see which one is faster
- What is the maximum fibonacci number you can compute with the dynamic version in less
time that it take the recursive to compute fibonacci of 30?

'''