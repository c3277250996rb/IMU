% 假设 myscript.m 在当前目录或在 MATLAB 路径中
addpath('tools');
addpath('data');
run('close_uart');

% 打开串口
s = serial('COM23', 'BaudRate', 115200); % 根据实际情况调整COM端口和波特率
fopen(s);

% N = 3000;
N = public_N + 1;
% N = 800;

time = zeros(N);
dataNumeric = zeros(N,6);

tic; % 开始计时
% 循环读取数据，直到收集20个数据点
for i = 1:N

    time(i) = i;
    data = (fgetl(s));
    dataArray = strsplit(data, ',');
    dataNumeric(i,:) = str2double(dataArray);    

    % 更新图形
    % plot(dataNumeric(i,1), 'o');
    % plot(dataNumeric(i,2), 'o');
    % plot(dataNumeric(i,3), 'o');
    % drawnow;

    % 显示解析后的数据
    disp(i);
    disp(dataNumeric(i,:));
    
    % 暂停一小段时间以控制更新频率（可根据需要调整）
    pause(0.01);
end

filename = 'data\6axis_data.csv';
csvwrite(filename, dataNumeric);
disp('csvwrite finished!');

% 关闭串口
fclose(s);
delete(s);
clear s;

clear;clc; close all;

elapsedTime = toc; % 结束计时并获取所用时间
fprintf('代码块执行时间为 %.4f 秒\n', elapsedTime);

% 修改表头
modify_first_row('data\6axis_data.csv', {'x', 'y', 'z', 'roll', 'pitch', 'yaw'});


