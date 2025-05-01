function output_array = a2v(input_data, input_string)
    [length_row, length_column] = size(input_data);
    length = min(3,length_column);
    output_array = zeros(length_row, length);

    if input_string == 'list'

        for i = 1:length
            output_array(:,i) = test_gfun(input_data(:,i));
        end

    end
        
end
