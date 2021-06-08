import os
import csv

num_elements = [16, 32, 64, 128]

for num in num_elements:

        output = os.popen('First.exe ' + str(num)).read()
        print(output, end='')
    
print('program finished.')
