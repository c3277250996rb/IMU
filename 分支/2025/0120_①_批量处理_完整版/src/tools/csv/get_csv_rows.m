function output_rows = get_csv_rows()

    data = readtable(public_csv());; % ��ȡCSV�ļ�
    [numRows, ~] = size(data); % ��ȡ����
    disp(['numRows:', num2str(numRows)]); % ��ʾ����
    output_rows = numRows;

end
