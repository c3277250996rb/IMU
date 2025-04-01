function csvFiles = findCsvFiles(folderPath)

%%% 是否包括子文件夹
    is_contain_children_folder = 0;
%%% 

    % 初始化存储 CSV 文件名的元胞数组
    csvFiles = {};
    
    % 获取当前文件夹的内容
    files = dir(folderPath);
    
    % 遍历文件夹内容
    for i = 1:length(files)
        % 跳过 '.' 和 '..'
        if strcmp(files(i).name, '.') || strcmp(files(i).name, '..')
            continue;
        end
        
        % 构造完整路径
        fullPath = fullfile(folderPath, files(i).name);
        
        % 如果是子文件夹，递归调用
        if files(i).isdir
            if 0 == is_contain_children_folder
                continue;
            else

                % 递归获取子文件夹中的 CSV 文件
                subFolderCsvFiles = findCsvFiles(fullPath);
                

                %%% 合并子文件夹的 CSV 文件

                % csvFiles = {csvFiles; subFolderCsvFiles}; %#ok<AGROW>
                % csvFiles = [csvFiles; subFolderCsvFiles]; %#ok<AGROW>

                % 将 cell1 中的每个元素追加到 cell2 后面
                for i = 1:numel(subFolderCsvFiles)
                    csvFiles{end+1} = subFolderCsvFiles{i};  % 将 cell1 的元素逐个添加到 cell2
                end

                %%%

            end


        else
            % 检查是否是 CSV 文件
            [~, ~, ext] = fileparts(files(i).name);
            if strcmpi(ext, '.csv') % 比较扩展名，忽略大小写
                % 添加到结果列表
                csvFiles{end+1} = fullPath; %#ok<AGROW>
            end
        end
    end
end

