% ���� myscript.m �ڵ�ǰĿ¼���� MATLAB ·����
addpath('tools');
addpath('data');
run('close_uart');

% �򿪴���
s = serial('COM23', 'BaudRate', 115200); % ����ʵ���������COM�˿ںͲ�����
fopen(s);

% N = 3000;
N = public_N + 1;
% N = 800;

time = zeros(N);
dataNumeric = zeros(N,6);

tic; % ��ʼ��ʱ
% ѭ����ȡ���ݣ�ֱ���ռ�20�����ݵ�
for i = 1:N

    time(i) = i;
    data = (fgetl(s));
    dataArray = strsplit(data, ',');
    dataNumeric(i,:) = str2double(dataArray);    

    % ����ͼ��
    % plot(dataNumeric(i,1), 'o');
    % plot(dataNumeric(i,2), 'o');
    % plot(dataNumeric(i,3), 'o');
    % drawnow;

    % ��ʾ�����������
    disp(i);
    disp(dataNumeric(i,:));
    
    % ��ͣһС��ʱ���Կ��Ƹ���Ƶ�ʣ��ɸ�����Ҫ������
    pause(0.01);
end

filename = 'data\6axis_data.csv';
csvwrite(filename, dataNumeric);
disp('csvwrite finished!');

% �رմ���
fclose(s);
delete(s);
clear s;

clear;clc; close all;

elapsedTime = toc; % ������ʱ����ȡ����ʱ��
fprintf('�����ִ��ʱ��Ϊ %.4f ��\n', elapsedTime);

% �޸ı�ͷ
modify_first_row('data\6axis_data.csv', {'x', 'y', 'z', 'roll', 'pitch', 'yaw'});


