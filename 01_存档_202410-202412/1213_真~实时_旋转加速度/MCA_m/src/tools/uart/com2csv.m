% ���� myscript.m �ڵ�ǰĿ¼���� MATLAB ·����
addpath(genpath('data'));
addpath(genpath('main'));
addpath(genpath('src'));
close_uart();

% �򿪴���
s = open_uart(get_com_number);

N = get_public_N() + 1;
% ������Դ���
maxRetries = 2;
retryCount = 0;

time = zeros(N);
dataNumeric = zeros(N,6);

tic; % ��ʼ��ʱ
% ѭ����ȡ���ݣ�ֱ���ռ�20�����ݵ�
figure;
hold on; box on;
i = 1;
for i = 1:N

    time(i) = i;
    % ��ȡ����
    while retryCount < maxRetries
        try
            dataNumeric(i,:) = com2data(s);
            break;
        catch ME
            % ���������ʾ������Ϣ
            retryCount = retryCount + 1;
            fprintf('ִ��ʧ��: %s\n', ME.message);
            fprintf('���Ե� %d ��...\n', retryCount);
            % ����Ƿ�ﵽ������Դ���
            if retryCount >= maxRetries
                close_uart();
                fprintf('�ﵽ������Դ�����ִֹͣ�С�\n');
            end
        end
    end
    % data = (fgetl(s));
    % dataArray = strsplit(data, ',');
    % dataNumeric(i,:) = str2double(dataArray);    

    % ����ͼ��
    plot(time(1:i), dataNumeric(1:i,1), 'b');
    plot(time(1:i), dataNumeric(1:i,2), 'r');
    plot(time(1:i), dataNumeric(1:i,3), 'y');
    if i == 10
        % ���⿨��
        legend('x', 'y', 'z');
    end

    drawnow;

    % ��ʾ�����������
    disp(['[', num2str(i), ']']);
    disp(num2str(dataNumeric(i,:)));
    
    % ��ͣһС��ʱ���Կ��Ƹ���Ƶ�ʣ��ɸ�����Ҫ������
    pause(0.0001);
end

filename = 'data\common\6axis_data.csv';
csvwrite(filename, dataNumeric);
disp('csvwrite finished!');

% �رմ���
fclose(s);
delete(s);
clear s;

% clear;clc; close all;

elapsedTime = toc; % ������ʱ����ȡ����ʱ��
fprintf('�����ִ��ʱ��Ϊ %.4f ��\n', elapsedTime);

% �޸ı�ͷ
modify_first_row('data\common\6axis_data.csv', {'x', 'y', 'z', 'roll', 'pitch', 'yaw'});


