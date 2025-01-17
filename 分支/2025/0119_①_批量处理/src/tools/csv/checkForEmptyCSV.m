function checkForEmptyCSV(filename)
    % 读取CSV文件
    data = csv2array();  % 使用readmatrix读取数值数据
    
    % 检查每行是否有空值（NaN）
    rowsWithEmpty = any(isnan(data), 2);  % 检查每行是否有NaN，返回逻辑列向量
    
    % 找到包含空值的行号
    emptyRows = find(rowsWithEmpty);  % 找到值为true的行号
    
    % 如果有空值行，输出这些行号
    if ~isempty(emptyRows)
        disp('以下行包含空值：');
        disp(emptyRows);
    else
        disp('没有发现空值');
    end
end
