function output_string = flag_status_num2str(input_number)
    switch input_number
        case 0
            output_string = '��ֹ';
        case 1
            output_string = '��';
        case 2
            output_string = '����';
        case 3
            output_string = '����';
        otherwise
            output_string = ['input_number: ', input_number];
    end
end