function output_path = return_folderPath(input_path)
    [folderPath, fileName, ext] = fileparts(input_path);
    output_path = folderPath;
end