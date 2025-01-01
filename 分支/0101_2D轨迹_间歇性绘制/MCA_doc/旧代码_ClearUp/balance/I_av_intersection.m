function output_I_index = I_av_intersection(input_accelerate, input_velocity, input_judge_stop_times, input_is_use_uart, input_is_use_rotate_accel)

    %阈值为0.1
    threshold = 0.1;
    cut_size = input_judge_stop_times;

    % disp(input_accelerate(1:3,1:3));
    
    if input_is_use_uart
        % 使用串口
    else
        % 不用串口 => 转置 a
        input_accelerate = input_accelerate';
    end

    % 截取后 input_judge_stop_times 个步长的数组 compare_accelerate  compare_velocity
    cut_accelerate = input_accelerate((end-cut_size):end);
    cut_velocity = input_velocity((end-cut_size):end);
    
    %再截取大于阈值的部分
    cut_cut_accelerate = cut_accelerate(abs(cut_accelerate) > threshold);
    cut_cut_velocity = cut_velocity(abs(cut_velocity) > threshold);

    % disp(cut_cut_accelerate);
    % disp(cut_cut_velocity);
    
    %拼接回原数组
    compare_accelerate = [input_accelerate(1:(end-cut_size-1)),cut_cut_accelerate];
    compare_velocity = [input_velocity(1:(end-cut_size-1)),cut_cut_velocity];
    
    % 初始化索引数组
    index = [];
    
    % input_velocity = input_velocity';
    % 比较差值并记录符号变化的地方
    temp_length = min( length(compare_accelerate), length(compare_velocity));
    for i = 2:temp_length
        if sign(compare_accelerate(i) - compare_velocity(i)) ~= sign(compare_accelerate(i-1) - compare_velocity(i-1))
            index = [index, i];
        end
    end
    % disp(length(index));
    
    %输出索引
    if length(index)
        output_I_index = index(end);
    else
        output_I_index = 110;
    end
        
end