import os
import csv

os.system('g++ -o prog main.cpp -fopenmp')
print('prog.exe compiled.')

data = []

output = os.popen('prog.exe').read()
print(output, end='')
lines = output.split('\n')

print('program finished.')

for line in lines:
    if len(line) <= 1:
        continue
    temp = line.split()
    row = []
    row.append((5.0 / 9.0) * (float(temp[5]) - 32))
    row.append(float(temp[7]) * 2.45)
    row.append(float(temp[9]) * 2.45)
    row.append(temp[11])
    row.append(temp[13])
    data.append(row)

with open('output.csv', 'w', newline='') as file:
    writer = csv.writer(file)
    for row in data:
        writer.writerow(row)

print('data exported to output.csv')
