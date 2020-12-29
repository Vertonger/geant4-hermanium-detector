particles_nums = 10
x = '/run/initialize\n/gun/particle gamma'
for i in range(1080):
    x += '\n\n/gun/energy ' + str(i+120) + ' keV'
    x += '\n/run/beamOn ' + str(particles_nums)
    x += '\n/analysis/setFileName ' + str(i+120)

with open('run1.mac', 'w') as f:
    f.write(x)