LENGTH = 1000
HIGH = 25

f = open('C:\\Users\\John\\source\\repos\\mario\\mario\\map.txt', 'w')
for y in range(HIGH):
    for x in range(LENGTH):
        if y == 0:
            f.write('=')
        elif y == HIGH - 1 and x % 50 == 10:
            f.write('^')
        elif y == HIGH - 1:
            f.write('=')
        elif x == 0:
            f.write('|')
        elif x == LENGTH - 1:
            f.write('|')
        elif x % 50 == 0 and (y == HIGH - 2 or y == HIGH - 3):
            f.write('|')
        elif y == HIGH - 4 and (x % 10 == 24 or x % 50 == 25 or x % 50 == 26):
            f.write('=')
        else:
            f.write('e')
    f.write('\n')
f.close()
