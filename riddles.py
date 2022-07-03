import numpy as np

GREEN = '\u001b[32m'
RED = '\u001b[31m'
RESET = '\x1b[0m' 

def prison_random(n):
    boxes =  np.random.permutation(n)
    prisoners = np.random.permutation(n)
    dic_prisoners = {}
    dic_boxes = {}
    for i in range(n):
        dic_prisoners[i] = boxes[i]
        dic_boxes[i] = prisoners[i]

    sum_ = 0
    for i in range(n):
        sequence = np.random.permutation(n)[:n // 2]
        for attempt in sequence:
            if dic_boxes[attempt] == dic_prisoners[i]:
                sum_ += 1
                break
    
    if sum_ == n:
        return 'Alive'
    else:
        return 'Dead'


def prison_paths(n):
    boxes =  np.random.permutation(n)
    prisoners = np.random.permutation(n)
    dic_prisoners = {}
    dic_boxes = {}
    for i in range(n):
        dic_prisoners[i] = boxes[i]
        dic_boxes[i] = prisoners[i]

    sum_ = 0
    for i in range(n):
        attempts = 0
        attempt = dic_prisoners[i]
        while attempts < n // 2:
            if dic_boxes[attempt] == dic_prisoners[i]:
                sum_ += 1
                break
            attempt = dic_boxes[attempt]
            attempts += 1

    if sum_ == n:
        return 'Alive'
    else:
        return 'Dead'

def main():

    simulations = 1_000_000
    num_prisoners = 14
    mode = 0

    p = 0
    counter = 0
    print()
    for i in range(simulations):
        if mode == 0:
            outcome = prison_paths(num_prisoners)
        else:
            outcome = prison_random(num_prisoners)
        if outcome == 'Alive':
            p += 1
        counter += 1
        print('\033[FSimulation # {:7d} alive {:6.4f} % of the time'.format(i + 1, p/counter * 100))

if __name__ == '__main__':
    main()