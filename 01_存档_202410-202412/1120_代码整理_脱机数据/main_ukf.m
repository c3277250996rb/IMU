tic; % 开始计时
addpath('tools');
addpath('data');
addpath('scripts');

% 采集数据
% disp('[start]:  6axis2csv');
% run('6axis2csv');
% disp('[finish]: 6axis2csv');

% 旋转矩阵 - 消除零飘
disp('[start]:  rotate_accel');
run('rotate_accel');
disp('[finish]: rotate_accel');

% UKF
disp('[start]:  triple_icd');
run('triple_icd');
disp('[finish]: triple_icd');

figure; icd('y');legend('直接计算V','UKF后V','直接计算S','UKF后S');


elapsedTime = toc; % 结束计时并获取所用时间
fprintf('代码块执行时间为 %.4f 秒\n', elapsedTime);
