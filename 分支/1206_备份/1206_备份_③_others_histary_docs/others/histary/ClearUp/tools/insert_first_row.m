function insert_first_row(filename, newString)
    % �ļ���
    % filename = 'data.csv';
    % newString = '���ǵ�һ�в��������'; % ָ���ַ���

    % ��ȡԭʼCSV����
    fileData = fileread(filename); 

    % �����ַ������뵽ԭʼ���ݵĵ�һ��
    newFileData = sprintf('%s\n%s', newString, fileData);

    % д��CSV�ļ�
    fid = fopen(filename, 'w');
    if fid == -1
        error('�޷����ļ�');
    end
    fprintf(fid, '%s', newFileData);
    fclose(fid);

    disp('���ַ����Ѳ��뵽��һ��');
end
