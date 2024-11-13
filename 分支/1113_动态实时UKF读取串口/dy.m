% 假设 myscript.m 在当前目录或在 MATLAB 路径中
% run('close_uart');

% 打开串口
s = serial('COM3', 'BaudRate', 115200); % 根据实际情况调整COM端口和波特率
fopen(s);

% 初始化变量
data = [];
time = [];
N = 9999;
T = 0.01;
velocity = zeros(N);
a = 0.061 / 1000;
L = 2;

% covariation_finally：协方差矩阵的初始值，表示状态估计的不确定性。
covariation_finally = eye(L);
Xukf = zeros(L,N);
is_continuous = 1;
is_use_com = 1;


tic; % 开始计时
% 循环读取数据，直到收集20个数据点
for i = 2:N
    % 从串口读取一个浮点数
    value = str2double(fgetl(s));
    
    % 将时间和值添加到数组中
    time(i) = i;
    data(i) = value;
    velocity(i) = velocity(i - 1) + (data(i) + data(i - 1)) /2 *T;
    
    [Xukf, covariation_finally] = iukf( N, T, L, i, ...
                                        data, Xukf, covariation_finally, ...
                                        is_use_com, is_continuous);

    % 更新图形
    % plot(time(2:i), velocity(2:i), '+');
    plot(Xukf(2:i), '+');
    xlabel('Time');
    ylabel('Value');
    title('Real-time Data Plot');
    drawnow;
    
    % 暂停一小段时间以控制更新频率（可根据需要调整）
    pause(0.012);
end

elapsedTime = toc; % 结束计时并获取所用时间
fprintf('代码块执行时间为 %.4f 秒\n', elapsedTime);


% 关闭串口
fclose(s);
delete(s);
clear s;





