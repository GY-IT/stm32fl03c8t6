% 1. 定义离散系统的矩阵 A 和 B
A = [1 1; 0 1];    % 2x2 状态矩阵 (位置和速度)
B = [0.5; 1];      % 2x1 控制输入矩阵
% % 2. 定义加权矩阵 Q 和 R
Q = [10 0; 0 1];    % 状态代价矩阵 (2x2)
Qh = [1 0 ; 0 1];
R = 1;              % 控制输入代价矩阵 (标量)
% 3. 定义半个周期正弦目标轨迹 x_d
N = 50;             % 时间步数
x_d = zeros(2, N);  % 初始化目标轨迹
t = (0:N-1);        % 时间步
x_d(1, :) = sin(pi * t / (N-1));   % 目标轨迹 x1 (横坐标)
x_d(2, :) = (pi * t / (N-1));   % 目标轨迹 x2 (纵坐标)
% 4. 离散LQT的初始化
P = zeros(2, 2*(N+1));              % 最优代价矩阵初始化为Q
K = zeros(1, 2*N);    % 控制输入矩阵
J = zeros(1, 1*N);

% 5. 动态规划求解最优输入矩阵 K 和最优代价矩阵 P
P(:,2*(N+1)-1:2*(N+1)) = Qh;

for i = N:-1:1
    j = i + 1;
    % 计算增益矩阵 K 和代价矩阵 P
    P(:,2*i-1:2*i) = Q + A' * P(:,2*j-1:2*j) * A - A' * P(:,2*j-1:2*j) * B * inv(R + B' * P(:,2*j-1:2*j) * B) * B' * P(:,2*j-1:2*j) * A; % 更新代价矩阵 P
    K(:,2*i-1:2*i) = (R + B' * P(:,2*j-1:2*j) * B) \ (B' * P(:,2*j-1:2*j) * A); % 计算控制输入矩阵 K 
end

% 6. 计算最优控制输入和状态轨迹
x0 = [1; 0];  % 初始状态 (x, y)
x = zeros(2, N);  % 状态矩阵
u = zeros(1, N);  % 控制输入矩阵
x(:, 1) = x0;     % 设置初始状态

for k = 1:N-1
    u(:, k) = -K(:, 2*k-1:2*k) * (x(:, k) - x_d(:, k)); % 计算最优控制输入
    x(:, k+1) = A * x(:, k) + B * u(:, k); % 更新状态
    J(:, k) = (x(:, k) - x_d(:, k))'*Q*(x(:, k) - x_d(:, k))+ u(:, k)'*R* u(:, k)
end

% 7. 绘制目标轨迹和运动轨迹
figure;
subplot(2,1,1);
plot(t, x_d(1,:), 'k--', 'LineWidth', 2); % 绘制目标轨迹x_d (x1)
hold on;
plot(t, x_d(2,:), 'k--', 'LineWidth', 2); % 绘制目标轨迹x_d (x2)
xlabel('Time Step (k)');
ylabel('Target State Values');
legend('x_d (Target x)', 'y_d (Target y)');
title('Target Trajectory (Half Sine Wave)');
grid on;

subplot(2,1,2);
plot(t, x(1,:), 'r', 'LineWidth', 2); % 绘制x1的响应
hold on;
plot(t, x(2,:), 'b', 'LineWidth', 2); % 绘制x2的响应
xlabel('Time Step (k)');
ylabel('State Values');
legend('x (Position)', 'y (Velocity)');
title('State Trajectory (Motion Trajectory)');
grid on;
