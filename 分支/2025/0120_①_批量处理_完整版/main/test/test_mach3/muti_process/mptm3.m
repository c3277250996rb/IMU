% --- Muti Process Test Mach3 --- %
tic; % 开始计时


is_calc = 1;


% 1. csv预处理
input_path = 'data\storage\mach3\muti_process\input\';
output_path = 'data\storage\mach3\muti_process\output\';


% folder_path = '0117\x_return';
% folder_path = '0117\y_return';
% folder_path = '0117\x_single';
% folder_path = '0117\y_single';
folder_path = '0117\circle';

% folder_path = '0117';

t_f_p = folder_path;
folder_path = [input_path, folder_path];
csv_cell = findCsvFiles(folder_path);

length_csv_cell = numel(csv_cell);
for i = 1:length_csv_cell
    input_path = cell2mat(csv_cell(i));
    % input_path = sprintf('%s', input_path);

    mpm3l2c(input_path);

    persentage = i / length_csv_cell;
    persentage = sprintf('%d', persentage * 100);  % 格式化为字符串
    disp(['[预处理]进度: ', persentage, '% --- ', input_path]);

end


% 2. 循环计算, 绘图
figure_hold_box();
legend_cell = {};
input_title = '';


elapsedTime = 0;
text2title = '';
if is_calc

    for i = 1:length_csv_cell
        input_path = cell2mat(csv_cell(i));
        input_path = strrep(input_path, '\input\', '\output\');
        % input_path = sprintf('%s', input_path);

        persentage = i / length_csv_cell;
        persentage = sprintf('%d', persentage * 100);  % 格式化为字符串
        
        legend_cell(end+1) = {['_', num2str(i)]};
        input_title = ['[', t_f_p, '] --- ', num2str(i), '/', num2str(length_csv_cell), ' --- ', persentage, '%'];
        mptm3c(input_path, legend_cell, input_title, i);

        disp(['[计算]进度: ', persentage, '% --- ', input_path]);

        temp_elapsedTime = toc; % 结束计时并获取所用时间
        elapsedTime = elapsedTime + temp_elapsedTime;
        % text2title = sprintf('%d - 累计用时: %.1f秒', folder_path, elapsedTime);
        text2title = sprintf('累计用时: %.1f秒', elapsedTime);
        title(text2title);
    end

    
    title([text2title, ' [完成]']);

end


