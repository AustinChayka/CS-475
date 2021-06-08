import os
import csv

os.system('g++ -o prog main.cpp -fopenmp -msse')
print('prog.exe compiled.')

sizes = [500000, 1000000, 5000000, 10000000, 50000000, 100000000]
data = []

for size in sizes:
    output = os.popen('prog.exe ' + str(size)).read()
    print(output)
    output = output.split()
    temp = []
    temp.append(size)
    temp.append(output[5])
    temp.append(output[11])
    temp.append(output[13])
    data.append(temp)
    
print('program finished.')

with open('output.csv', 'w', newline='') as file:
    writer = csv.writer(file)
    for row in data:
        writer.writerow(row)

print('data exported to output.csv')
