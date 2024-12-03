tic; % 开始计时
addpath('tools');
addpath('data');
addpath('scripts');

% % 采集数据 - 脚本命名开头不能使用数字
disp('[start]:  axis2csv');
run('axis2csv');
disp('[finish]: axis2csv');

% % 旋转矩阵 - 消除零飘
disp('[start]:  rotate_accel');
run('rotate_accel');
disp('[finish]: rotate_accel');

% % UKF
% disp('[start]:  triple_icd');
% run('triple_icd');
% disp('[finish]: triple_icd');

% run('show');
% figure; icd('x'); legend('直接计算V','UKF后V','直接计算S','UKF后S');
% icd('x'); title(['x = ', num2str(iread('data\share.csv', 1, 1))]);
icd('x'); title('+/-0.4米'); legend('加速度', '速度', '位移');
% figure; icd('y'); title(['y = ', num2str(iread('data\share.csv', 1, 1))]);


elapsedTime = toc; % 结束计时并获取所用时间
fprintf('代码块执行时间为 %.4f 秒\n', elapsedTime);
