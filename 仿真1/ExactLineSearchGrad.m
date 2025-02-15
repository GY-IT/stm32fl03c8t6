clear all;close all;clc;
num_h_add_1=3;
num_h=2;
syms v1 v2 gama
Vk = sym('v', [num_h 1]);
vk = [0.2 0.2];
xP = [1 ;0 ;0 ;0];
N = [-1	0	-0.0181745223556820	0	0.00601905656020225	0;
0	0	-0.00121213956640082	0	-1.51422806545991e-05	0;
0	-1	0	-0.0181745223556820	0	0.00601905656020225;
0	0	0	-0.00121213956640082	0	-1.51422806545991e-05;];
M = [1	0	0	0	0	0;
0	1	0	0	0	0;
0	0	0.0302883468793631	0	-0.0121138245236791	0;
0	0	0	0.0302883468793631	0	-0.0121138245236791;
0	0	-0.0121138245236791	0	0.00609476796347734	0;
0	0	0	-0.0121138245236791	0	0.00609476796347734;];
%%%%%%%%%%  h函数 
sk = 0;
I1=ones(num_h, 1);
F = tril(ones(num_h));
fun_h = F*Vk + sk*I1;
% fun_h(10)
%%%%%%%%%%  P-F 构造f中的路径函数ρ函数
fun_r_P_k = cell(num_h_add_1,1);
fun_f = cell(1,num_h);              %f函数10个，10个速度
for i = 1:1:num_h_add_1             %构造路径函数11个
    if i == 1
        fun_r_P_k(2*i-1:2*i,:) = num2cell([sk ;atan(sk)]);
    else
        fun_r_P_k(2*i-1:2*i,:) = sym2cell([fun_h(i-1) ;atan(fun_h(i-1))]);
    end
end

