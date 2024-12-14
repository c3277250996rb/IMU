% 1.��ȡԭʼ����
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
    % ���� a = 1����ʼԪ�أ���h = 2����������n = 5��Ԫ���ܸ�����������д�ɣ�
    % arr = 1:2:9;
    % disp(arr);
%



% PID������ʼ��
   
    Kp = 1.55;  % Proportional gain
    Ki = 1;   % Integral gain

    % array
        start = Kp;
        % start = Ki;

        % direction = 1;
        direction = -1;

        heel = 0.25;

        % size = 5;
        size = 8;
        
        k_array = zeros(size);
        for i = 1:size
            k_array(i) = start + (i-1) * (direction * heel);
        end
    %

%

nine_axes_data = csv2array();


figure_hold_box();
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
            ['Kp: ', num2str(k_array(1))], ...
            ['Kp: ', num2str(k_array(2))], ...
            ['Kp: ', num2str(k_array(3))], ...
            ['Kp: ', num2str(k_array(4))], ...
            ['Kp: ', num2str(k_array(5))] ...
            );
end


if size == 8
    legend( ...
            ['Kp: ', num2str(k_array(1))], ...
            ['Kp: ', num2str(k_array(2))], ...
            ['Kp: ', num2str(k_array(3))], ...
            ['Kp: ', num2str(k_array(4))], ...
            ['Kp: ', num2str(k_array(5))], ...
            ['Kp: ', num2str(k_array(6))], ...
            ['Kp: ', num2str(k_array(7))], ...
            ['Kp: ', num2str(k_array(8))] ...
            );
end

title(['Ki: ', num2str(Ki)]);


