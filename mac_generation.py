import os
import matplotlib.pyplot as plt
import numpy as np

try:
    os.remove("/home/vertonger/Desktop/Учёба/Geant/test/build-test/itog.csv")
except OSError:
    pass

particles_nums = 2000

# cnt2 = 0

for i in range(128):
    energy = str(120+i*10)
    uniq_particles = []
    # try:
    #     os.remove("/home/vertonger/Desktop/Учёба/Geant/test/build-test/result_nt_dE(Z)_t1.csv")
    # except OSError:
    #     pass
    # try:
    #     os.remove("/home/vertonger/Desktop/Учёба/Geant/test/build-test/result_nt_dE(Z)_t0.csv")
    # except OSError:
    #     pass
    # try:
    #     os.remove("/home/vertonger/Desktop/Учёба/Geant/test/build-test/result_nt_dE(Z).csv")
    # except OSError:
    #     pass

    x = '/run/initialize\n'
    x += '/gun/particle gamma\n'
    x += '/gun/direction 0 0 -1\n'
    x += '\n\n/gun/energy ' + energy + ' keV'
    x += '\n/run/beamOn ' + str(particles_nums) 

    # cnt = 0

    with open('run1.mac', 'w') as f:
        f.write(x)

    os.system("cd /home/vertonger/Desktop/Учёба/Geant/test/build-test && ./exampleB1 run1.mac")

    result = ''
    
    with open ('/home/vertonger/Desktop/Учёба/Geant/test/build-test/result_nt_dE(Z)_t1.csv', 'r') as f:
        # text = f.read()
        # cnt += text.count('0,Crystal')
        lines = f.read().splitlines()
        for line in lines:
            if line[0] == '#': continue
            words = line.split(',')
            if line[0] == '0' and line[1] == 'Crystal':
                if line[2] in uniq_particles:
                    continue
                else:
                    uniq_particles.append(line[2])


        
    #     lines = f.read().splitlines()
    # for i in lines:
    #     if i['0'] == '#': continue
    #     result = i + ',' + energy
    with open ('/home/vertonger/Desktop/Учёба/Geant/test/build-test/result_nt_dE(Z).csv', 'r') as f:
        # text = f.read()
        # cnt += text.count('0,Crystal')
        lines = f.read().splitlines()
        for line in lines:
            if line[0] == '#': continue
            words = line.split(',')
            if line[0] == '0' and line[1] == 'Crystal':
                if line[2] in uniq_particles:
                    continue
                else:
                    uniq_particles.append(line[2])
    with open ('/home/vertonger/Desktop/Учёба/Geant/test/build-test/result_nt_dE(Z)_t0.csv', 'r') as f:
        # text = f.read()
        # cnt += text.count('0,Crystal')
        lines = f.read().splitlines()
        for line in lines:
            if line[0] == '#': continue
            words = line.split(',')
            if line[0] == '0' and line[1] == 'Crystal':
                if line[2] in uniq_particles:
                    continue
                else:
                    uniq_particles.append(line[2])
    cnt = len(uniq_particles)
    # result = energy + ',' + str(cnt/particles_nums) + '\n'
    result = energy + ',' + str(cnt) + '\n'
    with open('/home/vertonger/Desktop/Учёба/Geant/test/build-test/itog.csv', 'a') as f:
        f.write(result)    

x = []
y = []
with open('/home/vertonger/Desktop/Учёба/Geant/test/build-test/itog.csv', 'r') as f:
    lines = f.read().splitlines()
    for line in lines:
        words = line.split(',')
        x.append(int(words[0]))
        y.append(int(words[1])/100)
        
fig, ax = plt.subplots()
ax.plot(x,y)
plt.yscale("log")
# plt.xticks(np.linspace(0,500,250))
plt.grid()
ax.set(xlabel = 'Энергия, кэВ',
    ylabel = 'Эффективность, %',
    title = 'Заголовок')
# plt.show()
plt.savefig('graphic.png')
