function show_single(csv_name)
    tic; % 开始计时
    addpath(genpath('data'));
    addpath(genpath('main'));
    addpath(genpath('src'));
    
    % clear;clc; close all;
    % N = 3000-1;
    N = public_N;

    data = readtable([csv_name]);
    % data = readtable([csv_name, '.csv']);
    x = data.x;


    figure;
    hold on ;box on
    t=(0:1:N-1);
    % x = x ./ 5000 .* 24 ./ 4;
    plot(t, x);

    % 保存图像为PNG文件
    title(csv_name);
    saveas(gcf, 'x-a.png');


end

