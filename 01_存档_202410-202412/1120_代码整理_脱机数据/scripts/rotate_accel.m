% ��ȡCSV�ļ�
addpath('tools');
addpath('data');
data = readtable('data\6axis_data.csv'); % �滻Ϊ���CSV�ļ���

% ȷ��������������Ϊ65
if size(data, 1) < 150
    error('������������150�У�');
end

% ��ת���� - ��ת���ٶ�
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
    % ��ת���� ֻ��Ҫ��תһ��
    accel_array(i,:) = rotateAcceleration(temp_x, temp_y, temp_z, temp_roll, temp_pitch, temp_yaw);
end
disp(accel_array);

% ����ǰ150��ÿ�е�ƽ��ֵ - ������Ʈ
mean_accel_values = zeros(3);
adjusted_data = zeros(N,3);

for i = 1:3
    % ?????????
    mean_accel_values(i) = mean(accel_array(1:150,i)); % ���м���ƽ��ֵ
    % ÿ�м�ȥ��Ӧƽ��ֵ
    adjusted_data(:,i) = accel_array(:,i) - mean_accel_values(i);
    
end
disp(mean_accel_values);

% �������������ݵ��µ�CSV�ļ�
csvwrite('data\adjusted_data.csv', adjusted_data);

% �޸ı�ͷ
insert_first_row('data\adjusted_data.csv', 'x,y,z');
run('show');



