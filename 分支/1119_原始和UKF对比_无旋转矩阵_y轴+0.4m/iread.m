function output = iread(filename, startRow, startCol)
    % 读取 CSV 文件的指定行列数据
    startRow = startRow - 1;          % 起始行 (从 0 开始计数)
    startCol = startCol - 1;          % 起始列 (从 0 开始计数)
    rangeData = csvread(filename, startRow, startCol, [startRow startCol startRow startCol]);

    % 输出结果
    % disp('读取的指定数据:');
    % disp(rangeData);
    output = rangeData;
end
