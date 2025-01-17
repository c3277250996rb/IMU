function result = process_string(input_str)
    % ����С�����λ��
    dot_pos = strfind(input_str, '.');
    
    if isempty(dot_pos)  % ���û��С����
        % ֱ�ӷ���ԭ�ַ���
        result = input_str;
    else
        % ��ȡС����ǰ�Ĳ���
        int_part = input_str(1:dot_pos-1);
        
        % ��ȡС�����Ĳ��֣�����ȡǰ��λ
        dec_part = input_str(dot_pos+1:end);
        
        % ���С�����ֲ���5λ������0
        if length(dec_part) < 5
            dec_part = [dec_part, repmat('0', 1, 5-length(dec_part))];
        else
            dec_part = dec_part(1:5);
        end
        
        % ƴ�ӽ��
        result = [int_part, dec_part];
    end
end
