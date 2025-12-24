import numpy as np
import matplotlib.pyplot as plt

UINT16_MAX = 65535

# ------------------------
# Parameters (adjustable)
# ------------------------
m_a = 1                 # acceleration (speed units per ms)
m_maxSpeed = 1250
m_distance = 500      # arbitrary distance units
sim_time_ms = 10000      # simulation length
dt_ms = 1               # timestep

# ------------------------
# State variables
# ------------------------
time = []
speed = []
position = []
acceleration = []

m_lastSpeed = 0
m_lastTick = 0
pos = 0

# ------------------------
# Helper functions
# ------------------------
def posRampSpeed(lastSpeed, a, dt):
    # EXACT mirror of your code
    return min(lastSpeed + a * dt, UINT16_MAX)

def negRampSpeed(a, distance):
    return np.sqrt(2 * a * 1000 * distance)

# ------------------------
# Simulation loop
# ------------------------
oldPos = 0
for t in range(0, sim_time_ms, dt_ms):
    dt = dt_ms

    speedPos = posRampSpeed(m_lastSpeed, m_a, dt)
    speedNeg = negRampSpeed(m_a, np.abs(m_distance - oldPos)) * np.sign(m_distance - oldPos)

    v = min(speedPos, speedNeg, m_maxSpeed)

    # Integrate
    pos += v * dt / 1000.0  # convert ms → s for position
    oldPos = pos
    a_inst = (v - m_lastSpeed) / dt if dt > 0 else 0

    # Save
    time.append(t / 1000.0)  # seconds
    speed.append(v)
    position.append(pos)
    acceleration.append(a_inst)

    # Update state
    m_lastSpeed = v
    m_lastTick += dt

    if pos >= m_distance:
        break

# ------------------------
# Plotting
# ------------------------
plt.figure(figsize=(10, 8))

plt.subplot(3, 1, 1)
plt.plot(time, position)
plt.ylabel("s(t) position")
plt.grid(True)

plt.subplot(3, 1, 2)
plt.plot(time, speed)
plt.ylabel("v(t) speed")
plt.grid(True)

plt.subplot(3, 1, 3)
plt.plot(time, acceleration)
plt.ylabel("a(t) acceleration")
plt.xlabel("Time [s]")
plt.grid(True)

plt.tight_layout()
plt.show()
