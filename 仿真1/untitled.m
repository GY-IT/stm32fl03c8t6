% 蒙特卡洛模拟的次数
num_simulations = 100;

% 每次模拟投掷的硬币次数
num_tosses = 1000;

% 存储每次模拟的结果
heads_count = zeros(num_simulations, 1);

% 进行100次蒙特卡洛模拟
for i = 1:num_simulations
    % 每次模拟中进行 num_tosses 次投掷，生成0（反面）和1（正面）
    tosses = rand(num_tosses, 1) < 0.5;  % 生成均匀分布的随机数，0.5作为阈值
    heads_count(i) = sum(tosses);  % 计算正面朝上的次数
end

% 计算正面朝上的平均次数
mean_heads = mean(heads_count);

% 绘制结果
figure;
histogram(heads_count, 20);  % 绘制100次模拟结果的直方图
title('100次蒙特卡洛模拟结果');
xlabel('正面朝上的次数');
ylabel('频率');

disp(['正面朝上的平均次数: ', num2str(mean_heads)]);
