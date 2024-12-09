function output_I_index = I_av_intersection(input_accelerate, input_velocity, input_judge_stop_times, input_is_use_uart, input_is_use_rotate_accel)

    %��ֵΪ0.1
    threshold = 0.1;
    cut_size = input_judge_stop_times;

    % disp(input_accelerate(1:3,1:3));
    
    if input_is_use_uart
        % ʹ�ô���
    else
        % ���ô��� => ת�� a
        input_accelerate = input_accelerate';
    end

    % ��ȡ�� input_judge_stop_times ������������ compare_accelerate  compare_velocity
    cut_accelerate = input_accelerate((end-cut_size):end);
    cut_velocity = input_velocity((end-cut_size):end);
    
    %�ٽ�ȡ������ֵ�Ĳ���
    cut_cut_accelerate = cut_accelerate(abs(cut_accelerate) > threshold);
    cut_cut_velocity = cut_velocity(abs(cut_velocity) > threshold);

    % disp(cut_cut_accelerate);
    % disp(cut_cut_velocity);
    
    %ƴ�ӻ�ԭ����
    compare_accelerate = [input_accelerate(1:(end-cut_size-1)),cut_cut_accelerate];
    compare_velocity = [input_velocity(1:(end-cut_size-1)),cut_cut_velocity];
    
    % ��ʼ����������
    index = [];
    
    % input_velocity = input_velocity';
    % �Ƚϲ�ֵ����¼���ű仯�ĵط�
    temp_length = min( length(compare_accelerate), length(compare_velocity));
    for i = 2:temp_length
        if sign(compare_accelerate(i) - compare_velocity(i)) ~= sign(compare_accelerate(i-1) - compare_velocity(i-1))
            index = [index, i];
        end
    end
    % disp(length(index));
    
    %�������
    if length(index)
        output_I_index = index(end);
    else
        output_I_index = 110;
    end
        
end