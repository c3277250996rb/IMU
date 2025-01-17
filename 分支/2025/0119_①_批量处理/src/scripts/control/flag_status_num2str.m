function output_string = flag_status_num2str(input_status, input_extra_status)
    switch input_status
        case 0
            output_string = '��ֹ';
        case 1
            output_string = '��';
        case 2
            output_string = '����';
        case 3
            output_string = '����';
        otherwise
            output_string = ['input_status: ', input_status];
    end

    if input_extra_status <= 0 && input_status <= 2
        output_string = [output_string, ' - ��λ'];
    else
        output_string = [output_string, ' - ����'];
    end
end

