import os
import csv

os.system('g++ -o prog main.cpp -fopenmp')
print('prog.exe compiled.')

threadRange = [1, 2, 4, 8]
nodeRange = [200, 300, 500, 750, 1000, 2000, 3000, 5000]

megaHeights = []

for threads in threadRange:
    for nodes in nodeRange:
        output = os.popen('prog.exe ' + str(threads) + ' ' + str(nodes)).read()
        print(output)
        megaHeights.append(output.split()[10])
        
else:
    print('tests finished.')

    data = []
    nodeRange.insert(0, None)
    data.append(nodeRange)
    for i in range(len(threadRange)):
        temp = []
        temp.append(threadRange[i])
        for j in range(len(nodeRange) - 1):
            temp.append(megaHeights[j + i * (len(nodeRange) - 1)])
        data.append(temp)
                
    with open('output.csv', 'w', newline='') as file:
        writer = csv.writer(file)
        for row in data:
            writer.writerow(row)

    print('data exported to output.csv')
