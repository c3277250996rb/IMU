% % 假设 myscript.m 在当前目录或在 MATLAB 路径中
% % run('close_uart');

% % 打开串口
% s = serial('COM3', 'BaudRate', 115200); % 根据实际情况调整COM端口和波特率
% fopen(s);

% % 初始化变量
% data = [];
% time = [];
% N = 500;
% Z = zeros(N);
% T = 0.01;

% tic; % 开始计时
% % 循环读取数据，直到收集20个数据点
% for i = 2:N
%     % 从串口读取一个浮点数
%     value = str2double(fgetl(s));
    
%     % 将时间和值添加到数组中
%     time(i) = i;
%     data(i) = value;

%     % if i >= 2
%         Z(i) = Z(i - 1) + (data(i) + data(i - 1)) /2 *T;
%     % end
    
%     % 更新图形
%     plot(i, Z(i), 'o');
%     plot(i, 0, '-k.'); % 真实
%     xlabel('Time');
%     ylabel('Value');
%     title('Real-time Data Plot');
%     drawnow;

%     fprintf('Z(%d): %f\n', i, Z(i));
    
%     % 暂停一小段时间以控制更新频率（可根据需要调整）
%     pause(0.012);
% end

% elapsedTime = toc; % 结束计时并获取所用时间
% fprintf('代码块执行时间为 %.4f 秒\n', elapsedTime);


% % 关闭串口
% fclose(s);
% delete(s);
% clear s;





% % 读取CSV文件
% data = readtable('3axis_data.csv'); % 替换为你的CSV文件名

% % 确保数据行数至少为65
% % if size(data, 1) < 65
% %     error('数据行数不足65行！');
% % end

% % 计算前65行每列的平均值
% mean_x_values = mean(data.x(1:65)); % 按列计算平均值
% mean_y_values = mean(data.y(1:65)); % 按列计算平均值
% mean_z_values = mean(data.z(1:65)); % 按列计算平均值

% % 每列减去对应平均值
% adjusted_data_x = data.x - mean_x_values;
% adjusted_data_y = data.y - mean_y_values;
% adjusted_data_z = data.z - mean_z_values;

% adjusted_data = zeros(600-1,3);

% adjusted_data(:,1) = adjusted_data_x;
% adjusted_data(:,2) = adjusted_data_y;
% adjusted_data(:,3) = adjusted_data_z;
% % 保存调整后的数据到新的CSV文件
% csvwrite('adjusted_data.csv', adjusted_data);






% 读取CSV文件
data = readtable('6axis_data.csv'); % 替换为你的CSV文件名

% 确保数据行数至少为65
% if size(data, 1) < 65
%     error('数据行数不足65行！');
% end


% 旋转加速度零飘
temp_x = 0;
temp_y = 0;
temp_z = 0;
temp_roll   = 0;
temp_pitch  = 0;
temp_yaw    = 0;
% N = 1000-1;
N = 3000-1;
accel_array = zeros(N,3);
gyro_array = zeros(N,3);
for i = 1:N
    temp_x = data.x(i);
    temp_y = data.y(i);
    temp_z = data.z(i);
    temp_roll  = data.roll(i);
    temp_pitch = data.pitch(i);
    temp_yaw   = data.yaw(i);
    % gyro_array(i,:)  = [data.roll(i), data.pitch(i), data.yaw(i)];
    % 旋转矩阵 只需要旋转一次
    accel_array(i,:) = rotateAcceleration(temp_x, temp_y, temp_z, temp_roll, temp_pitch, temp_yaw);
end
disp(accel_array);

% 计算前150行每列的平均值
mean_accel_values = zeros(3);
adjusted_data = zeros(N,3);

for i = 1:3
    % ?????????
    mean_accel_values(i) = mean(accel_array(1:150,i)); % 按列计算平均值
    % 每列减去对应平均值
    adjusted_data(:,i) = accel_array(:,i) - mean_accel_values(i);
    
end
disp(mean_accel_values);
% mean_roll_values    = mean(data.roll    (1:150)); % 按列计算平均值
% mean_pitch_values   = mean(data.pitch   (1:150)); % 按列计算平均值
% mean_yaw_values     = mean(data.yaw     (1:150)); % 按列计算平均值


% adjusted_data_roll  = data.roll  - mean_roll_values ;
% adjusted_data_pitch = data.pitch - mean_pitch_values;
% adjusted_data_yaw   = data.yaw   - mean_yaw_values  ;


% adjusted_data(:,4) = adjusted_data_roll ;
% adjusted_data(:,5) = adjusted_data_pitch;
% adjusted_data(:,6) = adjusted_data_yaw  ;
% 保存调整后的数据到新的CSV文件
csvwrite('adjusted_data.csv', adjusted_data);



% 修改表头
insert_first_row('adjusted_data.csv', 'x,y,z');
run('show');



