import os
import csv

num_elements = [16, 32, 64, 128]
local_sizes = [2, 4, 6, 8]

for size in local_sizes:
    for num in num_elements:

        output = os.popen('First.exe ' + str(size) + ' ' + str(num)).read()
        print(output, end='')
    
print('program finished.')
