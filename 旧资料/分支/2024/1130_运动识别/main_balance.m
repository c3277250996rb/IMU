tic; % ��ʼ��ʱ
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
    % �򿪴���
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

% �˶���־ (default: 0)
% 0: ��ֹ
% 1: �˶�
flag_movement = 0;

% �����־ (default: 0)
% 0: �ر�
% 1: ����
flag_calc = 0;

% ״̬��־ (default: 0)
% 0: ��ֹ
% 1: ��
% 2: �ƶ�
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


% ��ѭ��
while true

    if is_use_uart
        % ��ȡ����
        all_data(index_while,:) = com2data(s);
        x(index_while) = all_data(index_while, 1);
    end

    if index_while > scan_begin_index
        
    % ���: ��ֹ -> �˶�
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

    % ���: �˶� -> ��ֹ
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

    % ����״̬
        if flag_calc == 1
            % ����״̬
            input_data  = x;
            input_start = index_move_start;
            input_finish= index_move_finish;
            input_mean  = mean_before_move;
            output_status   = ukf_balance(input_data, input_start, input_finish, input_mean);

            flag_status = output_status;
            disp(['[ʱ��]: ', char(datetime('now'))]);
            disp(['[״̬]: ', num2str(flag_status)]);
            % ����
            index_move_start = 0;
            index_move_finish = 0;
            flag_calc = 0;
            flag_movement = 0;

            % �˳�ѭ��
            % break;
        end
    %

    end

    index_while = index_while + 1;
    % �˳�ѭ��
    if index_while == public_N
        break;
    end

end



elapsedTime = toc; % ������ʱ����ȡ����ʱ��
fprintf('�����ִ��ʱ��Ϊ %.4f ��\n', elapsedTime);
