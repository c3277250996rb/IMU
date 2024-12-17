% 1.读取原始数据
close_uart();
N = get_public_N();
% s = open_uart(get_com_number);
time=(0:1:N-1);
nine_axes_data = zeros(N,9);
euler_data = zeros(N,3);
rotated_accelerate_data = zeros(N,3);
rate = 5000 * 24 * 4;
rate = rate / 10000 / 3;
mean_head_oa = zeros(3);
mean_head_ra = zeros(3);


% example
    % 假设 a = 1（起始元素）、h = 2（步长）、n = 5（元素总个数），可以写成：
    % arr = 1:2:9;
    % disp(arr);
%


% default
    is_use_compare = 0;
    is_use_display = 0;
%

is_use_compare = 1;
% is_use_display = 1;



% PID参数初始化
   
    Kp = 0.25;  % Proportional gain
    Ki = 0.15;   % Integral gain

    % array
        % start = Kp; s_s = 'Kp';
        start = Ki; s_s = 'Ki';

        % direction = 1;
        direction = -1;

        heel = 0.02;

        size = 5;
        % size = 8;
        
        k_array = zeros(size);
        for i = 1:size
            k_array(i) = start + (i-1) * (direction * heel);
        end
    %

%

nine_axes_data = csv2array();



figure_hold_box();
if is_use_compare
    
    for j = 1:size
        % disp(['[j][', num2str(j), ']']);

        for i = 1:N
            % disp(['[i][', num2str(i), ']']);
            % euler_data(i,:) = PID4Euler(nine_axes_data(i,:));
            euler_data(i,:) = PID4Euler(nine_axes_data(i,:), k_array(j), Ki);

        end

        % plot(time(1:N), euler_data(1:N,1));
        if j == 1
            plot(time(1:N), euler_data(1:N,2), 'k');
        else
            plot(time(1:N), euler_data(1:N,2));
        end
        % plot(time(1:N), euler_data(1:N,3));

    end


        % legend('roll', 'pitch', 'yaw');

    if size == 5
        legend( ...
                [s_s ,': ', num2str(k_array(1))], ...
                [s_s ,': ', num2str(k_array(2))], ...
                [s_s ,': ', num2str(k_array(3))], ...
                [s_s ,': ', num2str(k_array(4))], ...
                [s_s ,': ', num2str(k_array(5))] ...
                );
    end


    if size == 8
        legend( ...
                [s_s, ': ', num2str(k_array(1))], ...
                [s_s, ': ', num2str(k_array(2))], ...
                [s_s, ': ', num2str(k_array(3))], ...
                [s_s, ': ', num2str(k_array(4))], ...
                [s_s, ': ', num2str(k_array(5))], ...
                [s_s, ': ', num2str(k_array(6))], ...
                [s_s, ': ', num2str(k_array(7))], ...
                [s_s, ': ', num2str(k_array(8))] ...
                );
    end

    % title(['Ki: ', num2str(Ki)]);
    title(['Kp: ', num2str(Kp)]);

end




if is_use_display

    j = 3;
    Kp1 = 4.5;
    Ki1 = 1;
    for i = 1:N
        euler_data(i,:) = PID4Euler(nine_axes_data(i,:), Kp1, Ki1);
    end
    for j = 1:3
        plot(time(1:N), euler_data(1:N,j), 'b');
    end


    Kp2 = 0.25;
    Ki2 = 0.15;
    for i = 1:N
        euler_data(i,:) = PID4Euler(nine_axes_data(i,:), Kp2, Ki2);
    end
    for j = 1:3
        plot(time(1:N), euler_data(1:N,j), 'k');
    end


    % legend( ...
    %         ['Kp: ', num2str(Kp1), ' - Ki: ', num2str(Ki1)], ...
    %         ['Kp: ', num2str(Kp2), ' - Ki: ', num2str(Ki2)]  ...
    %         );

    title( ...
            ['蓝[Kp:', num2str(Kp1), ',Ki:', num2str(Ki1), '] - ', ...
             '黑[Kp:', num2str(Kp2), ',Ki:', num2str(Ki2), ']']  ...
            )

end





