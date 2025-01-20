function output_rows = get_csv_rows()

    data = readtable(public_csv());; % 读取CSV文件
    [numRows, ~] = size(data); % 获取行数
    disp(['numRows:', num2str(numRows)]); % 显示行数
    output_rows = numRows;

end
