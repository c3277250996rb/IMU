% ÅĞ¶ÏÁãµã
function output_zero_index = zero_judgement(input_array)
    output_zero_index = [];
    % disp(input_array);
    for i = 2:length(input_array)
        if input_array(i-1) * input_array(i) < 0
            output_zero_index = [output_zero_index, i];
        end
    end
end