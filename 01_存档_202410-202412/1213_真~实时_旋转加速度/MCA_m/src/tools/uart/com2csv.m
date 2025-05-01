% 假设 myscript.m 在当前目录或在 MATLAB 路径中
addpath(genpath('data'));
addpath(genpath('main'));
addpath(genpath('src'));
close_uart();

% 打开串口
s = open_uart(get_com_number);

N = get_public_N() + 1;
% 最大重试次数
maxRetries = 2;
retryCount = 0;

time = zeros(N);
dataNumeric = zeros(N,6);

tic; % 开始计时
% 循环读取数据，直到收集20个数据点
figure;
hold on; box on;
i = 1;
for i = 1:N

    time(i) = i;
    % 读取数据
    while retryCount < maxRetries
        try
            dataNumeric(i,:) = com2data(s);
            break;
        catch ME
            % 捕获错误并显示错误信息
            retryCount = retryCount + 1;
            fprintf('执行失败: %s\n', ME.message);
            fprintf('重试第 %d 次...\n', retryCount);
            % 检查是否达到最大重试次数
            if retryCount >= maxRetries
                close_uart();
                fprintf('达到最大重试次数，停止执行。\n');
            end
        end
    end
    % data = (fgetl(s));
    % dataArray = strsplit(data, ',');
    % dataNumeric(i,:) = str2double(dataArray);    

    % 更新图形
    plot(time(1:i), dataNumeric(1:i,1), 'b');
    plot(time(1:i), dataNumeric(1:i,2), 'r');
    plot(time(1:i), dataNumeric(1:i,3), 'y');
    if i == 10
        % 避免卡顿
        legend('x', 'y', 'z');
    end

    drawnow;

    % 显示解析后的数据
    disp(['[', num2str(i), ']']);
    disp(num2str(dataNumeric(i,:)));
    
    % 暂停一小段时间以控制更新频率（可根据需要调整）
    pause(0.0001);
end

filename = 'data\common\6axis_data.csv';
csvwrite(filename, dataNumeric);
disp('csvwrite finished!');

% 关闭串口
fclose(s);
delete(s);
clear s;

% clear;clc; close all;

elapsedTime = toc; % 结束计时并获取所用时间
fprintf('代码块执行时间为 %.4f 秒\n', elapsedTime);

% 修改表头
modify_first_row('data\common\6axis_data.csv', {'x', 'y', 'z', 'roll', 'pitch', 'yaw'});


