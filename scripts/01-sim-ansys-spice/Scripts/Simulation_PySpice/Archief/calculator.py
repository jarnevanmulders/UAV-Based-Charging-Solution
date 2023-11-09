import math

FREQ = 6.78e6

L1 = 2.131e-6
L2 = 1.155e-6
R1 = 0.24617
R2 = 0.28147
k = 0.044284

#RLOAD = 3.17 @ u_Ω
#RS = 1e-3 @ u_Ω

C1 = (2 * math.pi * FREQ)**-2/L1
print(C1*1e12)
C2 = (2 * math.pi * FREQ)**-2/L2
print(C2*1e12)

def efficiency_calculation(l1, r1, l2, r2, load, k):
    qt = 2 * math.pi * FREQ * l1 / r1
    qr = 2 * math.pi * FREQ * l2 / (r2 + load)
    comb = k ** 2 * qt * qr
    return load / (load + r2) * comb / (1 + comb)


def optimal_load_calculation(l1, r1, l2, r2, k):
    qt = 2 * math.pi * FREQ * l1 / r1
    q2 = 2 * math.pi * FREQ * l2 / r2
    return math.sqrt(r2 ** 2 * (1 + k ** 2 * qt * q2))

RL = optimal_load_calculation(L1, R1, L2, R2, k)
print(RL)



PL = 50

QT = 2 * math.pi * FREQ * L1 / R1
QR = 2 * math.pi * FREQ * L2 / (R2 + RL)

Vs = math.sqrt(PL / (RL * QT * QR * k ** 2) * (R1 * (R2 + RL) * (1 + k ** 2 * QT * QR) ** 2))
print(Vs)