function create_folder(input_path)
    folderPath = return_folderPath(input_path);
    fileName = 'data.txt';  % �ļ���
    filePath = fullfile(folderPath, fileName);  % �������ļ�·��

    % ����ļ����Ƿ����
    if exist(folderPath, 'dir') ~= 7
        mkdir(folderPath);  % �ļ��в�����ʱ����
        disp('�ļ����Ѵ���');
    end
end