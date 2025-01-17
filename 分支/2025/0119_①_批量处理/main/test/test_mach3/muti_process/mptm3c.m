% --- Muti Process Test Mach3 Calculate --- %
function mptm3c(input_path)
    

    tic; % ��ʼ��ʱ


        all_data = readtable(input_path); % ��ȡCSV�ļ�
        all_data_array = table2array(all_data); % ��ȡCSV�ļ�
        [numRows, ~] = size(all_data); % ��ȡ����
        disp(['numRows:', num2str(numRows)]); % ��ʾ����
        output_rows = numRows;
        

        N = output_rows;
        %%% checkForEmptyCSV: 
            
            % ���ÿ���Ƿ��п�ֵ��NaN��
            rowsWithEmpty = any(isnan(all_data_array), 2);  % ���ÿ���Ƿ���NaN�������߼�������
            
            % �ҵ�������ֵ���к�
            emptyRows = find(rowsWithEmpty);  % �ҵ�ֵΪtrue���к�
            
            % ����п�ֵ�У������Щ�к�
            if ~isempty(emptyRows)
                disp('�����а�����ֵ��');
                disp(emptyRows);
            else
                disp('û�з��ֿ�ֵ');
            end
            
        %%%


    % �������� ���� ��ʼ��

        time = zeros(N);
        x = zeros(N);

        % ������Դ���
        maxRetries = 2;
        retryCount = 0;

        is_use_uart = get_is_use_uart();
        input_is_use_rotate_accel = get_is_use_rotate_accel();

        %

        index_while = 1;
        index_move_start = 0;
        index_move_finish = 0;

        % �˶���־ (default: 0)
        % 0: ��ֹ
        % 1: �˶�
        flag_movement = 0;

        % �����־ (default: 0)
        % 0: �ر�
        % 1: ����
        flag_calc = 0;
        flag_calc_times = 0;

        % ״̬��־ (default: 0)
        % 0: ��ֹ
        % 1: ��
        % 2: ����
        % 3: �ƶ�
        flag_status_number = 0;
        flag_status_string = '��ʼ��';

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
        


    % ��ѭ��
    while true
    % (ʵʱ����)

    % �˶�ʶ��
        if index_while > 2*scan_move_avg_size
            % ���: ��ֹ -> �˶�  (��ȡ��ʼ����: index_move_start)
                if flag_movement == 0
                    mean_is_move = mean( x(index_while - scan_move_avg_size : index_while) );

                    % abs(��ǰ - ��ֵ) > �˶���ֵ ��
                    if abs( x(index_while) - mean_is_move ) > threshold_is_move
                        flag_movement = 1; % ����˶�
                        % index_move_start = index_while; % ������ʼ����
                        index_move_start = index_while - scan_move_avg_size; % ������ʼ����
                        mean_before_move = mean( x(index_while - 2*scan_move_avg_size : index_while - scan_move_avg_size) ); % ������Ʈ

                    
                %%% ǿ�ƻ�����
                        index_move_start = 200;
                        index_move_finish = N - 200; % ���½�������
                %%% 


                        flag_calc = 1; % ��������
                    end

                end
            %

            % ���: �˶� -> ��ֹ  (��ȡ��������: index_move_finish)
                % if flag_movement == 1
                %     mean_is_stop = mean( x(index_while-scan_stop_avg_size : index_while) );

                %     % abs(��ǰ - ��ֵ) < ��ֹ��ֵ ��
                %     if abs( mean_is_stop - x(index_while) ) < threshold_is_stop
                %         count_stop_times = count_stop_times + 1; % ��ֹ�����ۼ�

                %         if count_stop_times >= judge_stop_times
                %             count_stop_times = 0; % �������
                %             flag_movement = 0; % ��Ǿ�ֹ
                %             index_move_finish = index_while; % ���½�������
                %             flag_calc = 1; % ��������
                %         end

                %     end

                % end
            %

            % ���� & ״̬
                if flag_calc == 1
                    % �Ƿ���ת
                        if input_is_use_rotate_accel
                            temp_data = rotateAcceleration(all_data);
                            all_data(:,1:3) = temp_data;

                        else
                            % input_data = x;

                        end
                    %

                    % ����
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

                    % 3D ��ͼ
                    
                        size_ = 20;
                        size_time_3 = size_ * 1;
                        size_time_3_plus_1 = size_ + 1;
                        time = (1:1:calc_length);

                        figure_hold_box();

                        %%% D
                            % subplot(row, colume, location);
                            subplot(1, 3, 1);
                            hold on; box on;

                            % D ���� ԭʼ
                                plot_data = zeros(calc_length,2);
                                plot_data(:,1) = calc_data(:,4,1);
                                plot_data(:,2) = calc_data(:,4,2);
                                % plot(plot_data(1:size_time_3,1), plot_data(1:size_time_3,2), 'r'); % head
                                plot(plot_data(1:40,1), plot_data(1:40,2), 'r'); % head
                                plot(plot_data(size_time_3_plus_1:end-size_time_3_plus_1,1), plot_data(size_time_3_plus_1:end-size_time_3_plus_1,2), 'k'); % middle
                                plot(plot_data(end-size_:end,1), plot_data(end-size_:end,2), 'g'); % tail
                            %

                            % D ���� ����
                                plot_data = zeros(calc_length,2);
                                plot_data(:,1) = calc_data(:,5,1);
                                plot_data(:,2) = calc_data(:,5,2);
                                % plot(plot_data(1:size_time_3,1), plot_data(1:size_time_3,2), 'r'); % head
                                plot(plot_data(1:40,1), plot_data(1:40,2), 'r'); % head
                                plot(plot_data(size_time_3_plus_1:end-size_time_3_plus_1,1), plot_data(size_time_3_plus_1:end-size_time_3_plus_1,2), 'b'); % middle
                                plot(plot_data(end-size_:end,1), plot_data(end-size_:end,2), 'g'); % tail

                                % output_I_index = output_map('output_I_index');
                                % plot(plot_data(output_I_index,1), plot_data(output_I_index,2), 'r+');
                            %

                            % Reference Line
                                temp1 = zeros(calc_length,1);
                                temp1 = 50;
                                temp2 = (1:1:calc_length);
                                temp3 = zeros(calc_length,1);
                                temp3 = -50;
                                plot(plot_data(:,1),temp1,'w');
                                plot(plot_data(:,1),temp3,'w');
                                plot(temp1,plot_data(:,2),'w');
                                plot(temp3,plot_data(:,2),'w');
                            %

                            xlabel('X - D'); % ��� x ���ǩ
                            ylabel('Y - D'); % ��� y ���ǩ
                            drawnow;
                        %%% 


                        %%% X
                            % subplot(row, colume, location);
                            subplot(1, 3, 2);
                            hold on; box on;

                            for i = 1:5
                                plot(time,calc_data(:,i,1));
                            end

                            xlabel('x'); % ��� x ���ǩ

                            if get_is_use_direct_intergration
                                legend('a', 'u-v', 'o-v', 'u-d', 'o-d');
                            end
                            drawnow;
                        %%%


                        %%% Y
                            % subplot(row, colume, location);
                            subplot(1, 3, 3);
                            hold on; box on;

                            for i = 1:5
                                plot(time,calc_data(:,i,2));
                            end

                            xlabel('y'); % ��� x ���ǩ

                            if get_is_use_direct_intergration
                                legend('a', 'u-v', 'o-v', 'u-d', 'o-d');
                            end
                            drawnow;
                        %%%


                    % 

                    % ����
                        index_move_start = 0;
                        index_move_finish = 0;
                        flag_calc = 0;
                        flag_movement = 0;
                    %

                    % ���ʹ��ʹ�ô���
                        if 0 == get_is_use_uart
                            break;
                        end
                    %
                end
            %
        end

    % (�˳�ѭ��)
        index_while = index_while + 1;
        if index_while == (N - 1)
            if is_use_uart == 0
                break;
            end
        end

    end



    elapsedTime = toc; % ������ʱ����ȡ����ʱ��
    fprintf('�����ִ��ʱ��Ϊ %.4f ��\n', elapsedTime);


end