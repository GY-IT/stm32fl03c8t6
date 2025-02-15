clear all;close all;clc;        %清除工作区，关闭窗口，清除命令行
%%%%%%%%%%%%%%%%%%%%%%%%%这里所优化的 速度为横坐标
% %%%%%%%%%%%%%%%%%%%%%%%   （一） 系统矩阵      %%%%%%%%%%%%%%%%%%%%%% 
%dx=[dp; dv]=[0 1; 0 0]*[p; v]+[0; 1]*a
%y=[1 0]*x

tao=0.2;                        % 采样时间
T=2;                            % 2秒内完成
num_h = T/tao;                  % 计算h=10
num_h_add_1 = num_h+1;          % h+1

Ac=[0 1;0 0];                   % 连续系统Ac矩阵        
Bc=[0;1];                       % 连续系统Bc矩阵
z2=zeros(2);                    % 2*2的0矩阵
z2x1=zeros(2,1);                % 2*1的0矩阵
% 连续系统
A = [Ac z2;z2 Ac];                  % 组成A矩阵
B = [Bc z2x1;z2x1 Bc];              % 组成B矩阵
C1 = [1 0 0 0;0 0 1 0];             % C1矩阵
D = zeros(2);                       % D矩阵

% 离散化,eAt预解矩阵laplace法
syms s t Ls;                    % 求状态转移矩阵 利用拉氏变换，syms为符号函数用来定义数学函数
I = eye(size(A));               % 具有A矩阵大小的单位矩阵
Ls = inv(s*I - A);              % 求逆
eAt = ilaplace(Ls,s,t);         % 状态转移矩阵eAt,ilaplace为拉氏反变换函数
G = subs(eAt,t,tao);            % 采样时间取tao = 0.2计算出G
HLs = int(eAt,t,0,tao);         % 0到tao=0.2对t积分
H = HLs*B;                      % 乘B矩阵

% 离散化后系统
A = double(G);                    % 化为矩阵形式
B = double(H);                    % 化为矩阵形式
C1 = C1;
D = D;

% 权重矩阵设置
Q = 1*eye(2);                   % 状态误差权重矩阵
Qh = 10*eye(2);                 % 终端权重矩阵
R = 0.001*eye(2);               % 控制输入权重

% 初始状态
x = zeros(4, num_h_add_1);      % 定义用于存储状态的矩阵
z = zeros(2, num_h_add_1);      % 定义用于存储输出的矩阵
x(:,1) = [0; 0; -1; 0];       % 初始状态位置，初始状态速度 
z(:,1) = C1*x(:,1);             % 计算出输出
u = zeros(2, num_h);            % 定义用于存储控制输入的矩阵

% %%%%%%%%%%%%%%%%    （二）参考轨迹设置      %%%%%%%%%%%%%%%%%%%%%%

% 求曲线长度,平均速度，每一步距离
syms x1  y1                                % 设变量x 函数y
y1 = atan(x1);                            % 曲线y=atan(x)
f_int = sqrt(1+diff(y1)^2);              % 构造积分f函数
vc = double(int(f_int,x1,-3,3)/2);       % 对f_int从-3到3积分，并除以总时间2，为平均速度，转变成double类型
v0 = vc*0.2;                            % 一步0.2秒，v0为每一步距离长度

% 求每一个s横坐标
syms x0 y0 a0                               % 设变量x0 函数y0 上限变量a0
y0 = atan(x0);
f0_int = sqrt(1 + diff(atan(x0), x0).^2);
s0(:,1) = -3;                                   % s0储存计算出的所有s值
for k = 1:num_h                                 % num_h = 10个要求解的s值
    eq = int(f0_int, x0, s0(:,k), a0) - v0;     % 构造单变量要求解的函数eq  
    s0(:,k+1) = vpasolve (eq);                  % 求解函数eq
end

% 求轨迹
r_T_k = zeros(2*(num_h_add_1),1);               % 定义TT的参考轨迹，用于计算
r_T_plot = zeros(2,num_h_add_1);                % 定义TT的参考轨迹，用于绘图
for k = 1:num_h_add_1                           % 一共11个参考点
    p1_T = s0(:,k);                             % 循环取出每一个s的值
    r_T_k(2*k-1:2*k, :) = [p1_T; atan(p1_T)];   % 计算出参考轨迹，用于计算
    r_T_plot(:, k) = [p1_T; atan(p1_T)];        % 计算出参考轨迹，用于绘图
