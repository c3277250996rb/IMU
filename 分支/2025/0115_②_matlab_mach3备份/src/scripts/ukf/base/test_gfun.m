function output_array = test_gfun(input_data)
    % input_data: a
    [length_row, length_column] = size(input_data);
    length = length_row;
    T = 0.039;

    temp_v = zeros(length_row,1);
    for i = 2:length
        %   v1    =     v0      + (     a0         +        a1    ) /2 *T
        temp_v(i) = temp_v(i-1) + (input_data(i-1) + input_data(i)) /2 *T;
    end
        
%     output_array: v
    output_array = temp_v;
end
