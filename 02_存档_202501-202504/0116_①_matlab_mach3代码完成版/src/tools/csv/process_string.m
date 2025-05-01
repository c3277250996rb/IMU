function result = process_string(input_str)
    % 查找小数点的位置
    dot_pos = strfind(input_str, '.');
    
    if isempty(dot_pos)  % 如果没有小数点
        % 直接返回原字符串
        result = input_str;
    else
        % 提取小数点前的部分
        int_part = input_str(1:dot_pos-1);
        
        % 提取小数点后的部分，并截取前五位
        dec_part = input_str(dot_pos+1:end);
        
        % 如果小数部分不够5位，补充0
        if length(dec_part) < 5
            dec_part = [dec_part, repmat('0', 1, 5-length(dec_part))];
        else
            dec_part = dec_part(1:5);
        end
        
        % 拼接结果
        result = [int_part, dec_part];
    end
end
