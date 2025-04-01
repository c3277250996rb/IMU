% 1.读取原始数据
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

% UKF参数初始化
    % Q：过程噪声的协方差，用于模拟系统中的随机扰动。
    Q = 1;
    % R：观测噪声的协方差，表示测量过程中的随机误差。
    R = 1;
    % L：状态变量的维度。
    L = 2;    %?个变量
    % alpha、kalpha、belta：UKF的超参数，用于调整Sigma点的生成和权重。
    alpha=1;
    kalpha=0;
    belta=2;
    % ramda：UKF的扩展参数，影响Sigma点的散布程度。
    ramda=3-L;

    for j=1:2*L+1
        Wm(j)=1/(2*(L+ramda));
        Wc(j)=1/(2*(L+ramda));
    end
    % Wm(1)=ramda/(L+ramda);%权值Wm的初值需要另外定
    % Wc(1)=ramda/(L+ramda)+1-alpha^2+belta;%权值Wc的初值需要另外定
    % Z = accelerate;
    Xukf = zeros(L,N);
    covariation_finally=eye(L);%协方差阵初始化
%

figure; hold on; box on;
for i = 1:N
    time(i) = i;
    six_axis_data(i,:) = com2data(s);

    % 2.旋转并存至temp
    rotated_accelerate_data(i,:) = rotateAcceleration(six_axis_data(i,:));
    
    if i < 50
        disp(['[', num2str(i), ']']);
    end
    % if i == 50
    %     disp('[初始化][完成]');
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




