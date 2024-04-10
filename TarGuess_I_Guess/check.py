# py2
import hashlib
fin = open('output_dodonew_targuess1_buquchong.txt','r') # output guess
gin = open('dodonew_PI_49775.csv') # test set


crack = [0 for i in range(1010)]
tot = 0.0
for line in gin:
    tot += 1
    line = line.strip('\n');
    psw = line.split('\t')[1];
    flag = 0
    for i in range(1000):
        guess_line = fin.readline()
        guess_line = guess_line.strip('\n')
        guess = guess_line.split('\t')[0]
        if psw == guess and flag == 0:
            if flag == 1:
                print line, guess_line,i
            crack[i] += 1
            flag = 1
print tot
sum = 0
for i in range(1000):
    sum += crack[i]
    print i + 1, sum / tot
