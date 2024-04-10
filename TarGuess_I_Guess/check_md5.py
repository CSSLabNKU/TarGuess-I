import hashlib
fin=open('output_xiaomi_targuess1.txt','r') # output guess
gin=open('xiaomi_PI.csv') # test set

def calHash(guess,salt):
    m = hashlib.md5(guess).hexdigest()
    rtn = hashlib.md5(m+""+salt).hexdigest()
    return rtn

crack = [0 for i in range(1010)]
tot = 0.0
for line in gin:
    tot += 1
    line = line.strip('\n')
    psw = line.split('\t')[1]
    h = psw[0:32]
    salt = psw[33:]
    flag = 0
    for i in range(1000):
        guess_line = fin.readline()
        guess_line = guess_line.strip('\n')
        guess = guess_line.split('\t')[0]
        hguess = calHash(guess,salt)
        if h == hguess and flag == 0:
            if flag == 1:
                print line, guess_line,i
            crack[i] += 1
            flag = 1
print tot
sum = 0
for i in range(1000):
    sum += crack[i]
    print i + 1, sum / tot
