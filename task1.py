import numpy as np
import math
import matplotlib.pyplot as plt
import imageio
 
# 车辆模型
class Vehicle:
    def __init__(self, x=0.0, y=0.0, theta=0.0, v=0.0):
        self.x = x
        self.y = y
        self.theta = theta  # 航向角
        self.v = v          # 速度
 
    def update(self, a, omega, dt):
        """
        更新车辆状态
        a: 加速度
        omega: 角速度
        dt: 时间步长
        """
        self.x += self.v * np.cos(self.theta) * dt
        self.y += self.v * np.sin(self.theta) * dt
        self.theta += omega * dt
        self.v += a * dt
 
# 轨迹
class Trajectory:
    def __init__(self):
        self.cx = np.linspace(0, 50, 500)
        self.cy = [np.sin(ix / 5.0) * ix / 2.0 for ix in self.cx]
        self.theta = [np.arctan2(self.cy[i+1] - self.cy[i], self.cx[i+1] - self.cx[i]) if i < len(self.cx)-1 else 0.0 for i in range(len(self.cx))]
        self.v = np.full_like(self.cx, 3.0)  # 目标速度为3 m/s
 
    def get_reference(self, index):
        """
        获取参考轨迹点
        """
        return np.array([self.cx[index], self.cy[index], self.theta[index], self.v[index]])
 
# iLQR控制器
class iLQRController:
    def __init__(self, N=50, max_iter=10, dt=0.1):
        self.N = N  # 控制时域长度
        self.max_iter = max_iter  # 最大迭代次数
        self.dt = dt  # 时间步长
        self.Q = np.diag([1.0, 1.0, 0.5, 0.1])  # 状态权重矩阵
        self.R = np.diag([0.1, 0.1])  # 控制权重矩阵
        self.Qf = self.Q * 10  # 终端状态权重矩阵
 
    def ilqr(self, vehicle, trajectory, index):
        """
        使用iLQR计算最优控制序列
        """
        # 初始化状态和控制序列
        x_dim = 4  # 状态维度 [x, y, theta, v]
        u_dim = 2  # 控制维度 [a, omega]
        xs = np.zeros((self.N + 1, x_dim))
        us = np.zeros((self.N, u_dim))
        
        # 初始状态
        xs[0] = np.array([vehicle.x, vehicle.y, vehicle.theta, vehicle.v])
        
        # 初始猜测控制序列（全零）
        us = np.zeros((self.N, u_dim))
        
        for iteration in range(self.max_iter):
            # 前向传播，计算状态轨迹
            for k in range(self.N):
                xs[k+1] = self.dynamics(xs[k], us[k], self.dt)
            
            # 计算代价函数梯度和Hessian矩阵
            Vx = self.Qf @ (xs[-1] - trajectory.get_reference(index + self.N))
            Vxx = self.Qf
            k_list = []
            K_list = []
            for k in reversed(range(self.N)):
                xk = xs[k]
                uk = us[k]
                x_ref = trajectory.get_reference(index + k)
                # 计算状态和控制的偏导数
                fx, fu = self.linearize_dynamics(xk, uk, self.dt)
                lx = self.Q @ (xk - x_ref)
                lu = self.R @ uk
                lxx = self.Q
                luu = self.R
                lux = np.zeros((u_dim, x_dim))
                # 计算Q函数的二次近似
                Qx = lx + fx.T @ Vx
                Qu = lu + fu.T @ Vx
                Qxx = lxx + fx.T @ Vxx @ fx
                Quu = luu + fu.T @ Vxx @ fu
                Qux = lux + fu.T @ Vxx @ fx
                # 计算控制增量
                Quu_inv = np.linalg.inv(Quu + np.eye(u_dim) * 1e-6)  # 加小值防止矩阵奇异
                k = -Quu_inv @ Qu
                K = -Quu_inv @ Qux
                # 更新V函数
                Vx = Qx + K.T @ Quu @ k + K.T @ Qu + Qux.T @ k
                Vxx = Qxx + K.T @ Quu @ K + K.T @ Qux + Qux.T @ K
                # 存储k和K
                k_list.insert(0, k)
                K_list.insert(0, K)
            # 更新控制序列并前向模拟
            x_new = np.copy(xs[0])
            xs_new = [x_new]
            us_new = []
            for k in range(self.N):
                du = k_list[k] + K_list[k] @ (x_new - xs[k])
                us_new.append(us[k] + du)
                x_new = self.dynamics(x_new, us_new[-1], self.dt)
                xs_new.append(x_new)
            xs = np.array(xs_new)
            us = np.array(us_new)
            # 判断收敛性
            cost = self.compute_total_cost(xs, us, trajectory, index)
            print(f"Iteration {iteration}, Cost: {cost}")
            if cost < 1e-6:
                break
        return us[0]  # 返回当前时刻的最优控制
 
    def dynamics(self, x, u, dt):
        """
        动力学模型
        """
        x_next = np.zeros_like(x)
        x_next[0] = x[0] + x[3] * np.cos(x[2]) * dt  # x
        x_next[1] = x[1] + x[3] * np.sin(x[2]) * dt  # y
        x_next[2] = x[2] + u[1] * dt                 # theta
        x_next[3] = x[3] + u[0] * dt                 # v
        return x_next
 
    def linearize_dynamics(self, x, u, dt):
        """
        线性化动力学模型，返回状态和控制的雅可比矩阵
        """
        fx = np.eye(4)
        fx[0, 2] = -x[3] * np.sin(x[2]) * dt
        fx[0, 3] = np.cos(x[2]) * dt
        fx[1, 2] = x[3] * np.cos(x[2]) * dt
        fx[1, 3] = np.sin(x[2]) * dt
        fx[2, 2] = 1.0
        fx[3, 3] = 1.0
 
        fu = np.zeros((4, 2))
        fu[2, 1] = dt  # theta 对 omega 的偏导
        fu[3, 0] = dt  # v 对 a 的偏导
 
        return fx, fu
 
    def compute_total_cost(self, xs, us, trajectory, index):
        """
        计算总的代价函数
        """
        cost = 0.0
        for k in range(self.N):
            xk = xs[k]
            uk = us[k]
            x_ref = trajectory.get_reference(index + k)
            dx = xk - x_ref
            cost += dx.T @ self.Q @ dx + uk.T @ self.R @ uk
        # 终端代价
        dx = xs[-1] - trajectory.get_reference(index + self.N)
        cost += dx.T @ self.Qf @ dx
        return cost
 
