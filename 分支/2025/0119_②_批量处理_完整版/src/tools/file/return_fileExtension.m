function output_ext = return_fileExtension(input_path)
    [folderPath, fileName, ext] = fileparts(input_path);
    output_ext = ext;
end