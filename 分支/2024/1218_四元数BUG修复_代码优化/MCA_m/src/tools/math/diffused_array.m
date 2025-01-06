function output_array = diffused_array(input_K, input_heel, input_size)
    temp_output = zeros(input_size);

    % floor(a / b)������ȡ������ȥС�����֣����������
    % ceil(a / b)������ȡ���������������
    % fix(a / b)��ȡ�������֣����㷽��ȡ������
    % round(a / b)���������롣
    % 1 2 3 4 5

    half_window = ceil(input_size / 2);

    for i = 1:input_size
        temp_output(i) = input_K + (half_window-i) * input_heel;
    end

    output_array = temp_output;


end