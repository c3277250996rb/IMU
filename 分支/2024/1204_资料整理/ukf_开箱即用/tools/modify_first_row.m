function modify_first_row(filename, newFirstRow)
    
    % 读取CSV文件
    fid = fopen(filename, 'r'); % 打开文件

    % 读取所有行
    rawData = textscan(fid, '%s', 'Delimiter', '\n'); 
    fclose(fid);
    rawData = rawData{1}; % 获取实际数据

    % 拆分第一行
    firstRow = strsplit(rawData{1}, ','); % 假设用逗号分隔

    % 检查新第一行与列数是否匹配
    if length(newFirstRow) ~= length(firstRow)
        error('新第一行的列数与CSV数据的列数不匹配！');
    end

    % 替换第一行
    rawData{1} = strjoin(newFirstRow, ',');

    % 保存修改后的CSV文件
    fid = fopen(filename, 'w'); % 打开文件进行写入
    fprintf(fid, '%s\n', rawData{:}); % 写入每一行
    fclose(fid);

    disp('第一行已成功修改！');

end