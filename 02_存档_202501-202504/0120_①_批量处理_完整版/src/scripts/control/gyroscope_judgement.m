function output_extra_status = gyroscope_judgement(input_data)
    temp_difference = zeros(3);
    window_size = 10;
    difference_threshold = 10;

    for i = 1:3
        if i == 3
            continue; % yaw 航向角不处理
        end
        t_i_d = input_data(:,i);
        temp_difference(i) = abs(mean(t_i_d(1:window_size)) - mean(t_i_d(end-window_size:end)));
        
        if temp_difference(i) > difference_threshold
            output_extra_status = 1;
            return;
        end
    end

    % otherwise
    output_extra_status = 0;

end