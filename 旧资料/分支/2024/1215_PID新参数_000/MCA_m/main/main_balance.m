tic; % 开始计时
addpath(genpath('data'));
addpath(genpath('main'));
addpath(genpath('src'));

% 控制流程 变量 初始化

    N = get_public_N() + 1;
    time = zeros(N);
    all_data = zeros(N,6);
    x = zeros(N);

    % 最大重试次数
    maxRetries = 2;
    retryCount = 0;

    is_use_uart = get_is_use_uart();
    input_is_use_rotate_accel = get_is_use_rotate_accel();

    %

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
    judge_stop_times = get_judge_stop_times();

    threshold_is_move = 200;
    threshold_is_stop = 400;
%


% 使用串口则打开
if is_use_uart
    % 打开串口
    close_uart();
    s = open_uart(get_com_number());

% 不用串口则读取文件
else
    all_data = readtable(public_csv());

    x = all_data.x;
end


% 死循环
while true
% (实时数据)
    if is_use_uart
        % 读取串口
        while retryCount < maxRetries
            try
                all_data(index_while,:) = com2data(s);
                x(index_while) = all_data(index_while, 1); % x轴
                % x(index_while) = all_data(index_while, 2); % y轴
                break;
            catch ME
                % 捕获错误并显示错误信息
                retryCount = retryCount + 1;
                fprintf('执行失败: %s\n', ME.message);
                fprintf('重试第 %d 次...\n', retryCount);
                % 检查是否达到最大重试次数
                if retryCount >= maxRetries
                    close_uart();
                    fprintf('达到最大重试次数，停止执行。\n');
                end
            end
        end

        % 初始化信息
        if index_while < 2*scan_move_avg_size
            disp(['[', flag_status_string, '][', num2str(index_while), ']: ', num2str(x(index_while))]);
        end
        if index_while == 100
            disp('[初始化][完成]');
        end
    end

% 运动识别
    if index_while > 2*scan_move_avg_size
        % 检测: 静止 -> 运动  (获取起始索引: index_move_start)
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

        % 检测: 运动 -> 静止  (获取结束索引: index_move_finish)
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

        % 计算 & 状态
            if flag_calc == 1
                % 是否旋转
                    if input_is_use_rotate_accel
                        input_data = zeros(length(x));
                        % disp(all_data(1:6,1:6));
                        temp_data = rotateAcceleration(all_data);
                        input_data = temp_data(:,1); % x列
                    else
                        input_data = x;
                    end
                %

                % 计算
                    input_start = index_move_start;
                    input_finish= index_move_finish;
                    input_mean  = mean_before_move;
                    input_judge_stop_times = judge_stop_times;
                    input_is_use_uart = is_use_uart;
                    output_map     = ukf_balance(input_data, input_start, input_finish, input_mean, input_judge_stop_times);


                    


                    output_status = output_map('output_status');
                    output_tail_d = output_map('output_tail_d');

                    input_data = all_data( index_move_start:(index_move_finish) , 4:6 );
                    output_extra_status = 0;
%                     output_extra_status = gyroscope_judgement(input_data);
                %

                % 打印状态
                    % flag_status_number = output_status + output_extra_status;
                    flag_status_string = flag_status_num2str(output_status, output_extra_status);
                    disp(['[时间]: ', char(datetime('now'))]);
                    flag_calc_times = flag_calc_times + 1; % 次数累加
                    disp(['[状态][', num2str(flag_calc_times), ']: ', flag_status_string]);
                    disp(['[距离][', num2str(flag_calc_times), ']: ', num2str(output_tail_d)]);
                    disp(' '); % 空白换行
                % 

                % 清理
                    index_move_start = 0;
                    index_move_finish = 0;
                    flag_calc = 0;
                    flag_movement = 0;
                %
            end
        %
    end

% (退出循环)
    index_while = index_while + 1;
    if index_while == (N - 1)
        if is_use_uart == 0
            break;
        end
    end

end



elapsedTime = toc; % 结束计时并获取所用时间
fprintf('代码块执行时间为 %.4f 秒\n', elapsedTime);
