% 读取文件

file = 'circle_r-0.1_01';
filename = [file, '.csv'];  % 替换为实际文件名
output_dir = 'data\storage\mach3\mutied\';
output_file = [output_dir, file, '_mutied.csv'];  % 输出文件名
delimiter = ',';  % 替换为实际分隔符
target_column = 3;  % 指定列编号




% 打开输入文件
fid = fopen(filename, 'r');
if fid == -1
    error('无法打开文件 %s', filename);
end

% 打开输出文件
out_fid = fopen(output_file, 'w');
if out_fid == -1
    fclose(fid);
    error('无法创建输出文件 %s', output_file);
end

% 逐行读取并处理
line = fgetl(fid);
while ischar(line)
    % 按分隔符分割
    parts = strsplit(line, delimiter);
    
    % 检查列是否存在
    if length(parts) >= target_column
        original_str = parts{target_column};
        
        % 找到小数点并右移 6 个字符
        dot_index = strfind(original_str, '.');
        if ~isempty(dot_index)
            
            modified_str = process_string(original_str);

            % 替换目标列内容
            parts{target_column} = modified_str;
        end
    end
    
    % 写入新文件
    fprintf(out_fid, '%s\n', strjoin(parts, delimiter));
    
    % 读取下一行
    line = fgetl(fid);
end




% 关闭文件
fclose(fid);
fclose(out_fid);

disp('处理完成！');
