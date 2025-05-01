tic; % ��ʼ��ʱ
addpath(genpath('data'));
addpath(genpath('main'));
addpath(genpath('src'));

% �������� ���� ��ʼ��

    N = get_public_N() + 1;
    time = zeros(N);
    all_data = zeros(N,6);
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


% ʹ�ô������
if is_use_uart
    % �򿪴���
    close_uart();
    s = open_uart(get_com_number());

% ���ô������ȡ�ļ�
else
    all_data = readtable(public_csv());

    x = all_data.x;
end


% ��ѭ��
while true
% (ʵʱ����)
    if is_use_uart
        % ��ȡ����
        while retryCount < maxRetries
            try
                all_data(index_while,:) = com2data(s);
                x(index_while) = all_data(index_while, 1); % x��
                % x(index_while) = all_data(index_while, 2); % y��
                break;
            catch ME
                % ���������ʾ������Ϣ
                retryCount = retryCount + 1;
                fprintf('ִ��ʧ��: %s\n', ME.message);
                fprintf('���Ե� %d ��...\n', retryCount);
                % ����Ƿ�ﵽ������Դ���
                if retryCount >= maxRetries
                    close_uart();
                    fprintf('�ﵽ������Դ�����ִֹͣ�С�\n');
                end
            end
        end

        % ��ʼ����Ϣ
        if index_while < 2*scan_move_avg_size
            disp(['[', flag_status_string, '][', num2str(index_while), ']: ', num2str(x(index_while))]);
        end
        if index_while == 100
            disp('[��ʼ��][���]');
        end
    end

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
                end
            end
        %

        % ���: �˶� -> ��ֹ  (��ȡ��������: index_move_finish)
            if flag_movement == 1
                mean_is_stop = mean( x(index_while-scan_stop_avg_size : index_while) );

                % abs(��ǰ - ��ֵ) < ��ֹ��ֵ ��
                if abs( mean_is_stop - x(index_while) ) < threshold_is_stop
                    count_stop_times = count_stop_times + 1; % ��ֹ�����ۼ�

                    if count_stop_times >= judge_stop_times
                        count_stop_times = 0; % �������
                        flag_movement = 0; % ��Ǿ�ֹ
                        index_move_finish = index_while; % ���½�������
                        flag_calc = 1; % ��������
                    end

                end

            end
        %

        % ���� & ״̬
            if flag_calc == 1
                % �Ƿ���ת
                    if input_is_use_rotate_accel
                        input_data = zeros(length(x));
                        % disp(all_data(1:6,1:6));
                        temp_data = rotateAcceleration(all_data);
                        input_data = temp_data(:,1); % x��
                    else
                        input_data = x;
                    end
                %

                % ����
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

                % ��ӡ״̬
                    % flag_status_number = output_status + output_extra_status;
                    flag_status_string = flag_status_num2str(output_status, output_extra_status);
                    disp(['[ʱ��]: ', char(datetime('now'))]);
                    flag_calc_times = flag_calc_times + 1; % �����ۼ�
                    disp(['[״̬][', num2str(flag_calc_times), ']: ', flag_status_string]);
                    disp(['[����][', num2str(flag_calc_times), ']: ', num2str(output_tail_d)]);
                    disp(' '); % �հ׻���
                % 

                % ����
                    index_move_start = 0;
                    index_move_finish = 0;
                    flag_calc = 0;
                    flag_movement = 0;
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
