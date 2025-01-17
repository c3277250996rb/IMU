run('c.m');


% file_name = 'circle_r-0.1_01';
% file_name = 'circle_r-0.1_02';

% file_name = '0';

% file_name = 'x-return';
% file_name = 'x-single';
% file_name = 'y-return';
% file_name = 'y-single';

% file_name = '0117';
% file_name = '0117_��';
% file_name = '0117_circle';
file_name = '0117_single';


% 1. ���ļ�
fid = fopen([file_name, '.csv'], 'r');

% 2. ��ʼ��һ����Ԫ���������ڴ洢ÿһ�е�����
dataCell = {};

% 3. ���ж�ȡ�ļ�����
while ~feof(fid)
    % ��ȡһ������
    line = fgetl(fid);
    % ʹ�ö��ŷָ������뵥Ԫ��
    row = str2double(strsplit(line, ','));
    % ����ǰ�д洢�����ݵ�Ԫ����
    dataCell = [dataCell; row];
end

% 4. �ر��ļ�
fclose(fid);

% 5. ����Ԫ������ת��Ϊ����ȱʧֵ�� NaN ���
old_dataArray = cell2mat(cellfun(@(x) [x, nan(1, max(cellfun(@numel, dataCell)) - numel(x))], dataCell, 'UniformOutput', false));
dataArray = old_dataArray(11:end-10,1:6); % ������β10��

% 6. �ҵ�����ȱʧֵ����
missingRows = any(isnan(dataArray), 2);

% 7. ����ȱʧ���в��޸�
i = 1;
while i <= size(dataArray, 1)
    if missingRows(i)
        % ����ǵ�һ�л����һ�У�����
        if i == 1 || i == size(dataArray, 1)
            i = i + 1;
            continue;
        end
        
        % ���ȱʧֵ�������֣���������Щ����ȱʧ����
        if all(missingRows(i:i+1))
            % ����ѡ���������У����߶���Щ������������
            i = i + 2; % ��������
            continue;
        else
            % ���������еľ�ֵ�ȱʧֵ
            dataArray(i, :) = (dataArray(i - 1, :) + dataArray(i + 1, :)) / 2;
        end
    end
    i = i + 1;
end

%%% �޷�
    % ������ֵ
    threshold =25000;

    % ��ȡÿһ�����Ƿ��о���ֵ������ֵ��Ԫ��
    rows_to_delete = any(abs(dataArray) > threshold, 2);

    % ɾ��������������
    A_filtered = dataArray(~rows_to_delete, :);

    % ���¸�ֵ
    dataArray = A_filtered;
%%%

% 8. ���޸��������д���µ�CSV�ļ�
dir = 'data\storage\mach3\fixed\';
output_path = [dir, file_name, '_fixed.csv'];
% dataArray(:,3) = dataArray(:,3) ./ 1e5;
csvwrite(output_path, dataArray);

insert_first_row(output_path, ['x,y,z,roll,pitch,yaw']);
