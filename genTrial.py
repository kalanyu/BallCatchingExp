from random import randint

variation = [1,2,4,5,6,8,9];
trial_no = raw_input('Input total trial number for each stimulus value\n')
print 'total trial number = ' + str(int(trial_no) * len(variation))

count_list = [int(trial_no)] * len(variation);
print sum(count_list)
count = 1
with open('trial_file.csv','w') as f:
    while sum(count_list) != 0:
        index = randint(0,len(variation)-1)
        if count_list[index] == 0:
            continue
        print count
        count += 1
        count_list[index] -= 1
f.closed
