% 1.读取原始数据
close_uart();
N = get_public_N();
% s = open_uart(get_com_number);
time=(0:1:N-1);
six_axis_data = zeros(N,6);
rotated_accelerate_data = zeros(N,3);
rate = 5000 * 24 * 4;
rate = rate / 10000 / 3;
mean_head_oa = zeros(3);
mean_head_ra = zeros(3);

% UKF参数初始化
   
%

six_axis_data = csv2array();

rotated_accelerate_data = rotateAcceleration(six_axis_data);


%%% 均值 & 比例
    % six_axis_data = six_axis_data ./ rate;
    rotated_accelerate_data = rotated_accelerate_data ./ rate;

    % 均值
    for i = 1:3
        six_axis_data(:,i) = ( six_axis_data(:,i) - mean(six_axis_data(1:100,i)) ) ./rate;
        rotated_accelerate_data(:,i) = rotated_accelerate_data(:,i) - mean(rotated_accelerate_data(1:100,i));
    end

    % yaw比例
    six_axis_data(:,6) = six_axis_data(:,6) / 3;
%%%



%%% V 速度计算
    ov = a2v(six_axis_data, 'list');
    rv = a2v(rotated_accelerate_data, 'list');
%%%






% %%% 原始
%     figure; hold on; box on;

%     for i = 1:1
%         oa = six_axis_data(:,i);
%         plot(time,oa);
%         plot(time,ov(:,i));
%     end

%     for i = 4:6
%         angle = six_axis_data(:,i);
%         plot(time,angle);
%     end
%     legend('ox','ov','roll','pitch','yaw');



%     figure; hold on; box on;
%     for i = 2:2
%         oa = six_axis_data(:,i);
%         plot(time,oa);
%         plot(time,ov(:,i));
%     end

%     for i = 4:6
%         angle = six_axis_data(:,i);
%         plot(time,angle);
%     end
%     legend('oy','ov','roll','pitch','yaw');
% %%%


%%% 旋转
    figure; hold on; box on;
    for i = 1:1
        ra = rotated_accelerate_data(:,i);
        plot(time,ra);
        plot(time,rv(:,i));
    end

    for i = 4:6
        angle = six_axis_data(:,i);
        plot(time,angle);
    end
    legend('rx','rv','roll','pitch','yaw');



    figure; hold on; box on;
    for i = 2:2
        ra = rotated_accelerate_data(:,i);
        plot(time,ra);
        plot(time,rv(:,i));
    end

    for i = 4:6
        angle = six_axis_data(:,i);
        plot(time,angle);
    end
    legend('ry','rv','roll','pitch','yaw');

%%%


