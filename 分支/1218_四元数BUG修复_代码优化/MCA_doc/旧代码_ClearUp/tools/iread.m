function output = iread(filename, startRow, startCol)
    % ��ȡ CSV �ļ���ָ����������
    startRow = startRow - 1;          % ��ʼ�� (�� 0 ��ʼ����)
    startCol = startCol - 1;          % ��ʼ�� (�� 0 ��ʼ����)
    rangeData = csvread(filename, startRow, startCol, [startRow startCol startRow startCol]);

    % ������
    % disp('��ȡ��ָ������:');
    % disp(rangeData);
    output = rangeData;
end
