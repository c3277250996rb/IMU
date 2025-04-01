% 1.��ȡԭʼ����
close_uart();
N = get_public_N;
s = open_uart(get_com_number);
time = zeros(N);
six_axes_data = zeros(N,6);
L = 9;
nine_axes_data = zeros(N,L);

rotated_accelerate_data = zeros(N,3);
rate = 5000 * 24 * 4;
rate = rate / 50000;
is_use_rate = 1;




%%% default
    is_only_original = 0;
    is_only_rotated  = 0;
    is_both          = 0;
%%%

% is_only_original = 1;
% is_only_rotated  = 2;
is_both          = 3;


Kp = 4;
Ki = 1;

% Kp = 0.25;
% Ki= 0.15;





figure; hold on; box on;
% for i = 1:N
i = 0;
while true

    i = i + 1;
    if i == 80000
    % if i == 300
        % break;
        % ����
        i = 1;
        time = zeros(N);
        six_axes_data = zeros(N,6);
        nine_axes_data = zeros(N,L);
        rotated_accelerate_data = zeros(N,3);
    end


    %%% ��ȡ & PID & rotate
        time(i) = i;
        nine_axes_data(i,:) = com2data(s);

        six_axes_data(i,1:3) = nine_axes_data(i,1:3);
        six_axes_data(i,4:6) = PID4Euler(nine_axes_data(i,:), Kp, Ki);

        % 2.��ת������temp
        if is_both || is_only_rotated
            rotated_accelerate_data(i,:) = rotateAcceleration(six_axes_data(i,:));
        else
            rotated_accelerate_data(i,:) = six_axes_data(i,:);
        end
    %%%


    %%% rate
        % if i < 3
        if i <= 99
            % disp (['[��ʼ��][', num2str(i), ']']);
        end
        if i == 100
            title('[���]');
            disp ('[��ʼ��][���]');
            % for j = 1:3
            %     mean_head_oa(j) = mean(six_axes_data());
            % end
        end

        if is_use_rate
            six_axes_data(i,1:3) = six_axes_data(i,1:3) ./ rate;
            % six_axes_data(i,6) = six_axes_data(i,6) ./ 3;
            rotated_accelerate_data(i,:) = -(rotated_accelerate_data(i,:) ./ rate);
        end

        if is_both
            six_axes_data(i,1:3) = six_axes_data(i,1:3) + 300;
            six_axes_data(i,6) = (six_axes_data(i,6)) ;
        end
    %%%


    %%% new plot
        if is_only_rotated
            t_od = zeros(200,6);
            t_ra = zeros(200,3);
            window_size = 300;
            % window_size = 100;
            % window_size = 10;

            if i >= window_size
                t_od = six_axes_data(i-window_size+1:i,:);
                t_ra = rotated_accelerate_data(i-window_size+1:i,:);
                t_t  = time(i-window_size+1:i);

                
                % zs = zeros(window_size);
                % zs(:) = 0.0001;
                % plot(t_t, zs, 'k');


                cla; % �����
                % legend('x', 'y', 'roll', 'pitch', 'yaw');
                % 3.
                % oa
                % plot(t_t, t_od(:,1), 'r');
                % plot(t_t, t_od(:,2), 'r');
                % plot(t_t, t_od(:,3), 'r');

                % ra
                plot(t_t, t_ra(:,1), 'r');
                plot(t_t, t_ra(:,2), 'b');
                % plot(t_t, t_ra(:,3), 'y');

                % euler
                plot(t_t, t_od(:,4), 'g');
                plot(t_t, t_od(:,5), 'k');
                plot(t_t, t_od(:,6), 'm');


            else
                % continue;
                t_od = six_axes_data;
                t_ra = rotated_accelerate_data;
                t_t  = time;


                % zs = zeros(i);
                % zs(:) = 0.0001;
                % plot(t_t(1:i), zs, 'k');


                cla; % �����
                % title(['[��ʼ��][', num2str(i), ']']);
                % legend('x', 'y', 'roll', 'pitch', 'yaw');
                % 3.
                % oa
                % plot(time, six_axes_data(:,1), 'r');
                % plot(time, six_axes_data(:,2), 'r');
                % plot(time, six_axes_data(:,3), 'r');

                % ra
                plot(t_t(1:i), t_ra(1:i,1), 'r');
                plot(t_t(1:i), t_ra(1:i,2), 'b');
                % plot(time, rotated_accelerate_data(:,3), 'y');

                % euler
                plot(t_t(1:i), t_od(1:i,4), 'g');
                plot(t_t(1:i), t_od(1:i,5), 'k');
                plot(t_t(1:i), t_od(1:i,6), 'm');
            end
        end

        if is_both
            t_od = zeros(200,6);
            t_ra = zeros(200,3);
            window_size = 300;
            % window_size = 100;
            % window_size = 10;

            if i >= window_size

                % 1
                t_od = six_axes_data(i-window_size+1:i,:);
                t_ra = rotated_accelerate_data(i-window_size+1:i,:);
                t_t  = time(i-window_size+1:i);

                
                % zs = zeros(window_size);
                % zs(:) = 0.0001;
                % plot(t_t, zs, 'k');

                % 2
                cla; % �����
                % legend('x', 'y', 'roll', 'pitch', 'yaw');

                % 3
                % oa
                plot(t_t, t_od(:,1), 'r');
                plot(t_t, t_od(:,2), 'b');
                % plot(t_t, t_od(:,3), 'y');

                % ra
                plot(t_t, t_ra(:,1), 'k');
                plot(t_t, t_ra(:,2), 'm');
                % plot(t_t, t_ra(:,3), 'y');

                % euler
                % plot(t_t, t_od(:,4), 'b');
                % plot(t_t, t_od(:,5), 'm');
                % plot(t_t, t_od(:,6), 'g');


            else
                % continue;
                t_od = six_axes_data;
                t_ra = rotated_accelerate_data;
                t_t  = time;


                % zs = zeros(i);
                % zs(:) = 0.0001;
                % plot(t_t(1:i), zs, 'k');


                cla; % �����
                % title(['[��ʼ��][', num2str(i), ']']);
                % legend('x', 'y', 'roll', 'pitch', 'yaw');
                % 3.
                % oa
                plot(t_t(1:i), six_axes_data(1:i,1), 'r');
                plot(t_t(1:i), six_axes_data(1:i,2), 'b');
                % plot(time, six_axes_data(:,3), 'r');

                % ra
                plot(t_t(1:i), t_ra(1:i,1), 'k');
                plot(t_t(1:i), t_ra(1:i,2), 'm');
                % plot(time, rotated_accelerate_data(:,3), 'y');

                % euler
                % plot(t_t(1:i), t_od(1:i,4), 'b');
                % plot(t_t(1:i), t_od(1:i,5), 'm');
                % plot(t_t(1:i), t_od(1:i,6), 'g');
            end
        end

    %%%


    %%% ��
        if i == 2
            % legend('roll', 'pitch', 'yaw');
            % legend('o', 'r');
            % legend('x', 'y', 'z');
            if is_only_rotated
                legend('x', 'y', 'roll', 'pitch', 'yaw');
            end
            if is_both
                % legend('o x', 'o y', 'r x', 'r y', 'yaw');
                % legend('o x', 'r x', 'yaw');
                % legend('o x', 'r x', 'roll', 'yaw', 'pitch');
                % legend('o x', 'o y', 'r x', 'r y');
            end
            
        end

        % if i == 10
        %     cla;
        %     disp('p92');
        %     % pause(5);
        % end

        drawnow;
        pause(0.001);
        % pause(0.01);
    %%%
end

% 4.
% 
% 
% 
% 
% 




