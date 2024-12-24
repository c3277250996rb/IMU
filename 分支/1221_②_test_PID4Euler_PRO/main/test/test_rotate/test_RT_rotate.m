% 1.读取原始数据
close_uart();

                    r_d_t = 6;
range_data = 50 *   r_d_t;
range_data_plus_1 = range_data + 1;

N = range_data_plus_1;
time = (1:1:range_data);
s = open_uart(get_com_number);

nine_axes_data = zeros(N,9);
six_axes_data = zeros(N,6);
euler_data = zeros(N,3);
rotated_accelerate_data = zeros(N,3);

% 准线
reference_positive_1500 = (zeros(N,1) + 1500);
reference_positive_90 = (zeros(N,1) + 90);
reference_zero = zeros(N,1);
reference_negative_90 = (zeros(N,1) - 90);
reference_negative_1500 = (zeros(N,1) - 1500);


row = 3;
colume = 1;


Kp = 4;  % Proportional gain
Ki = 1;   % Integral gain
size = 5;
size = 7;
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






figure_hold_box();
i = 0;
start = 0;
finish = 0;
while true
    plot_length = min(range_data, i);
    if i < range_data_plus_1
        i = i + 1;
    end


%%% Com & Euler & Rotate

    % com
    nine_axes_data(i,:) = com2data(s);

    % euler
    input_gyro = nine_axes_data(i,4:6);
    euler_data(i,:) = PID4Euler(size, nine_axes_data(i,:), input_gyro, Kp, Ki, 1, 1);
    
    % rotate
    six_axes_data(i,:) = [nine_axes_data(i,1:3), euler_data(i,:)];
    rotated_accelerate_data(i,:) = rotateAcceleration(six_axes_data(i,:));

%%%


    if i == range_data_plus_1
        nine_axes_data(1:range_data,:) = nine_axes_data(2:range_data_plus_1,:);
        rotated_accelerate_data(1:range_data,:) = rotated_accelerate_data(2:range_data_plus_1,:);
        euler_data(1:range_data,:) = euler_data(2:range_data_plus_1,:);
    end



%%% Auto Range Plot


    

    % original accelerate 
        location = 1;
        subplot(row, colume, location);
        cla;
        hold on; box on;
        % ox
        plot(time(1:plot_length), nine_axes_data(1:plot_length,1), 'r');
        % oy
        plot(time(1:plot_length), nine_axes_data(1:plot_length,2), 'b');
        % reference_axis
        plot(time(1:plot_length), reference_positive_1500(1:plot_length,1), 'w');
        plot(time(1:50), reference_zero(1:50,1), 'k');
        if plot_length > 10
            plot(time(plot_length-10:plot_length), reference_zero(plot_length-10:plot_length,1), 'k');
        end
        plot(time(1:plot_length), reference_negative_1500(1:plot_length,1), 'w');
    %


    % rotated accelerate 
        location = 2;
        subplot(row, colume, location);
        cla;
        hold on; box on;
        % rx
        plot(time(1:plot_length), rotated_accelerate_data(1:plot_length,1), 'g');
        % ry
        plot(time(1:plot_length), rotated_accelerate_data(1:plot_length,2), 'm');
        % reference_axis
        plot(time(1:plot_length), reference_positive_1500(1:plot_length,1), 'w');
        plot(time(1:50), reference_zero(1:50,1), 'k');
        if plot_length > 10
            plot(time(plot_length-10:plot_length), reference_zero(plot_length-10:plot_length,1), 'k');
        end
        plot(time(1:plot_length), reference_negative_1500(1:plot_length,1), 'w');
    %


    % roll pitch yaw
        location = 3;
        subplot(row, colume, location);
        cla;
        hold on; box on;
        % roll
        plot(time(1:plot_length), euler_data(1:plot_length,1), 'c');
        % pitch
        plot(time(1:plot_length), euler_data(1:plot_length,2));
        % % yaw
        plot(time(1:plot_length), euler_data(1:plot_length,3));
        % reference_axis
        plot(time(1:plot_length), reference_positive_90(1:plot_length,1), 'w');
        plot(time(1:plot_length), reference_negative_90(1:plot_length,1), 'w');
    %




%%%



    if i == 10
        location = 1;
        subplot(row, colume, location);
        legend('o x', 'o y');

        location = 2;
        subplot(row, colume, location);
        legend('r x', 'r y');

        location = 3;
        subplot(row, colume, location);
        legend('roll', 'pitch', 'yaw');
    end

    
    drawnow;
    pause(0.001);
end



