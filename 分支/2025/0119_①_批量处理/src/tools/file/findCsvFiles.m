function csvFiles = findCsvFiles(folderPath)

%%% �Ƿ�������ļ���
    is_contain_children_folder = 0;
%%% 

    % ��ʼ���洢 CSV �ļ�����Ԫ������
    csvFiles = {};
    
    % ��ȡ��ǰ�ļ��е�����
    files = dir(folderPath);
    
    % �����ļ�������
    for i = 1:length(files)
        % ���� '.' �� '..'
        if strcmp(files(i).name, '.') || strcmp(files(i).name, '..')
            continue;
        end
        
        % ��������·��
        fullPath = fullfile(folderPath, files(i).name);
        
        % ��������ļ��У��ݹ����
        if files(i).isdir
            if 0 == is_contain_children_folder
                continue;
            else

                % �ݹ��ȡ���ļ����е� CSV �ļ�
                subFolderCsvFiles = findCsvFiles(fullPath);
                

                %%% �ϲ����ļ��е� CSV �ļ�

                % csvFiles = {csvFiles; subFolderCsvFiles}; %#ok<AGROW>
                % csvFiles = [csvFiles; subFolderCsvFiles]; %#ok<AGROW>

                % �� cell1 �е�ÿ��Ԫ��׷�ӵ� cell2 ����
                for i = 1:numel(subFolderCsvFiles)
                    csvFiles{end+1} = subFolderCsvFiles{i};  % �� cell1 ��Ԫ�������ӵ� cell2
                end

                %%%

            end


        else
            % ����Ƿ��� CSV �ļ�
            [~, ~, ext] = fileparts(files(i).name);
            if strcmpi(ext, '.csv') % �Ƚ���չ�������Դ�Сд
                % ��ӵ�����б�
                csvFiles{end+1} = fullPath; %#ok<AGROW>
            end
        end
    end
end

