% ���� myscript.m �ڵ�ǰĿ¼���� MATLAB ·����
% run('close_uart');

% �򿪴���
s = serial('COM3', 'BaudRate', 115200); % ����ʵ���������COM�˿ںͲ�����
fopen(s);

% ��ʼ������
data = [];
time = [];
N = 9999;
T = 0.01;
velocity = zeros(N);
a = 0.061 / 1000;
L = 2;

% covariation_finally��Э�������ĳ�ʼֵ����ʾ״̬���ƵĲ�ȷ���ԡ�
covariation_finally = eye(L);
Xukf = zeros(L,N);
is_continuous = 1;
is_use_com = 1;


tic; % ��ʼ��ʱ
% ѭ����ȡ���ݣ�ֱ���ռ�20�����ݵ�
for i = 2:N
    % �Ӵ��ڶ�ȡһ��������
    value = str2double(fgetl(s));
    
    % ��ʱ���ֵ��ӵ�������
    time(i) = i;
    data(i) = value;
    velocity(i) = velocity(i - 1) + (data(i) + data(i - 1)) /2 *T;
    
    [Xukf, covariation_finally] = iukf( N, T, L, i, ...
                                        data, Xukf, covariation_finally, ...
                                        is_use_com, is_continuous);

    % ����ͼ��
    % plot(time(2:i), velocity(2:i), '+');
    plot(Xukf(2:i), '+');
    xlabel('Time');
    ylabel('Value');
    title('Real-time Data Plot');
    drawnow;
    
    % ��ͣһС��ʱ���Կ��Ƹ���Ƶ�ʣ��ɸ�����Ҫ������
    pause(0.012);
end

elapsedTime = toc; % ������ʱ����ȡ����ʱ��
fprintf('�����ִ��ʱ��Ϊ %.4f ��\n', elapsedTime);


% �رմ���
fclose(s);
delete(s);
clear s;