end

% 求 vk
vk = zeros(1,10);                                   % 定义sk和sk+1的步长
for k = 1: 10                                       % 一共10个vk
    vk(:, k) = r_T_k(2*k+1, :) - r_T_k(2*k-1, :);   % vk = sk+1 - sk
end

% %%%%%%%%%%%   （三）递推求解代价矩阵 P 和控制增益 K    %%%%%%%%%%%%%% 

K = cell(1,num_h);              % 定义元胞用于存储每一个 K
L = cell(1,num_h);              % 用于存储 L
P = cell(1,num_h_add_1);        % 用于存储 P
N = cell(1,num_h_add_1);        % 用于存储 N
M = cell(1,num_h_add_1);        % 用于存储 M

P_h = C1'*Qh*C1;                % 定义P_h
N_h = -C1'*Qh;                  % 定义N_h
M_h = Qh;                       % 定义M_h
P{:, num_h_add_1} = P_h;        % 存入P_h
N{:, num_h_add_1} = N_h;        % 存入N_h
M{:, num_h_add_1} = M_h;        % 存入M_h

for k = num_h:-1:1
    % 计算 K L 
    K{:,k} =  -(R + B'*P{:, k+1}*B) \ (B'*P{:, k+1}*A);    % 右除
    L{:,k} =  -(R + B'*P{:, k+1}*B) \ (B'*N{:, k+1});
    % 更新 P N M
    P{:, k} = A'*P{:, k+1}*A + C1'*Q*C1 + A'*P{:, k+1}*B*K{:,k};
    N{:, k} = [-C1'*Q (A + B * K{:,k})'*N{:, k+1}];
    M{:, k} = [Q zeros(2, 2*(num_h-k+1)); zeros(2*(num_h-k+1), 2) M{:, k+1} + N{:, k+1}'*B*L{:,k}];
end

% %%%%%%%%%%%%%%%%%%%    （四）状态、输入、代价求解   %%%%%%%%%%%%%%%%%%%%%


% 仿真系统
for k = 1:num_h
    % 最优控制输入
    u(:,k) = K{:,k} * x(:,k) + L{:,k}*r_T_k(2*(k+1)-1:2*(num_h_add_1),:);
    
    % 系统状态更新
    x(:,k+1) = A * x(:,k) + B * u(:,k);
    z(:,k+1) = C1*x(:,k+1);
end
% 计算代价
J = cell(1,num_h_add_1);            % 定义用于存储代价的矩阵
J{:, num_h_add_1} = x(:,num_h_add_1)'*P{:,num_h_add_1}*x(:,num_h_add_1) + 2*x(:,num_h_add_1)'*N{:,num_h_add_1}*r_T_k(2*num_h_add_1-1:2*num_h_add_1,:) + r_T_k(2*num_h_add_1-1:2*num_h_add_1,:)'*M{:,num_h_add_1}*r_T_k(2*num_h_add_1-1:2*num_h_add_1,:);
for k = 1:1:10
    J{:, k} = x(:,k)'*P{:,k}*x(:,k) + 2*x(:,k)'*N{:,k}*r_T_k(2*k-1:2*num_h_add_1,:) + r_T_k(2*k-1:2*(num_h_add_1),:)'*M{:,k}*r_T_k(2*k-1:2*(num_h_add_1),:);
end
J{:, 1}

% %%%%%%%%%%%%%%%%%%%    P-F  在线       %%%%%%%%%%%%%%%%%%%%%
% syms v1 v2 v3 v4 v5 v6 v7 v8 v9 v10
% Vk = sym('v', [num_h 1]);
% learning_rate = 0.1;                % 学习率
% max_iter = 20;                      % 最大迭代次数
% tolerance = 1e-6;                   % 收敛阈值
% 
% vk;                                 %轨迹跟踪速度
% vPk =zeros(1,10);                   %路径跟随速度                       
% b1 = -3;                            %路径跟随p1_P点
% r_P_k = zeros(2*(num_h_add_1),1);   %路径跟随r_P_k
% r_P_plot = zeros(2,num_h_add_1);    %路径跟随r_P_plot
% 
% xP = zeros(4, num_h_add_1);         % P-F状态
% zP = zeros(2, num_h_add_1);         % P-F输出
% xP(:,1) = [-3.2; 0; 0; 0];          % 初始位置为 ，初始速度为 
% zP(:,1) = C1*xP(:,1);               % P-F输出
% uP = zeros(2, num_h);               % P-F输入
% JP = cell(1,num_h_add_1);           % P-F代价
% 
% %%%%%%%%%%  h函数 
% 
% sk = -3;
% I1=ones(num_h, 1);
% F = tril(ones(num_h));
% fun_h = F*Vk + sk*I1;
% fun_h(10)
% 
% %%%%%%%%%  P-F 构造f中的路径函数ρ函数
% fun_r_P_k = cell(num_h_add_1,1);
% fun_f = cell(1,num_h);              %f函数10个，10个速度
% for i = 1:1:num_h_add_1             %构造路径函数11个
%     if i == 1
%         fun_r_P_k(2*i-1:2*i,:) = num2cell([sk ;atan(sk)]);
%     else
%         fun_r_P_k(2*i-1:2*i,:) = sym2cell([fun_h(i-1) ;atan(fun_h(i-1))]);
%     end
% end
% 
% for k = 1:1:num_h       %   10个速度
%     k
%     %%%%%%%%%%  P-F 构造f函数
%     rou = [fun_r_P_k{2*k-1:2*num_h_add_1,:}];
%     if  k == 1
%         vPk = vk;
%         v_vPk = (vPk(1,k:num_h)');
%         fun_f{:, k} =  2*xP(:,k)'*N{:,k}*rou.' + rou*M{:,k}*rou.'; 
%     else
%         v_vPk = v_vPk_o(2:num_h+2-k,:);
%         fun_f{:, k} = 2*xP(:,k)'*N{:,k}*rou.' + rou*M{:,k}*rou.';
%         fun_f{:, k} = subs(fun_f{:, k},Vk(1:k-1),vPk(1:k-1)');%赋予之前时刻的速度值给f函数，更新函数
%     end
%     %求速度：梯度下降 
%     if k ~= num_h
%         for i =1:1:max_iter
%             grad_k = gradient(fun_f{:, k});   %%求梯度f{:, k}
%             subgrad_k = subs(grad_k,Vk(k:num_h),v_vPk);
%             double_subgrad_k = double(subgrad_k);
%             v_vPk = v_vPk - learning_rate * double_subgrad_k;
%         end
%     else
%         v_vPk = 3 - xP(1,k);
%     end
%     
%     %取出此时的最优速度
%     vPk(k) = v_vPk(1)
%     v_vPk_o = v_vPk;                        %将更改后速度储存
%     %%%%%%%%%%  P-F 求出此时参考轨迹
%     for i = k:num_h
%         b1(:,i+1) = b1(i)+v_vPk(i+1-k);        %更改b1 横坐标
%     end
%     for i = k:num_h_add_1
%         p1_P = b1(i);                       %曲线2
%         r_P_k(2*i-1:2*i, :) = [p1_P; atan(p1_P)]; 
%         r_P_plot(:, i) = [p1_P; atan(p1_P)];
%     end
%     %%%%%%%%%%  P-F 此时状态、输入
%     uP(:,k) = K{:,k} * xP(:,k) + L{:,k}*r_P_k(2*(k+1)-1:2*(num_h_add_1),:);     
%     xP(:,k+1) = A * xP(:,k) + B * uP(:,k);
%     zP(:,k+1) = C1*xP(:,k+1);
%     %%%%%%%%%%  P-F-0 此时状态、输入
%     xP_k = xP(:,1);
%     for i =1:1:num_h
% 
%         uP_k(:,i) = K{:,i} * xP_k(:,i) + L{:,i}*r_P_k(2*(i+1)-1:2*(num_h_add_1),:);     
%         xP_k(:,i+1) = A * xP_k(:,i) + B * uP_k(:,i);
%         zP_k(:,i+1) = C1*xP_k(:,i+1);
%     end
%     JP_sum_k  = 0;
%     for i = num_h_add_1:-1:1
%         JP{:, i} = xP_k(:,i)'*P{:,i}*xP_k(:,i) + 2*xP_k(:,i)'*N{:,i}*r_P_k(2*i-1:2*num_h_add_1,:) + r_P_k(2*i-1:2*(num_h_add_1),:)'*M{:,i}*r_P_k(2*i-1:2*(num_h_add_1),:);
%         JP_sum_k = JP_sum_k + JP{:, i};
%     end
%     JP{:, 1}
%     JP_sum_k
% 
% end

%%%%%%%%%%  P-F 代价

% JP_sum  = 0;
% for k = num_h_add_1:-1:1
%     JP{:, k} = xP(:,k)'*P{:,k}*xP(:,k) + 2*xP(:,k)'*N{:,k}*r_P_k(2*k-1:2*num_h_add_1,:) + r_P_k(2*k-1:2*(num_h_add_1),:)'*M{:,k}*r_P_k(2*k-1:2*(num_h_add_1),:);
%     JP_sum = JP_sum + JP{:, k};
% end
% JP_sum

% %%%%%%%%%%%%%%%%%%%       （五）绘图       %%%%%%%%%%%%%%%%%%%%%

figure;
subplot(2,1,1);
plot(0:num_h, x(1,:), 'b-o', 'LineWidth', 1.5); hold on;
plot(0:num_h, r_T_plot(1,:), 'r--o', 'LineWidth', 1.5);
xlabel('时间步 k'); ylabel('位置 p'); legend('实际位置', '参考位置');
title('小车轨迹跟踪 - 位置');
subplot(2,1,2);
plot(0:num_h, x(3,:), 'b-o', 'LineWidth', 1.5); hold on;
plot(0:num_h, r_T_plot(2,:), 'r--o', 'LineWidth', 1.5);
xlabel('时间步 k'); ylabel('位置 p'); legend('实际位置', '参考位置');
title('小车轨迹跟踪 - 位置');
grid on

figure;
subplot(2,1,1);
plot(0:num_h, x(2,:), 'b-o', 'LineWidth', 1.5); hold on;
xlabel('时间步 k'); ylabel('速度'); 
title('小车轨迹跟踪 - 速度');
subplot(2,1,2);
plot(0:num_h, x(4,:), 'b-o', 'LineWidth', 1.5); hold on;
xlabel('时间步 k'); ylabel('速度'); 
title('小车轨迹跟踪 - 速度');
grid on

figure;
subplot(2,1,1);
stairs(0:num_h-1, u(1,:), 'g-o', 'LineWidth', 1.5);
xlabel('时间步 k'); ylabel('控制输入 u');
title('控制输入');
subplot(2,1,2);
stairs(0:num_h-1, u(2,:), 'g-o', 'LineWidth', 1.5);
xlabel('时间步 k'); ylabel('控制输入 u');
title('控制输入');
grid on

figure;
j = cell2mat(J);
stairs(0:num_h, j, 'y-o', 'LineWidth', 1.5);
xlabel('时间步 k'); ylabel('代价 J');
title('代价');
grid on


figure;
subplot(1,1,1);
plot(x(1,:), x(3,:), 'b-o', 'LineWidth', 1.5); hold on;
plot(r_T_plot(1,:), r_T_plot(2,:), 'r--o', 'LineWidth', 1.5);hold on;
xlim([-3.2 3.1]);
ylim([-2 2]);
xlabel('p1'); ylabel('p2'); legend('实际位置', '参考位置');
title('小车轨迹跟踪 - 位置');
grid on
% subplot(2,1,2);
% plot(xP(1,:), xP(3,:), 'b-o', 'LineWidth', 1.5); hold on;
% plot(r_P_plot(1,:), r_P_plot(2,:), 'r--o', 'LineWidth', 1.5);hold on;
% xlim([-4 4]);
% xlabel('p1'); ylabel('p2'); legend('实际位置', '参考位置');
% title('小车路径跟随 - 位置');
% grid on



