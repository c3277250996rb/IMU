% % ���� myscript.m �ڵ�ǰĿ¼���� MATLAB ·����
% % run('close_uart');

% % �򿪴���
% s = serial('COM3', 'BaudRate', 115200); % ����ʵ���������COM�˿ںͲ�����
% fopen(s);

% % ��ʼ������
% data = [];
% time = [];
% N = 500;
% Z = zeros(N);
% T = 0.01;

% tic; % ��ʼ��ʱ
% % ѭ����ȡ���ݣ�ֱ���ռ�20�����ݵ�
% for i = 2:N
%     % �Ӵ��ڶ�ȡһ��������
%     value = str2double(fgetl(s));
    
%     % ��ʱ���ֵ��ӵ�������
%     time(i) = i;
%     data(i) = value;

%     % if i >= 2
%         Z(i) = Z(i - 1) + (data(i) + data(i - 1)) /2 *T;
%     % end
    
%     % ����ͼ��
%     plot(i, Z(i), 'o');
%     plot(i, 0, '-k.'); % ��ʵ
%     xlabel('Time');
%     ylabel('Value');
%     title('Real-time Data Plot');
%     drawnow;

%     fprintf('Z(%d): %f\n', i, Z(i));
    
%     % ��ͣһС��ʱ���Կ��Ƹ���Ƶ�ʣ��ɸ�����Ҫ������
%     pause(0.012);
% end

% elapsedTime = toc; % ������ʱ����ȡ����ʱ��
% fprintf('�����ִ��ʱ��Ϊ %.4f ��\n', elapsedTime);


% % �رմ���
% fclose(s);
% delete(s);
% clear s;





% % ��ȡCSV�ļ�
% data = readtable('3axis_data.csv'); % �滻Ϊ���CSV�ļ���

% % ȷ��������������Ϊ65
% % if size(data, 1) < 65
% %     error('������������65�У�');
% % end

% % ����ǰ65��ÿ�е�ƽ��ֵ
% mean_x_values = mean(data.x(1:65)); % ���м���ƽ��ֵ
% mean_y_values = mean(data.y(1:65)); % ���м���ƽ��ֵ
% mean_z_values = mean(data.z(1:65)); % ���м���ƽ��ֵ

% % ÿ�м�ȥ��Ӧƽ��ֵ
% adjusted_data_x = data.x - mean_x_values;
% adjusted_data_y = data.y - mean_y_values;
% adjusted_data_z = data.z - mean_z_values;

% adjusted_data = zeros(600-1,3);

% adjusted_data(:,1) = adjusted_data_x;
% adjusted_data(:,2) = adjusted_data_y;
% adjusted_data(:,3) = adjusted_data_z;
% % �������������ݵ��µ�CSV�ļ�
% csvwrite('adjusted_data.csv', adjusted_data);






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
    % ��ת���� ֻ��Ҫ��תһ��
    accel_array(i,:) = rotateAcceleration(temp_x, temp_y, temp_z, temp_roll, temp_pitch, temp_yaw);
end
disp(accel_array);

% ����ǰ150��ÿ�е�ƽ��ֵ
mean_accel_values = zeros(3);
adjusted_data = zeros(N,3);

for i = 1:3
    % ?????????
    mean_accel_values(i) = mean(accel_array(1:150,i)); % ���м���ƽ��ֵ
    % ÿ�м�ȥ��Ӧƽ��ֵ
    adjusted_data(:,i) = accel_array(:,i) - mean_accel_values(i);
    
end
disp(mean_accel_values);
% mean_roll_values    = mean(data.roll    (1:150)); % ���м���ƽ��ֵ
% mean_pitch_values   = mean(data.pitch   (1:150)); % ���м���ƽ��ֵ
% mean_yaw_values     = mean(data.yaw     (1:150)); % ���м���ƽ��ֵ


% adjusted_data_roll  = data.roll  - mean_roll_values ;
% adjusted_data_pitch = data.pitch - mean_pitch_values;
% adjusted_data_yaw   = data.yaw   - mean_yaw_values  ;


% adjusted_data(:,4) = adjusted_data_roll ;
% adjusted_data(:,5) = adjusted_data_pitch;
% adjusted_data(:,6) = adjusted_data_yaw  ;
% �������������ݵ��µ�CSV�ļ�
csvwrite('adjusted_data.csv', adjusted_data);



% �޸ı�ͷ
insert_first_row('adjusted_data.csv', 'x,y,z');
run('show');



