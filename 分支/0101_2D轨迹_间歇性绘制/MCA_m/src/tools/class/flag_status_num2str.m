function output_string = flag_status_num2str(input_status, input_extra_status)
    switch input_status
        case 0
            output_string = '静止';
        case 1
            output_string = '震动';
        case 2
            output_string = '往返';
        case 3
            output_string = '单程';
        otherwise
            output_string = ['input_status: ', input_status];
    end

    if input_extra_status <= 0 && input_status <= 2
        output_string = [output_string, ' - 复位'];
    else
        output_string = [output_string, ' - 报警'];
    end
end

