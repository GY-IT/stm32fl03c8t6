clear all;
close all;
clc;
%%%%%%%%%%  （一）系统矩阵%%%%%%%%%%%%%%%%%
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
%%%%%%%%%%% (三)初始状态%%%%%%%%%%%%%%%%%%%%
x0 = [1; 0];
x = x0;
u0 = 2;
u = u0;
%%%%%%%%%%%%（四）所有状态初始化%%%%%%%%%%%%%%%
k_steps = 100;
x_history = zeros(n, k_steps);
x_history(:, 1) = x;
u_history = zeros(p, k_steps);
u_history(:,1) = u;
%%%%%%%%%%  （五）权重矩阵%%%%%%%%%%%%%%%%%%%
Q = [1 0; 0 1];
Qh = [1 0; 0 1];
R = 0.01;
N= k_steps;
P_k = Qh;
%%%%%%%%%%  （六）计算P矩阵和K矩阵%%%%%%%%%%%%%%%%
% for k = 1 : N
%     K = inv(R+B'*P_k(1:2, : )*B)*B'*P_k(1:2,:)*A;
%     P_k = (A-B*K)'*P_k*(A-B*K)+(K)'*R*(K)+Q;
% %     P_k = Q + A'*P_k*A - A'*P_k*B*inv(R + B' * P_k * B)*B'*P_k*A;
%     if k ==1
%         K_N = K; 
%         P_N = [P_k; Qh];
%     else
%         K_N = [K; K_N];
%         P_N = [P_k; P_N];
%     end
% end
% for k = 1 : N
%     K = inv(R+B'*P_k*B)*B'*P_k*A;
%     P_k = (A-B*K)'*P_k*(A-B*K)+(K)'*R*(K)+Q;
%     if k ==1
%         K_N = K;
%     else
%         K_N = [K; K_N];
%     end
% end
%%%%%%%%%   （七）计算所有输入和状态%%%%%%%%%%%%%%%
for k = 1 : k_steps
    [K, S, P]=dlqr(A,B,Q,R);
    u= -K*x;
%     u = -K_N((k-1)*p+1:k*p, :)*x;

%     if u > 1
%         u = 1;
%     elseif u < -1
%         u = -1
%     else
%         u=u;
%     end
   
    x = A*x + B*u;
    x_history(:, k+1) = x;
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




