% 梯度下降法算法（二维函数）

% f: 目标函数
% df: 目标函数的梯度
% x0, y0: 初始值
% learning_rate: 学习率
% max_iter: 最大迭代次数
% tolerance: 收敛阈值
x0 = 0; % 初始x值
y0 = 0; % 初始y值
learning_rate = 0.1; % 学习率
max_iter = 100; % 最大迭代次数
tolerance = 1e-6; % 收敛阈值
% 定义符号变量
syms x1 y1 v_h2 v_h1
% 定义符号函数 u(x,y)
r = [atan(1+x1+y1); atan(1+x1); atan(1)]
u = r'*r;%函数   
gradU = gradient(u, [x1, y1]);%梯度
f1 = matlabFunction(u); %计算函数值
f2 = matlabFunction(gradU); %计算梯度值

% 初始化
x = x0;
y = y0;
iter = 0;
path_x = x; % 保存每一步x的变化
path_y = y; % 保存每一步y的变化
f_vals = f1(x, y); % 保存每一步对应的f(x, y)值

% 迭代过程
while iter < max_iter
    % 计算当前点的梯度
    v=f2(x,y);
    grad_x = v(1);
    grad_y = v(2);
    % 更新 x 和 y
    x = x - learning_rate * grad_x;
    y = y - learning_rate * grad_y;
    % 计算目标函数的值
    f_val = f1(x,y); 
    % 保存每一步的 x, y 和 f(x, y)
    path_x = [path_x, x];
    path_y = [path_y, y];
    f_vals = [f_vals, f_val];
    % 判断是否收敛（如果梯度小于阈值）
    if sqrt(grad_x^2 + grad_y^2) < tolerance
        break;
    end
    iter = iter + 1;
end
% 返回最优解及对应的目标函数值
x_opt = x;
y_opt = y;
f_opt = f1(x_opt, y_opt);

% 绘制目标函数的 3D 表面图
[X, Y] = meshgrid(-5:0.1:5, -5:0.1:5); % 创建网格
Z = f1(X, Y); % 计算每个点的目标函数值
figure;
surf(X, Y, Z); % 绘制3D表面图
hold on;
plot3(path_x, path_y, f_vals, 'ro-', 'MarkerFaceColor', 'r', 'LineWidth', 2); % 绘制梯度下降路径
title('梯度下降法：二维函数最优化');
xlabel('x');
ylabel('y');
zlabel('f(x, y)');
legend('目标函数 f(x, y)', '梯度下降路径');
grid on;
% 输出结果
fprintf('最优解 x: %.6f\n', x_opt);
fprintf('最优解 y: %.6f\n', y_opt);
fprintf('最优值 f(x, y): %.6f\n', f_opt);
fprintf('迭代次数: %d\n', iter);
   