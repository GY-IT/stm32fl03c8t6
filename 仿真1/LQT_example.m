clear all;
close all;
clc;
%%%%%%%%%%  （一）系统矩阵%%%%%%%%%%%%%%%%%
%%%%%%%%%   dx=[0 1;-1 -0.5]x+[0; 1]u 
%%%%%%%%%   y=[1 1]x
%%%%%%%%%
A = [0 1 ; -1 -0.5];
n = size(A, 1);
B = [0; 1];
p = size(B, 2);
C = [1  1];
D = [0];
%%%%%%%%%%  (二)离散化%%%%%%%%%%%%%%%%%%%%%
Ts = 0.1;
sys_d = c2d(ss(A, B, C, D), Ts);
A = sys_d.a;
B = sys_d.b;
% A = [1 1; 0 1];
% B = [0.5; 1];
%%%%%%%%%%% (三)初始状态%%%%%%%%%%%%%%%%%%%%
x0 = [0.1; 0.1];
x = x0;
u0 = 0;
u = u0;

a = linspace(0, 1*pi, 100); % 生成从0到2π的100个均匀分布的点
b = sin(a); % 计算每个x对应的sin值
c = ones(1,100);
xd = [b;c];
% xd = [2; 0];
%%%%%%%%%%%%（四）所有状态初始化%%%%%%%%%%%%%%%
k_steps = 100;
x_history = zeros(n, k_steps+1);
x_history(:, 1) = x0;
u_history = zeros(p, k_steps);

%%%%%%%%%%  （五）权重矩阵%%%%%%%%%%%%%%%%%%%
Q = [100 0; 0 1];
Qh = [10 0; 0 1];
R = 0.001;
N= k_steps;

%%%%%%%%%%%%%     扩维    %%%%%%%%%%%%%%%%%%%
xa = [x_history(1:2,1); xd(1:2,1)];
Aa = [A zeros(2); zeros(2) eye(2)];
Ba = [B; zeros(2,1)];
Qa = [eye(2) -eye(2)]'*Q*[eye(2) -eye(2)];
Qha = [eye(2) -eye(2)]'*Qh*[eye(2) -eye(2)];
P_k = Qha;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%  （六）计算P矩阵和K矩阵%%%%%%%%%%%%%%%%
% for k = 1 : N
%     K = inv(R+B'*P_k(1:2, : )*B)*B'*P_k(1:2,:)*A;
%     P_k = (A-B*K)'*P_k*(A-B*K)+(K)'*R*(K)+Q;
%     P_k = Q + A'*P_k*A - A'*P_k*B*inv(R + B' * P_k * B)*B'*P_k*A;
%     if k ==1
%         K_N = K; 
%         P_N = [P_k; Qh];
%     else
%         K_N = [K; K_N];
%         P_N = [P_k; P_N];
%     end
% end
for k = 1 : N
    K = inv(R+Ba'*P_k*Ba)*Ba'*P_k*Aa;
    P_k = (Aa-Ba*K)'*P_k*(Aa-Ba*K)+(K)'*R*(K)+Qa;
    if k ==1
        K_N = K;
    else
        K_N = [K; K_N];
    end
end
%%%%%%%%%   （七）计算所有输入和状态%%%%%%%%%%%%%%%
for k = 1 : k_steps
%     [K, S, P]=dlqr(Aa,Ba,Qa,R);
%     u= -K*xa;
    u = -K_N((k-1)*p+1:k*p, :)*xa;
    xa = [xa(1:2,1);xd(1:2,k)]
    xa = Aa*xa + Ba*u;
    x_history(:, k+1) = xa(1:2,:);
    u_history(:, k) = u;
end
%%%%%%%%    （八）画图%%%%%%%%%%%%%%%%%%%%%%%%%%
subplot(2, 1, 1);
for i = 1:n
    plot(x_history(i,:));
    hold;
end
legend(num2str((1:n)', 'x %d'));
xlim([1, k_steps]);
grid on
subplot(2, 1, 2);
for i = 1:p
    plot(u_history(i,:));
    hold;
end
legend(num2str((1:p)', 'u %d'));
xlim([1, k_steps]);
grid on


