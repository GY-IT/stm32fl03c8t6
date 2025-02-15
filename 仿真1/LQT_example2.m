% 参数设置
T = 1;         % 离散时间步长
m = 1;         % 小车质量
N = 50;        % 仿真步数
Q = diag([10, 1]);  % 状态误差权重矩阵
R = 1;         % 控制输入权重
P = Q;         % 终端权重矩阵

% 系统矩阵
A = [1, T; 0, 1];  
B = [0; T/m];

% 参考轨迹设置 (线性增长位置 + 恒定速度)
r = zeros(2, N+1);
for k = 0:N
    r(:, k+1) = [k; 1];  % 参考位置线性递增，速度恒定为1
end

% 递推求解代价矩阵 P 和控制增益 K
P_k = P;  % 初始化终端权重
K = zeros(1, 2, N);  % 用于存储控制增益
for k = N:-1:1
    % 控制增益矩阵 K_k
    K(:,:,k) = (R + B' * P_k * B) \ (B' * P_k * A);
    % 更新 P_k (动态规划递推)
    P_k = Q + A' * P_k * A - A' * P_k * B * K(:,:,k);
end

% 初始状态
x = zeros(2, N+1);   % 初始状态 [位置; 速度]
x(:,1) = [0; 0];     % 初始位置为 0，初始速度为 0
u = zeros(1, N);     % 控制输入

% 仿真系统
for k = 1:N
    % 最优控制输入
    u(k) = -K(:,:,k) * (x(:,k) - r(:,k));
    % 系统状态更新
    x(:,k+1) = A * x(:,k) + B * u(k);
end

% 绘图
figure;
subplot(3,1,1);
plot(0:N, x(1,:), 'b-', 'LineWidth', 1.5); hold on;
plot(0:N, r(1,:), 'r--', 'LineWidth', 1.5);
xlabel('时间步 k'); ylabel('位置 p'); legend('实际位置', '参考位置');
title('小车轨迹跟踪 - 位置');

subplot(3,1,2);
plot(0:N, x(2,:), 'b-', 'LineWidth', 1.5); hold on;
plot(0:N, r(2,:), 'r--', 'LineWidth', 1.5);
xlabel('时间步 k'); ylabel('速度 v'); legend('实际速度', '参考速度');
title('小车轨迹跟踪 - 速度');

subplot(3,1,3);
stairs(0:N-1, u, 'g-', 'LineWidth', 1.5);
xlabel('时间步 k'); ylabel('控制输入 u');
title('控制输入');

sgtitle('小车轨迹跟踪 - LQT仿真结果');