# 主函数
def main():
    vehicle = Vehicle()
    trajectory = Trajectory()
    controller = iLQRController(N=50, max_iter=10, dt=0.1)
    dt = 0.1
    x_history = []
    y_history = []
    total_time = len(trajectory.cx) - controller.N - 1
 
    # 创建图形
    fig, ax = plt.subplots()
    frames = []
 
    for t in range(total_time):
        # 获取当前最优控制
        u_opt = controller.ilqr(vehicle, trajectory, t)
        # 更新车辆状态
        vehicle.update(u_opt[0], u_opt[1], dt)
        # 记录轨迹
        x_history.append(vehicle.x)
        y_history.append(vehicle.y)
 
        # 绘制
        ax.cla()
        ax.plot(trajectory.cx, trajectory.cy, "-r", label="Reference Trajectory")
        ax.plot(x_history, y_history, "-b", label="Vehicle Trajectory")
        ax.set_xlim(0, 50)
        ax.set_ylim(-20, 25)
        ax.set_title(f"iLQR Trajectory Tracking - Step {t}")
        ax.set_xlabel("x [m]")
        ax.set_ylabel("y [m]")
        ax.grid(True)
 
        # 渲染当前帧
        fig.canvas.draw()
        image = np.frombuffer(fig.canvas.tostring_rgb(), dtype='uint8').reshape(fig.canvas.get_width_height()[::-1] + (3,))
        frames.append(image)
 
        # 实时显示
        plt.pause(0.001)
 
    # 保存为GIF
    imageio.mimsave('ilqr_trajectory_tracking.gif', frames, fps=10)
    plt.show()
 
if __name__ == '__main__':
    main()




#https://blog.csdn.net/a8598671/article/details/142320166
