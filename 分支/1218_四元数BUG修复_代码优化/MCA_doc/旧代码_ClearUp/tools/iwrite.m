function iwrite(filename, startRow, startCol, data2write)
    % ��ȡ���� CSV �ļ�
    data = csvread(filename);

    % �޸�ָ��λ�õ�����
    data(startRow, startCol) = data2write; % �޸ĵ�2����4�У���2����3��

    % д���ļ�
    csvwrite(filename, data);



end