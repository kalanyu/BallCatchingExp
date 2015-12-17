from random import randint
from datetime import datetime

variation = ['1.0e-01','2.0e-01','4.0e-01','5.0e-01','6.0e-01','8.0e-01','9.0e-01'];
trial_no = raw_input('Input total trial number for each stimulus value\n')
print 'total trial number = ' + str(int(trial_no) * len(variation))

filename = raw_input('Input file name\n')

count_list = [int(trial_no)] * len(variation);
print sum(count_list)
count = 1
# 1.000000e+00,-9.800000e+00,5.000000e-01,0.000000e-01,0.000000e-02,-10.000000e-01
with open(filename,'w') as f:
    while sum(count_list) != 0:
        index = randint(0,len(variation)-1)
        if count_list[index] == 0:
            continue
        else:
            count_list[index] -= 1
            order = True if randint(0,10) >= 5 else False

            for i in range(2):
                f.write(str(count) + '.0e+00   ') #trial count
                f.write('-9.80e+00   ') #fall acceleration
                f.write((variation[index] if order else '5.0e-01') + '   ') #stimulus weight
                f.write('0.00e+00   ') #height
                f.write('0.00e-02   ') #height shift
                f.write('-10.00e-01\n') #height disappear
                count += 1
                order = not order
f.closed