k=1
i=1
for k = 1:1:1       
    k
    %%%%%%%%%%  P-F 构造f函数
    rou = [fun_r_P_k{2*k-1:2*num_h_add_1,:}];
    if  k == 1
        vPk = vk;
        v_vPk = (vPk(1,k:num_h)');
        fun_f{:, k} =  2*xP(:,k)'*N*rou.' + rou*M*rou.'; 
%     else
%         v_vPk = v_vPk_o(2:num_h+2-k,:);
%         fun_f{:, k} = 2*xP(:,k)'*N{:,k}*rou.' + rou*M{:,k}*rou.';
%         fun_f{:, k} = subs(fun_f{:, k},Vk(1:k-1),vPk(1:k-1)');%赋予之前时刻的速度值给f函数，更新函数
    end
    %求速度：梯度下降 
    if k ~= num_h
        %%计算梯度
        grad_k = -1*gradient(fun_f{:, k}); %%%求梯度函数
        v_vPk_new = v_vPk
        for i = 1:1:10
            i
            subgrad_k = subs(grad_k,Vk(k:num_h),v_vPk);     %%梯度函数代值,带入新v
            double_subgrad_k = double(subgrad_k);               %%计算出具体值,计算出具体值

            v_vPk_new = v_vPk + gama*double_subgrad_k;      %%构造带有学习率gama的v
    
            newrou = subs(rou,Vk(k:num_h),v_vPk_new);           
            new_fun = 2*xP(:,k)'*N*newrou.' + newrou*M*newrou.';    %%构造带有学习率变量的函数
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            gama_k = solve(diff(new_fun)==0);                   %%%解出gama
            v_vPk_new = v_vPk - gama_k * double_subgrad_k;  %%计算v

            end
        end
    else
        v_vPk = 3 - xP(1,k)
    end
    
    %取出此时的最优速度
    vPk(k) = v_vPk(1)
    v_vPk_o = v_vPk;                        %将更改后速度储存
    %%%%%%%%%%  P-F 求出此时参考轨迹
    for i = k:num_h
        b1(:,i+1) = b1(i)+v_vPk(i+1-k);        %更改b1 横坐标
    end
    for i = k:num_h_add_1
        p1_P = b1(i);                       %曲线2
        r_P_k(2*i-1:2*i, :) = [p1_P; atan(p1_P)]; 
        r_P_plot(:, i) = [p1_P; atan(p1_P)];
    end
    %%%%%%%%%%  P-F 此时状态、输入
    uP(:,k) = K{:,k} * xP(:,k) + L{:,k}*r_P_k(2*(k+1)-1:2*(num_h_add_1),:);     
    xP(:,k+1) = A * xP(:,k) + B * uP(:,k);
    zP(:,k+1) = C1*xP(:,k+1);
    %%%%%%%%%%  P-F-0 此时状态、输入
    xP_k = xP(:,1);
    for i =1:1:num_h

        uP_k(:,i) = K{:,i} * xP_k(:,i) + L{:,i}*r_P_k(2*(i+1)-1:2*(num_h_add_1),:);     
        xP_k(:,i+1) = A * xP_k(:,i) + B * uP_k(:,i);
        zP_k(:,i+1) = C1*xP_k(:,i+1);
    end
    JP_sum_k  = 0;
    for i = num_h_add_1:-1:1
        JP{:, i} = xP_k(:,i)'*P{:,i}*xP_k(:,i) + 2*xP_k(:,i)'*N{:,i}*r_P_k(2*i-1:2*num_h_add_1,:) + r_P_k(2*i-1:2*(num_h_add_1),:)'*M{:,i}*r_P_k(2*i-1:2*(num_h_add_1),:);
        JP_sum_k = JP_sum_k + JP{:, i};
    end
    JP_sum_k

end












% 示例：使用精确线搜索的梯度下降法最小化 f(x, y) = (x - 3)^2 + (y - 2)^2
f = @(x, y) (x - 3).^2 + (y - 2).^2; % 目标函数
df = @(x, y) deal(2*(x - 3), 2*(y - 2)); % 目标函数的梯度

x0 = 0; % 初始x值
y0 = 0; % 初始y值
max_iter = 100; % 最大迭代次数
tolerance = 1e-6; % 收敛阈值

% 调用梯度下降函数
[x_opt, y_opt, f_opt, iter] = gradient_descent_with_exact_line_search(f, df, x0, y0, max_iter, tolerance);

% 输出结果
fprintf('最优解 x: %.6f\n', x_opt);
fprintf('最优解 y: %.6f\n', y_opt);
fprintf('最优值 f(x, y): %.6f\n', f_opt);
fprintf('迭代次数: %d\n', iter);
% 精确线搜索梯度下降法（二维函数）
function [x_opt, y_opt, f_opt, iter] = gradient_descent_with_exact_line_search(f, df, x0, y0, max_iter, tolerance)
    % f: 目标函数
    % df: 目标函数的梯度
    % x0, y0: 初始值
    % max_iter: 最大迭代次数
    % tolerance: 收敛阈值
    
    % 初始化
    x = x0;
    y = y0;
    iter = 0;
    path_x = x; % 保存每一步x的变化
    path_y = y; % 保存每一步y的变化
    f_vals = f(x, y); % 保存每一步对应的f(x, y)值
    
    % 迭代过程
    while iter < max_iter
        % 计算当前点的梯度
        [grad_x, grad_y] = df(x, y);
        
        % 计算当前梯度方向
        direction_x = -grad_x;
        direction_y = -grad_y;
        
        % 使用精确线搜索确定学习率
        t = exact_line_search(f, df, x, y, direction_x, direction_y);
        
        % 更新 x 和 y
        x = x + t * direction_x;
        y = y + t * direction_y;
        
        % 计算目标函数的值
        f_val = f(x, y);
        
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
    f_opt = f(x_opt, y_opt);
    
    % 绘制目标函数的 3D 表面图
    [X, Y] = meshgrid(-5:0.1:5, -5:0.1:5); % 创建网格
    Z = f(X, Y); % 计算每个点的目标函数值
    
    figure;
    surf(X, Y, Z); % 绘制3D表面图
    hold on;
    plot3(path_x, path_y, f_vals, 'ro-', 'MarkerFaceColor', 'r', 'LineWidth', 2); % 绘制梯度下降路径
    title('精确线搜索：梯度下降法优化');
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
end

% 精确线搜索函数
function t = exact_line_search(f, df, x, y, direction_x, direction_y)
    % 使用一维优化找到最优学习率t，沿着梯度方向进行优化
    % 目标函数沿着给定方向更新时的函数
    func = @(t) f(x + t * direction_x, y + t * direction_y);
    
    % 求解一维问题，使用fminbnd来找到最小化目标函数的t
    % fminbnd：用于在一个区间内找到一维函数的最小值
    t = fminbnd(func, 0, 1); % 选择初始区间 [0, 1]，可以调整
end