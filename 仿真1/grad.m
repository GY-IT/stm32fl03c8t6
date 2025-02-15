clear all;close all;clc;
syms x_1 y_1 rou
% f = [x_1 y_1 atan(x_1) atan(y_1)]*eye(4)*[x_1 y_1 atan(x_1) atan(y_1)].'    %%(1)
f = [x_1 y_1 (x_1-1/3*x_1^3) (y_1-1/3*y_1^3)]*eye(4)*[x_1 y_1 (x_1-1/3*x_1^3) (y_1-1/3*y_1^3)].'
% max_iter = 20
% learning_rate = 0.1
%求速度：梯度下降 
x=-3
y=-5
grad_k = gradient(f) 
% for i =1:1:1
    subgrad_k = subs(grad_k,[x_1 y_1],[x y]);
    double_subgrad_k = double(subgrad_k);
    %%%%%%%%求学习率
    xy_f = [x;y] - rou * double_subgrad_k;
    f1 = subs(f,[x_1;y_1],xy_f);
    f1_d = diff(f1)
    learning_rate = solve(f1_d==0);
%     learning_rate_1 = (double_subgrad_k.'*double_subgrad_k)/(double_subgrad_k.'*eye(2)*double_subgrad_k)
    %%%%%%%%
    xy = [x;y] - learning_rate * double_subgrad_k
% end
for i =1:1:1000
    subgrad_k = subs(grad_k,[x_1;y_1],xy);
    double_subgrad_k = double(subgrad_k);
    xy_f = xy - rou * double_subgrad_k;
    f1 = subs(f,[x_1;y_1],xy_f);
    f1_d = diff(f1);
    learning_rate = solve(f1_d==0);
    xy = [x;y] - learning_rate * double_subgrad_k
end


