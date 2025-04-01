run('c.m');


% file_name = 'circle_r-0.1_01';
% file_name = 'circle_r-0.1_02';

% file_name = '0';

% file_name = 'x-return';
% file_name = 'x-single';
% file_name = 'y-return';
% file_name = 'y-single';

% file_name = '0117';
% file_name = '0117_②';
% file_name = '0117_circle';
file_name = '0117_single';


% 1. 打开文件
fid = fopen([file_name, '.csv'], 'r');

% 2. 初始化一个单元格数组用于存储每一行的数据
dataCell = {};

% 3. 按行读取文件内容
while ~feof(fid)
    % 读取一行数据
    line = fgetl(fid);
    % 使用逗号分隔并存入单元格
    row = str2double(strsplit(line, ','));
    % 将当前行存储到数据单元格中
    dataCell = [dataCell; row];
end

% 4. 关闭文件
fclose(fid);

% 5. 将单元格数据转换为矩阵，缺失值以 NaN 填充
old_dataArray = cell2mat(cellfun(@(x) [x, nan(1, max(cellfun(@numel, dataCell)) - numel(x))], dataCell, 'UniformOutput', false));
dataArray = old_dataArray(11:end-10,1:6); % 丢弃首尾10行

% 6. 找到包含缺失值的行
missingRows = any(isnan(dataArray), 2);

% 7. 遍历缺失的行并修复
i = 1;
while i <= size(dataArray, 1)
    if missingRows(i)
        % 如果是第一行或最后一行，跳过
        if i == 1 || i == size(dataArray, 1)
            i = i + 1;
            continue;
        end
        
        % 如果缺失值连续出现，则跳过这些连续缺失的行
        if all(missingRows(i:i+1))
            % 这里选择跳过两行，或者对这些行做其他处理
            i = i + 2; % 跳过两行
            continue;
        else
            % 用相邻两行的均值填补缺失值
            dataArray(i, :) = (dataArray(i - 1, :) + dataArray(i + 1, :)) / 2;
        end
    end
    i = i + 1;
end

%%% 限幅
    % 设置阈值
    threshold =25000;

    % 获取每一行中是否有绝对值超过阈值的元素
    rows_to_delete = any(abs(dataArray) > threshold, 2);

    % 删除符合条件的行
    A_filtered = dataArray(~rows_to_delete, :);

    % 更新赋值
    dataArray = A_filtered;
%%%

% 8. 将修复后的数据写入新的CSV文件
dir = 'data\storage\mach3\fixed\';
output_path = [dir, file_name, '_fixed.csv'];
% dataArray(:,3) = dataArray(:,3) ./ 1e5;
csvwrite(output_path, dataArray);

insert_first_row(output_path, ['x,y,z,roll,pitch,yaw']);
