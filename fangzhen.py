import numpy as np
import matplotlib.pyplot as plt

# 系统参数
k12 = 0.5
k21 = 0.5

A = np.array([[0, 1, 0, 0],
              [-k12, 0, k12, 0],
              [0, 0, 0, 1],
              [k21, 0, -k21, 0]])

B = np.array([[0, 0],
              [1, 0],
              [0, 0],
              [0, 1]])

# 初始条件
x0 = np.array([0, 0, 0, 0])  # 初始位置和速度均为零
u = np.array([0, 0])         # 控制输入为零

# 时间设定
t_max = 10   # 总时间 10秒
dt = 0.01    # 时间步长
time_steps = int(t_max / dt)
time = np.linspace(0, t_max, time_steps)

# 状态变量
x = np.zeros((time_steps, 4))
x[0] = x0

# 欧拉法解状态方程
for i in range(1, time_steps):
    dx = A @ x[i-1] + B @ u
    x[i] = x[i-1] + dx * dt

# 提取位置和速度
p1 = x[:, 0]
v1 = x[:, 1]
p2 = x[:, 2]
v2 = x[:, 3]

# 绘图
plt.figure(figsize=(12, 6))

# 位置和速度的变化
plt.subplot(2, 1, 1)
plt.plot(time, p1, label='p1 (Position 1)')
plt.plot(time, p2, label='p2 (Position 2)')
plt.xlabel('Time [s]')
plt.ylabel('Position')
plt.title('Position vs Time')
plt.legend()

plt.subplot(2, 1, 2)
plt.plot(time, v1, label='v1 (Velocity 1)')
plt.plot(time, v2, label='v2 (Velocity 2)')
plt.xlabel('Time [s]')
plt.ylabel('Velocity')
plt.title('Velocity vs Time')
plt.legend()

plt.tight_layout()
plt.show()
