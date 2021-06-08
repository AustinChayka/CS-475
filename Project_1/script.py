import subprocess
import csv

threadRange = [1, 2, 4, 8]
trialRange = [100000, 200000, 300000, 400000, 500000, 1000000]

megaTrials = []

for threads in threadRange:
    for trials in trialRange:
        output = subprocess.run('prog.exe ' + str(threads) + ' ' + str(trials), capture_output=True, text=True)
        print(output.stderr, end='')
        megaTrials.append(output.stderr.split()[-1])
        
else:
    print('tests finished.')

    data = []
    trialRange.insert(0, None)
    data.append(trialRange)
    for i in range(len(threadRange)):
        temp = []
        temp.append(threadRange[i])
        for j in range(len(trialRange) - 1):
            temp.append(megaTrials[j + i * (len(trialRange) - 1)])
        data.append(temp)
                
    with open('output.csv', 'w', newline='') as file:
        writer = csv.writer(file)
        for row in data:
            writer.writerow(row)

    print('data exported to output.csv')
        
