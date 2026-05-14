import numpy as np
import matplotlib.pyplot as plt

UINT16_MAX = 65535

# Parameters
m_a = 50
m_maxSpeed = 5000
m_distance = 2000
sim_time_ms = 10000
dt_ms = 0.2

t = np.arange(0, sim_time_ms, dt_ms)
n = len(t)

speed = np.zeros(n)
position = np.zeros(n)
acceleration = np.zeros(n)

m_lastSpeed = 0.0
pos = 0.0

for i in range(n):
    remaining = m_distance - pos

    speed_pos = min(m_lastSpeed + m_a * dt_ms, UINT16_MAX)
    speed_neg = np.sqrt(2 * m_a * 1000 * np.abs(remaining)) * np.sign(remaining)

    v = min(speed_pos, speed_neg, m_maxSpeed)

    pos += v * dt_ms / 1000.0

    speed[i] = v
    position[i] = pos
    
    m_lastSpeed = v

    if pos >= m_distance:
        speed = speed[:i+1]
        position = position[:i+1]
        acceleration = acceleration[:i+1]
        t = t[:i+1]
        break


acceleration[1:] = (speed[1:] - speed[:-1]) / dt_ms
acceleration[0] = 0

# ------------------------
# Plotting
# ------------------------
plt.figure(figsize=(10, 8))

plt.subplot(3, 1, 1)
plt.plot(t, position)
plt.ylabel("s(t) position")
plt.grid(True)

plt.subplot(3, 1, 2)
plt.plot(t, speed)
plt.ylabel("v(t) speed")
plt.grid(True)

plt.subplot(3, 1, 3)
plt.plot(t, acceleration)
plt.ylabel("a(t) acceleration")
plt.xlabel("Time [ms]")
plt.grid(True)

plt.tight_layout()
plt.show()
