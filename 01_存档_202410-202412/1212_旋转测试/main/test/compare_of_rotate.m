% 1.��ȡԭʼ����
close_uart();
N = get_public_N;
s = open_uart(get_com_number);
time = zeros(N);
six_axis_data = zeros(N,6);
rotated_accelerate_data = zeros(N,3);
rate = 5000 * 24 * 4;
rate = rate / 50000;
is_use_rate = 1;
mean_head_oa = zeros(3);
mean_head_ra = zeros(3);

% UKF������ʼ��
    % Q������������Э�������ģ��ϵͳ�е�����Ŷ���
    Q = 1;
    % R���۲�������Э�����ʾ���������е������
    R = 1;
    % L��״̬������ά�ȡ�
    L = 2;    %?������
    % alpha��kalpha��belta��UKF�ĳ����������ڵ���Sigma������ɺ�Ȩ�ء�
    alpha=1;
    kalpha=0;
    belta=2;
    % ramda��UKF����չ������Ӱ��Sigma���ɢ���̶ȡ�
    ramda=3-L;

    for j=1:2*L+1
        Wm(j)=1/(2*(L+ramda));
        Wc(j)=1/(2*(L+ramda));
    end
    % Wm(1)=ramda/(L+ramda);%ȨֵWm�ĳ�ֵ��Ҫ���ⶨ
    % Wc(1)=ramda/(L+ramda)+1-alpha^2+belta;%ȨֵWc�ĳ�ֵ��Ҫ���ⶨ
    % Z = accelerate;
    Xukf = zeros(L,N);
    covariation_finally=eye(L);%Э�������ʼ��
%

figure; hold on; box on;
for i = 1:N
    time(i) = i;
    six_axis_data(i,:) = com2data(s);

    % 2.��ת������temp
    rotated_accelerate_data(i,:) = rotateAcceleration(six_axis_data(i,:));
    
    if i < 50
        disp(['[', num2str(i), ']']);
    end
    % if i == 50
    %     disp('[��ʼ��][���]');
    %     for j = 1:3
    %         mean_head_oa(j) = mean(six_axis_data())
    % end

    if is_use_rate
        six_axis_data(i,1:3) = six_axis_data(i,1:3) ./ rate;
        rotated_accelerate_data(i,:) = rotated_accelerate_data(i,:) ./ rate;
    end



    %%%
        % 3.plot
        % oa
        % plot(time(1:i), six_axis_data(1:i,1), 'r');
        % plot(time(1:i), six_axis_data(1:i,2), 'r');
        % plot(time(1:i), six_axis_data(1:i,3), 'r');

        % ra
        plot(time(1:i), rotated_accelerate_data(1:i,1), 'r');
        plot(time(1:i), rotated_accelerate_data(1:i,2), 'b');
        % plot(time(1:i), rotated_accelerate_data(1:i,3), 'y');

        % euler
        plot(time(1:i), six_axis_data(1:i,4), 'g');
        plot(time(1:i), six_axis_data(1:i,5), 'k');
        plot(time(1:i), six_axis_data(1:i,6), 'm');

    %%%


    if i == 2
        % legend('roll', 'pitch', 'yaw');
        % legend('o', 'r');
        % legend('x', 'y', 'z');
        legend('x', 'y', 'roll', 'pitch', 'yaw');
    end

    drawnow;
    pause(0.001);
end

% 4.
% 
% 
% 
% 
% 




