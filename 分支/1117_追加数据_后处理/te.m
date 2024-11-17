

% ��ȡCSV�ļ�
data = readtable('6axis_data.csv'); % �滻Ϊ���CSV�ļ���

% ȷ��������������Ϊ65
% if size(data, 1) < 65
%     error('������������65�У�');
% end


% ��ת���ٶ���Ʈ
temp_x = 0;
temp_y = 0;
temp_z = 0;
temp_roll   = 0;
temp_pitch  = 0;
temp_yaw    = 0;
% N = 1000-1;
% N = 3000-1;
% N = public_N;
N = 800-1;

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
    % ��ת���� ֻ��Ҫ��תһ��
    accel_array(i,:) = rotateAcceleration(temp_x, temp_y, temp_z, temp_roll, temp_pitch, temp_yaw);
end
% disp(accel_array);
csvwrite('rotated_accel.csv', accel_array);

% ����ǰ150��ÿ�е�ƽ��ֵ
mean_accel_values = zeros(3);
adjusted_data = zeros(N,3);

for i = 1:3
    % ?????????
    mean_accel_values(i) = mean(accel_array(1:150,i)); % ���м���ƽ��ֵ
    % mean_accel_values(i) = mean(accel_array(N-150:N,i)); % ���м���ƽ��ֵ
    % ÿ�м�ȥ��Ӧƽ��ֵ
    adjusted_data(:,i) = accel_array(:,i) - mean_accel_values(i);
    
end
disp(mean_accel_values);


% �������������ݵ��µ�CSV�ļ�
% temp_data = zeros(3000-N-1,3);
% adjusted_data = [adjusted_data; temp_data];

for i = 1:((3000-N-1)/100)
    adjusted_data = [adjusted_data; adjusted_data(50:149,:)];
end

csvwrite('adjusted_data.csv', adjusted_data);


% �޸ı�ͷ
insert_first_row('adjusted_data.csv', 'x,y,z');
run('show');

