% 读取CSV文件
addpath('tools');
addpath('data');
data = readtable('data\6axis_data.csv'); % 替换为你的CSV文件名

% 确保数据行数至少为65
if size(data, 1) < 150
    error('数据行数不足150行！');
end

% 旋转矩阵 - 旋转加速度
temp_x = 0;
temp_y = 0;
temp_z = 0;
temp_roll   = 0;
temp_pitch  = 0;
temp_yaw    = 0;

N = public_N;

accel_array = zeros(N,3);
gyro_array = zeros(N,3);

for i = 1:N
    temp_x = data.x(i);
    temp_y = data.y(i);
    temp_z = data.z(i);
    temp_roll  = data.roll(i);
    temp_pitch = data.pitch(i);
    temp_yaw   = data.yaw(i);
    % 旋转矩阵 只需要旋转一次
    accel_array(i,:) = rotateAcceleration(temp_x, temp_y, temp_z, temp_roll, temp_pitch, temp_yaw);
end
disp(accel_array);

% 计算前150行每列的平均值 - 计算零飘
mean_accel_values = zeros(3);
adjusted_data = zeros(N,3);

for i = 1:3
    % ?????????
    mean_accel_values(i) = mean(accel_array(1:150,i)); % 按列计算平均值
    % 每列减去对应平均值
    adjusted_data(:,i) = accel_array(:,i) - mean_accel_values(i);
    
end
disp(mean_accel_values);

% 保存调整后的数据到新的CSV文件
csvwrite('data\adjusted_data.csv', adjusted_data);

% 修改表头
insert_first_row('data\adjusted_data.csv', 'x,y,z');
run('show');



