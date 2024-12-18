% 1.读取原始数据
close_uart();
N = get_public_N();
% s = open_uart(get_com_number);
time=(0:1:N-1);
nine_axes_data = zeros(N,9);
rate = 5000 * 24 * 4;
rate = rate / 10000 / 3;

% example
    % 假设 a = 1（起始元素）、h = 2（步长）、n = 5（元素总个数），可以写成：
    % arr = 1:2:9;
    % disp(arr);
%


% default
    is_kpki = 0;
    is_rollpitchyaw = 0;
%

% is_kpki = 1;
is_rollpitchyaw = 1;



Kp = 2;  % Proportional gain
Ki = 1;   % Integral gain
size = 5;
Kp_adjusting_data = zeros(N, 3, size);
Ki_adjusting_data = zeros(N, 3, size);

% kp_array
    input_K = Kp;
    input_heel = 1;
    input_size = size;
    output_array = diffused_array(input_K, input_heel, input_size);
    kp_array = output_array;
%

% ki_array
    input_K = Ki;
    input_heel = 0.4;
    input_size = size;
    output_array = diffused_array(input_K, input_heel, input_size);
    ki_array = output_array;
%

% generate adjusting data
    nine_axes_data = csv2array();
    for i = 1:size
        for j = 1:N
            temp = nine_axes_data(j,4:6);
            Kp_adjusting_data(j, :, i) = PID4Euler(nine_axes_data(j,:),temp,  kp_array(i), Ki, 1, i);
            Ki_adjusting_data(j, :, i) = PID4Euler(nine_axes_data(j,:),temp,  Kp, ki_array(i), 2, i);
        end
    end
%



%%% KpKi分开画
if is_kpki
    % subplot Kp adjust
        figure_hold_box();
        row = 1;
        colume = 3;
        euler_title = {'roll', 'pitch', 'yaw'};
        adjust_string = 'p: ';
        adjust_legend = cell(1,size);

        for i = 1:size
            for j = 1:3
                location = j;
                subplot(row, colume, location);
                hold on; box on;
                plot(time(1:N), Kp_adjusting_data(:,j,i));
                title(euler_title(j));
            end
            adjust_legend(i) = {[adjust_string, num2str(kp_array(i))]};
        end
        legend(adjust_legend);
    %



    % subplot Ki adjust
        figure_hold_box();
        row = 1;
        colume = 3;
        euler_title = {'roll', 'pitch', 'yaw'};
        adjust_string = 'i: ';
        adjust_legend = cell(1,size);

        for i = 1:size
            for j = 1:3
                location = j;
                subplot(row, colume, location);
                hold on; box on;
                plot(time(1:N), Ki_adjusting_data(:,j,i));
                title(euler_title(j));
            end
            adjust_legend(i) = {[adjust_string, num2str(ki_array(i))]};
        end
        legend(adjust_legend);

    %
end
%%%





%%% RollPitchYaw分开画
if is_rollpitchyaw

    row = 1;
    colume = 2;
    euler_title = {'roll', 'pitch', 'yaw'};
    kp_adjust_string = 'p: ';
    ki_adjust_string = 'i: ';
    kp_adjust_legend = cell(1,size);
    ki_adjust_legend = cell(1,size);
    

    % for j = 1:3
    for j = 2
        figure_hold_box();

        for i = 1:size
            % subplot Kp adjust
                location = 1;
                subplot(row, colume, location);
                hold on; box on;
                plot(time(1:N), Kp_adjusting_data(:,j,i));
                kp_adjust_legend(i) = {[kp_adjust_string, num2str(kp_array(i))]};
                if i == size
                    title(euler_title(j));
                    legend(kp_adjust_legend);
                end
            %

            % subplot Ki adjust
                location = 2;
                subplot(row, colume, location);
                hold on; box on;
                plot(time(1:N), Ki_adjusting_data(:,j,i));
                ki_adjust_legend(i) = {[ki_adjust_string, num2str(ki_array(i))]};
                if i == size
                    title(euler_title(j));
                    legend(ki_adjust_legend);
                end
            %
        end

    end

end
%%%











