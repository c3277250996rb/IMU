function output_string = flag_status_num2str(input_number)
    switch input_number
        case 0
            output_string = '¾²Ö¹';
        case 1
            output_string = 'Õğ¶¯';
        case 2
            output_string = 'Íù·µ';
        case 3
            output_string = 'µ¥³Ì';
        otherwise
            output_string = ['input_number: ', input_number];
    end
end