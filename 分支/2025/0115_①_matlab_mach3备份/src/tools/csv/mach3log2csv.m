% file_name = 'circle_r-0.1_01';
file_name = 'circle_r-0.1_02';


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
for i = find(missingRows)'
    % ����ǵ�һ�л����һ�У�����
    if i == 1 || i == size(dataArray, 1)
        continue;
    end
    % ���������еľ�ֵ�ȱʧֵ
    dataArray(i, :) = (dataArray(i - 1, :) + dataArray(i + 1, :)) / 2;
end



% 8. ���޸��������д���µ�CSV�ļ�

dir = 'data\storage\mach3\fixed\';
output_path = [dir, file_name, '_fixed.csv'];
% dataArray(:,3) = dataArray(:,3) ./ 1e5;
csvwrite(output_path, dataArray);


insert_first_row(output_path, ['x,y,z,roll,pitch,yaw']);
