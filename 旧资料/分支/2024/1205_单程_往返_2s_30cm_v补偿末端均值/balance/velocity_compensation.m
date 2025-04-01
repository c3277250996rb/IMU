function output_velocity = velocity_compensation(input_comp_I_index, input_tail_size, input_near_size, input_accelerate, input_velocity)
    mean_tail_v = mean(input_velocity(end-input_tail_size : end));
    disp(['mean_tail_v(', num2str( length(input_velocity)-input_tail_size ), ':end): ', num2str(mean_tail_v)]);

    % 末端均值 < 0.15  =>  不补偿
    if abs(mean_tail_v) < 0.1
        disp('末端v均值 < 0.1  =>  不补偿');
        output_velocity = input_velocity;

    % 末端均值 > 0.15  =>  补偿
    else
        % 求出a零点
        output_zero_index = zero_judgement( input_accelerate(1 : input_comp_I_index-input_near_size) );
        % 切片出 待补偿的 末端v
        disp(['output_zero_index: ', num2str(output_zero_index(end))]);
        v_to_be_compensated = input_velocity( output_zero_index(end) : end );
        % 补偿
        v_compensated = v_to_be_compensated - mean_tail_v;
        % [前部, 后部]
        output_velocity = [input_velocity( 1 : output_zero_index(end)-1 ), v_compensated];
    end

end


