tic; % 开始计时
addpath('tools');
addpath('data');
addpath('muti_data');
addpath('scripts');
addpath('balance');
addpath('RealTimeData');


N = public_N + 1;
time = zeros(N);
all_data = zeros(N,6);
x = zeros(N);

is_use_uart = 1;
is_move_long_time = 1;
input_is_use_rotate_accel = 1;
is_use_RealTimeReadWriteCSV = 1;

if is_use_uart
    % 打开串口
    close_uart();
    s = open_uart(23);
else

    % all_data = readtable('return_fast_3cm.csv');
    % all_data = readtable('return_fast_20cm.csv');

    % all_data = readtable('return_slow_3cm.csv');
    % all_data = readtable('return_slow_20cm.csv');

    % all_data = readtable('shake_1cm.csv');
    % all_data = readtable('shake_2mm.csv');
    % all_data = readtable('shake_double.csv');

    % all_data = readtable('single_fast_3cm.csv');
    % all_data = readtable('single_fast_20cm.csv');

    % all_data = readtable('single_slow_3cm.csv');
    % all_data = readtable('single_slow_20cm.csv');

    % all_data = readtable('484.csv');

    all_data = readtable('0.8mps_0.8m_return_2.csv');

    % all_data = readtable('0.8mps_0.8m_single_1.csv');
    % all_data = readtable('0.8mps_0.8m_single_2.csv');

    % all_data = readtable('0.8mps_2m_single_1.csv');
    % all_data = readtable('0.8mps_2m_single_2.csv');
    % all_data = readtable('0.8mps_2m_single_3.csv');

    x = all_data.x;
end

% 常规变量
    index_while = 1;
    index_move_start = 0;
    index_move_finish = 0;

    % 运动标志 (default: 0)
    % 0: 静止
    % 1: 运动
    flag_movement = 0;

    % 计算标志 (default: 0)
    % 0: 关闭
    % 1: 开启
    flag_calc = 0;
    flag_calc_times = 0;

    % 状态标志 (default: 0)
    % 0: 静止
    % 1: 震动
    % 2: 往返
    % 3: 移动
    flag_status_number = 0;
    flag_status_string = '初始化';

    scan_begin_index = 50;
    scan_move_avg_size = 50;
    scan_stop_avg_size = 50;

    mean_is_move = 0;
    mean_is_stop = 0;
    mean_before_move = 0;
    % mean_after_move = 0;

    count_stop_times = 0;
                            j_s_t_t = 1;
    judge_stop_times = 50 * j_s_t_t;

    threshold_is_move = 200;
    threshold_is_stop = 400;
%


% 死循环
while true

    if is_use_uart
        % 读取串口
        all_data(index_while,:) = com2data(s);
        x(index_while) = all_data(index_while, 1);
        if index_while < 2*scan_move_avg_size
            disp(['[', flag_status_string, '][', num2str(index_while), ']: ', num2str(x(index_while))]);
        end

    end

    if index_while > 2*scan_move_avg_size
        
    % 检测: 静止 -> 运动
        if flag_movement == 0
            mean_is_move = mean( x(index_while - scan_move_avg_size : index_while) );

            % abs(当前 - 均值) > 运动阈值 吗
            if abs( x(index_while) - mean_is_move ) > threshold_is_move
                flag_movement = 1; % 标记运动
                % index_move_start = index_while; % 更新起始索引
                index_move_start = index_while - scan_move_avg_size; % 更新起始索引
                mean_before_move = mean( x(index_while - 2*scan_move_avg_size : index_while - scan_move_avg_size) ); % 更新零飘
            end
        end
    %

    % 检测: 运动 -> 静止
        if flag_movement == 1
            mean_is_stop = mean( x(index_while-scan_stop_avg_size : index_while) );

            % abs(当前 - 均值) < 静止阈值 吗
            if abs( mean_is_stop - x(index_while) ) < threshold_is_stop
                count_stop_times = count_stop_times + 1; % 静止次数累加

                if count_stop_times >= judge_stop_times
                    count_stop_times = 0; % 清除计数
                    flag_movement = 0; % 标记静止
                    index_move_finish = index_while; % 更新结束索引
                    flag_calc = 1; % 开启计算
                end

            end

        end
    %

    % 计算状态
        if flag_calc == 1
            % 更新状态

            if is_use_RealTimeReadWriteCSV
                % 获取当前时间
                currentTime = datetime('now', 'Format', 'yyyy-MM-dd_HH-mm-ss');
                % 转换为字符串              1234567890123456789
                ft = datestr(currentTime, 'yyyy-mm-dd_HH-MM-SS');
                formattedTime = [ ...
                                 ft(1:11), ...
                                %  ft(1:5), 'Y', ft(6:8), 'M', ft(9:11), 'D', ...
                                 ft(12:13), 'h', ft(14:16), 'm', ft(17:19), 's'];
                input_folder_name = 'RealTimeData\';
                input_csv_name = [input_folder_name, formattedTime, '.csv'];
                disp(['input_csv_name: ', num2str(input_csv_name)]);
                csvwrite(input_csv_name, input_data);
                % 修改表头
                modify_first_row(input_csv_name, {'x', 'y', 'z', 'roll', 'pitch', 'yaw'});
                pause(0.5);
                RT_all_data = readtable(input_csv_name);
                RT_x = RT_all_data.x;
            end


            % if input_is_use_rotate_accel
            %     input_data = zeros(length(x));
            %     % disp(all_data(1:6,1:6));
            %     temp_data = rotateAcceleration(all_data, is_use_uart);
            %     input_data = temp_data(:,1); % x列
            % else
            %     input_data = x;
            % end

            if input_is_use_rotate_accel
                input_data = zeros(length(x));
                % disp(all_data(1:6,1:6));
                temp_data = rotateAcceleration(RT_all_data, is_use_uart);
                input_data = temp_data(:,1); % x列
            else
                input_data = RT_x;
            end

            input_start = index_move_start;
            input_finish= index_move_finish;
            input_mean  = mean_before_move;
            %
            input_judge_stop_times = judge_stop_times;
            input_is_use_uart = is_use_uart;
            output_status   = ukf_balance(input_data, input_start, input_finish, input_mean, input_is_use_rotate_accel, input_judge_stop_times, input_is_use_uart);
            output_status   = ukf_balance(     RT_x, input_start, input_finish, input_mean,             0            , input_judge_stop_times, input_is_use_uart);

            flag_status_number = output_status;
            flag_status_string = flag_status_num2str(flag_status_number);
            disp(['[时间]: ', char(datetime('now'))]);
            flag_calc_times = flag_calc_times + 1; % 次数累加
            disp(['[状态][', num2str(flag_calc_times), ']: ', flag_status_string]);
            disp(' '); % 空白换行
            % 清理
            index_move_start = 0;
            index_move_finish = 0;
            flag_calc = 0;
            flag_movement = 0;

            % 退出循环
            % break;
        end
    %

    end

    index_while = index_while + 1;
    % 退出循环
    if index_while == public_N
        if is_use_uart == 0
            disp('index_while == public_N');
            break;
        end
    end

end



elapsedTime = toc; % 结束计时并获取所用时间
fprintf('代码块执行时间为 %.4f 秒\n', elapsedTime);
