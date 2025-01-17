function create_folder(input_path)
    folderPath = return_folderPath(input_path);
    fileName = 'data.txt';  % 文件名
    filePath = fullfile(folderPath, fileName);  % 完整的文件路径

    % 检查文件夹是否存在
    if exist(folderPath, 'dir') ~= 7
        mkdir(folderPath);  % 文件夹不存在时创建
        disp('文件夹已创建');
    end
end