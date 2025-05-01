function output_name = return_fileName(input_path)
    [folderPath, fileName, ext] = fileparts(input_path);
    output_name = fileName;
end