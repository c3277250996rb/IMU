function output_differential = calc_differential(input_data, input_count_size, input_window_size)
    output_differential = zeros(2,input_count_size);

    % 前部
    while i = input_window_size:input_count_size
        output_differential(1:i) = diff( input_data(i-input_count_size : i) );

    end

    % 后部
    while i = 1 : input_window_size-input_count_size
        output_differential(2:i) = diff( input_data(i : i+input_window_size) );
    end
    
end