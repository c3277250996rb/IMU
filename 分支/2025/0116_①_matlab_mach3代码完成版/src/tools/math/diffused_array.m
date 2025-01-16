function output_array = diffused_array(input_K, input_heel, input_size)
    temp_output = zeros(input_size);

    % floor(a / b)：向下取整（舍去小数部分，趋向负无穷）。
    % ceil(a / b)：向上取整（趋向正无穷）。
    % fix(a / b)：取整数部分（朝零方向取整）。
    % round(a / b)：四舍五入。
    % 1 2 3 4 5

    half_window = ceil(input_size / 2);

    for i = 1:input_size
        temp_output(i) = input_K + (half_window-i) * input_heel;
    end

    output_array = temp_output;


end