function modify_first_row(filename, newFirstRow)
    
    % ��ȡCSV�ļ�
    fid = fopen(filename, 'r'); % ���ļ�

    % ��ȡ������
    rawData = textscan(fid, '%s', 'Delimiter', '\n'); 
    fclose(fid);
    rawData = rawData{1}; % ��ȡʵ������

    % ��ֵ�һ��
    firstRow = strsplit(rawData{1}, ','); % �����ö��ŷָ�

    % ����µ�һ���������Ƿ�ƥ��
    if length(newFirstRow) ~= length(firstRow)
        error('�µ�һ�е�������CSV���ݵ�������ƥ�䣡');
    end

    % �滻��һ��
    rawData{1} = strjoin(newFirstRow, ',');

    % �����޸ĺ��CSV�ļ�
    fid = fopen(filename, 'w'); % ���ļ�����д��
    fprintf(fid, '%s\n', rawData{:}); % д��ÿһ��
    fclose(fid);

    disp('��һ���ѳɹ��޸ģ�');

end