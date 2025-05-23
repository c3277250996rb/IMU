% --- Muti Process Test Mach3 Calculate --- %
function mptm3c(input_path, legend_cell, input_title, loop_i)
    

    tic; % 开始计时


        all_data = readtable(input_path); % 读取CSV文件
        all_data_array = table2array(all_data); % 读取CSV文件
        [numRows, ~] = size(all_data); % 获取行数
        disp(['numRows:', num2str(numRows)]); % 显示行数
        output_rows = numRows;
        

        N = output_rows;
        %%% checkForEmptyCSV: 
            
            % 检查每行是否有空值（NaN）
            rowsWithEmpty = any(isnan(all_data_array), 2);  % 检查每行是否有NaN，返回逻辑列向量
            
            % 找到包含空值的行号
            emptyRows = find(rowsWithEmpty);  % 找到值为true的行号
            
            % 如果有空值行，输出这些行号
            if ~isempty(emptyRows)
                disp('以下行包含空值：');
                disp(emptyRows);
            else
                disp('没有发现空值');
            end
            
        %%%


    % 控制流程 变量 初始化

        time = zeros(N);
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



    x = all_data.x; % ???
        


    % 死循环
    while true
    % (实时数据)

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

                    
                %%% 强制划区间
                        index_move_start = 200;
                        index_move_finish = N - 200; % 更新结束索引
                %%% 


                        flag_calc = 1; % 开启计算
                    end

                end
            %

            % 检测: 运动 -> 静止  (获取结束索引: index_move_finish)
                % if flag_movement == 1
                %     mean_is_stop = mean( x(index_while-scan_stop_avg_size : index_while) );

                %     % abs(当前 - 均值) < 静止阈值 吗
                %     if abs( mean_is_stop - x(index_while) ) < threshold_is_stop
                %         count_stop_times = count_stop_times + 1; % 静止次数累加

                %         if count_stop_times >= judge_stop_times
                %             count_stop_times = 0; % 清除计数
                %             flag_movement = 0; % 标记静止
                %             index_move_finish = index_while; % 更新结束索引
                %             flag_calc = 1; % 开启计算
                %         end

                %     end

                % end
            %

            % 计算 & 状态
                if flag_calc == 1
                    % 是否旋转
                        if input_is_use_rotate_accel
                            temp_data = rotateAcceleration(all_data);
                            all_data(:,1:3) = temp_data;

                        else
                            % input_data = x;

                        end
                    %

                    % 计算
                        calc_length = index_move_finish - index_move_start;
                        calc_data = zeros(calc_length,5,3);

                        for i = 1:3

                            if i == 1
                                temp_all_data = all_data.x;
                            elseif i == 2
                                temp_all_data = all_data.y;
                            elseif i == 3
                                temp_all_data = all_data.z;
                            end


                            input_mean  = 0;

                            input_start = index_move_start;
                            input_finish= index_move_finish;
                            input_judge_stop_times = judge_stop_times;
                            output_map = ukf_mach3(temp_all_data, input_start, input_finish, input_mean, input_judge_stop_times);
                            calc_data(:,:,i) = output_map('output_calc_data');
                        end



                    %

                    % 3D 画图
                    
                        size_ = 20;
                        size_time_3 = size_ * 1;
                        size_time_3_plus_1 = size_ + 1;
                        time = (1:1:calc_length);

                        % figure_hold_box();

                        %%% D
                            % subplot(row, colume, location);
                            subplot(1, 3, 1);
                            hold on; box on;

                            % D 距离 UKF
                                plot_data = zeros(calc_length,2);
                                plot_data(:,1) = calc_data(:,4,1);
                                plot_data(:,2) = calc_data(:,4,2);
                                % plot(plot_data(1:size_time_3,1), plot_data(1:size_time_3,2), 'r'); % head
                                plot(plot_data(1:40,1), plot_data(1:40,2), 'r'); % head
                                plot(plot_data(size_time_3_plus_1:end-size_time_3_plus_1,1), plot_data(size_time_3_plus_1:end-size_time_3_plus_1,2) ); % middle
                                plot(plot_data(end-size_:end,1), plot_data(end-size_:end,2), 'g'); % tail
                            %

                            % D 距离 直接积分
                                plot_data = zeros(calc_length,2);
                                plot_data(:,1) = calc_data(:,5,1);
                                plot_data(:,2) = calc_data(:,5,2);
                                % plot(plot_data(1:size_time_3,1), plot_data(1:size_time_3,2), 'r'); % head
                                plot(plot_data(1:40,1), plot_data(1:40,2), 'r'); % head
                                plot(plot_data(size_time_3_plus_1:end-size_time_3_plus_1,1), plot_data(size_time_3_plus_1:end-size_time_3_plus_1,2) ); % middle
                                plot(plot_data(end-size_:end,1), plot_data(end-size_:end,2), 'g'); % tail

                                % output_I_index = output_map('output_I_index');
                                % plot(plot_data(output_I_index,1), plot_data(output_I_index,2), 'r+');
                            %

                            reference_size = 75;
                            % Reference Line
                                if 1 == loop_i
                                    temp1 = zeros(calc_length,1);
                                    temp1 = reference_size;
                                    temp2 = (1:1:calc_length);
                                    temp3 = zeros(calc_length,1);
                                    temp3 = 0 - reference_size;
                                    plot(plot_data(:,1),temp1,'w');
                                    plot(plot_data(:,1),temp3,'w');
                                    plot(temp1,plot_data(:,2),'w');
                                    plot(temp3,plot_data(:,2),'w');
                                end
                            %

                            xlabel('X - D'); % 添加 x 轴标签
                            ylabel('Y - D'); % 添加 y 轴标签

                            legend_cell = legend_cell';
                            length(legend_cell);

                            drawnow;
                        %%% 


                        %%% X
                            % subplot(row, colume, location);
                            subplot(1, 3, 2);
                            hold on; box on;

                            % for i = 1:5
                            for i = 4:5
                                plot(time,calc_data(:,i,1));
                            end

                            
                            % Reference Line
                                if 2 == loop_i
                                    temp1 = zeros(calc_length,1);
                                    temp1 = reference_size;
                                    temp2 = (1:1:calc_length);
                                    temp3 = zeros(calc_length,1);
                                    temp3 = 0 - reference_size;
                                    plot(plot_data(:,1),temp1,'w');
                                    plot(plot_data(:,1),temp3,'w');
                                    plot(temp1,plot_data(:,2),'w');
                                    plot(temp3,plot_data(:,2),'w');
                                end
                            %



                            ylabel('x'); % 添加 y 轴标签
                            xlabel(['正在计算: ', input_path]); % 添加 x 轴标签
                            title(input_title);

                            if get_is_use_direct_intergration
                                % legend('a', 'u-v', 'o-v', 'u-d', 'o-d');
                                legend('u-d', 'o-d');
                            end

                            length(legend_cell);
                            drawnow;
                        %%%


                        %%% Y
                            % subplot(row, colume, location);
                            subplot(1, 3, 3);
                            hold on; box on;

                            % for i = 1:5
                            for i = 4:5
                                plot(time,calc_data(:,i,2));
                            end

                            
                            % Reference Line
                                if 3 == loop_i
                                    temp1 = zeros(calc_length,1);
                                    temp1 = reference_size;
                                    temp2 = (1:1:calc_length);
                                    temp3 = zeros(calc_length,1);
                                    temp3 = 0 - reference_size;
                                    plot(plot_data(:,1),temp1,'w');
                                    plot(plot_data(:,1),temp3,'w');
                                    plot(temp1,plot_data(:,2),'w');
                                    plot(temp3,plot_data(:,2),'w');
                                end
                            %


                            ylabel('y'); % 添加 y 轴标签

                            if get_is_use_direct_intergration
                                % legend('a', 'u-v', 'o-v', 'u-d', 'o-d');
                                legend('u-d', 'o-d');
                            end

                            length(legend_cell);
                            drawnow;
                        %%%


                    % 

                    % 清理
                        index_move_start = 0;
                        index_move_finish = 0;
                        flag_calc = 0;
                        flag_movement = 0;
                    %

                    % 如果使不使用串口
                        if 0 == get_is_use_uart
                            break;
                        end
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


end