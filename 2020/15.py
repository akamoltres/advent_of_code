
import sys

def solution(target):
    fin = open(sys.argv[1])
    start_seq = [int(x) for x in fin.read().split(",")]

    last_used = {}
    prev_num = 0

    for i,n in enumerate(start_seq):
        last_used[n] = i + 1
        prev_num = n

    for i in range(len(start_seq) + 1, target + 1):
        if not prev_num in last_used or last_used[prev_num] == i - 1:
            last_used[prev_num] = i - 1
            prev_num = 0
        else:
            prev_num_cache = prev_num
            prev_num = (i - 1) - last_used[prev_num]
            last_used[prev_num_cache] = i - 1

    return prev_num

print("Part 1:", solution(2020))
print("Part 2:", solution(30000000))

