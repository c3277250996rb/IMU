tic; % 开始计时
addpath('tools');
addpath('data');
addpath('muti_data');
addpath('scripts');
addpath('balance');


N = public_N + 1;
time = zeros(N);
all_data = zeros(N,6);
x = zeros(N);

is_use_uart = 0;

if is_use_uart
    % 打开串口
    close_uart();
    s = open_uart(22);
else

    % all_data = readtable('return_fast_3cm.csv');
    % all_data = readtable('return_fast_20cm.csv');

    % all_data = readtable('return_slow_3cm.csv');
    % all_data = readtable('return_slow_20cm.csv');

    % all_data = readtable('shake_1cm.csv');
    % all_data = readtable('shake_2mm.csv');
    all_data = readtable('shake_double.csv');

    % all_data = readtable('single_fast_3cm.csv');
    % all_data = readtable('single_fast_20cm.csv');

    % all_data = readtable('single_slow_3cm.csv');
    % all_data = readtable('single_slow_20cm.csv');

    x = all_data.x;
end

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

% 状态标志 (default: 0)
% 0: 静止
% 1: 震动
% 2: 移动
flag_status = 0;

scan_begin_index = 50
scan_move_avg_size = 50;
scan_stop_avg_size = 50;

mean_is_move = 0;
mean_is_stop = 0;
mean_before_move = 0;
% mean_after_move = 0;

count_stop_times = 0;
judge_stop_times = 50;

threshold_is_move = 200;
threshold_is_stop = 400;


% 死循环
while true

    if is_use_uart
        % 读取串口
        all_data(index_while,:) = com2data(s);
        x(index_while) = all_data(index_while, 1);
    end

    if index_while > scan_begin_index
        
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
            input_data  = x;
            input_start = index_move_start;
            input_finish= index_move_finish;
            input_mean  = mean_before_move;
            output_status   = ukf_balance(input_data, input_start, input_finish, input_mean);

            flag_status = output_status;
            disp(['[时间]: ', char(datetime('now'))]);
            disp(['[状态]: ', num2str(flag_status)]);
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
        break;
    end

end



elapsedTime = toc; % 结束计时并获取所用时间
fprintf('代码块执行时间为 %.4f 秒\n', elapsedTime);
