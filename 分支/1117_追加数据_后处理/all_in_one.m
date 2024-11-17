tic; % 开始计时

% 采集数据
disp('[start]:  triple_axis2csv');
run('triple_axis2csv');
disp('[finish]: triple_axis2csv');

% 旋转矩阵 - 消除零飘
disp('[start]:  te');
run('te');
disp('[finish]: te');

% UKF
disp('[start]:  triple_icd');
run('triple_icd');
disp('[finish]: triple_icd');




elapsedTime = toc; % 结束计时并获取所用时间
fprintf('代码块执行时间为 %.4f 秒\n', elapsedTime);
