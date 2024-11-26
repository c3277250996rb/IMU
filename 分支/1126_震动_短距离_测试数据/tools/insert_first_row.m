function insert_first_row(filename, newString)
    % 文件名
    % filename = 'data.csv';
    % newString = '这是第一行插入的内容'; % 指定字符串

    % 读取原始CSV内容
    fileData = fileread(filename); 

    % 将新字符串插入到原始内容的第一行
    newFileData = sprintf('%s\n%s', newString, fileData);

    % 写回CSV文件
    fid = fopen(filename, 'w');
    if fid == -1
        error('无法打开文件');
    end
    fprintf(fid, '%s', newFileData);
    fclose(fid);

    disp('新字符串已插入到第一行');
end
