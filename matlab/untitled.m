%%

books = struct('name',{{'book1','book2'}},'price',[20,40]);
books.name
books.name(1)
books.name{1};

A = [1,2,3;4,5,6];
B = [1,2,3;4,5,6];
C = A .* B;

D = magic(5);
E = D(2,3);
F = D(3, 2:4);
[m, n] = find(D > 22);
[m, n];

G = {{'book1','book2'},{'book2'},'book2',[1,2,3]};
G(4);
G{4};

sum = 0;
for n =1:5
    sum = sum + n^2;
end

sum = 0;
for i = 1:5
    p = 1;
    for j = 1:i
        p = p*j;
    end
    sum = sum + p;
end

s = 0;
n = 1;
while n < 10
    s = s + n;
    n = n + 1;
end


x = 0:0.01:2*pi;
y = sin(x);
%figure
subplot(2,2,1)
plot(x, y)
title('y = sin(x)')
xlabel('x')
ylabel('y')
xlim([0 2*pi])


x = 0:0.01:20;
y1 = 200*exp(-0.05*x).*sin(x);
y2 = 0.8*exp(-0.5*x).*sin(10*x);
%figure
subplot(2,2,2)
[AX, H1, H2] = plotyy(x, y1, x, y2, 'plot' );
[AX, H1, H2];
set(get(AX(1), 'Ylabel'), 'String', 'Slow Decay')
set(get(AX(2), 'Ylabel'), 'String', 'Fast Decay')
xlabel('Time (\musec)')
title('Multiple Decay Rates')
set (H1, 'LineStyle', '--')
set (H2, 'LineStyle', ':')


t = 0:pi/50:10*pi;
plot3(sin(t), cos(t), t)
xlabel('sin(t)')
ylabel('cos(t)')
zlabel('t')
grid on
axis square


[x, y, z,] = peaks(30);
mesh(x, y, z)        %画网格
grid


A = [0,1,0;
    0,0,1;
    -4,-8,-5];
        
[P,J]=jordan(A);



%列写状态空间表达式矩阵
A=[0 0 -1;1 0 -3;0 1 -3];
B=[1 ;1 ;0];

C=[0 1 -2];
D=[0];

%得到传递函数表达式
[num, den]=ss2tf(A, B, C, D);

%在命令行打印传递函数
printsys(num, den);



% 定义系统的矩阵
A = [0 1; 0 0]; % 状态矩阵
B = [0; 1];     % 控制矩阵

% 定义代价函数的权重矩阵
Q = [1 0; 0 1]; % 状态的权重矩阵
R = 1;           % 控制输入的权重

% 计算 LQR 控制器的增益矩阵 K
K = lqr(A, B, Q, R);

% 显示结果
disp('LQR增益矩阵 K:');
disp(K);

% 设定初始状态
x0 = [1; 0]; % 初始状态

% 模拟系统响应
sys = ss(A-B*K, B, eye(2), 0); % 创建状态空间模型
t = 0:0.01:10; % 时间向量
[y, t, x] = lsim(sys, zeros(size(t)), t, x0); % 求解状态响应

% 绘制状态响应图
figure;
plot(t, x(:,1), 'r', 'LineWidth', 2); hold on;
plot(t, x(:,2), 'b', 'LineWidth', 2);
title('LQR 控制下的系统响应');
xlabel('时间 (秒)');
ylabel('状态变量');
legend('x_1', 'x_2');
grid on;








% 定义系统的参数和非线性方程
syms x1 x2 x3
f1 = x2;
f2 = -x1 - x2 + x3^2;
f3 = -x3 + x1^2 - x2^2;

% 定义状态向量 x
f = [f1; f2; f3];

% 求雅可比矩阵 A
A = jacobian(f, [x1, x2, x3]);

% 选择平衡点 (x1, x2, x3) = (0, 0, 0)
A_at_eq = subs(A, [x1, x2, x3], [0, 0, 0]);

% 计算特征值
eigenvalues = eig(A_at_eq);

% 显示特征值
disp('Eigenvalues of the linearized system at equilibrium point:');
disp(eigenvalues);

% 判断特征值是否具有负实部
if all(real(eigenvalues) < 0)
    disp('The linearized system is stable at the equilibrium point.');
else
    disp('The linearized system is not stable at the equilibrium point.');
end
