% --- Muti Process Test Mach3 --- %


is_calc = 1;


% 1. csvԤ����
input_path = 'data\storage\mach3\muti_process\input\';
output_path = 'data\storage\mach3\muti_process\output\';


% folder_path = '0117\x_return';
% folder_path = '0117\y_return';
% folder_path = '0117\x_single';
% folder_path = '0117\y_single';
folder_path = '0117\circle';


folder_path = [input_path, folder_path];
csv_cell = findCsvFiles(folder_path);

length_csv_cell = numel(csv_cell);
for i = 1:length_csv_cell
    input_path = cell2mat(csv_cell(i));
    % input_path = sprintf('%s', input_path);

    mpm3l2c(input_path);

    persentage = i / length_csv_cell;
    persentage = sprintf('%d', persentage * 100);  % ��ʽ��Ϊ�ַ���
    disp(['[Ԥ����]����: ', persentage, '% --- ', input_path]);

end


% 2. ѭ������, ��ͼ
if is_calc

    for i = 1:length_csv_cell
        input_path = cell2mat(csv_cell(i));
        input_path = strrep(input_path, '\input\', '\output\');
        % input_path = sprintf('%s', input_path);
        
        mptm3c(input_path);

        persentage = i / length_csv_cell;
        persentage = sprintf('%d', persentage * 100);  % ��ʽ��Ϊ�ַ���
        disp(['[����]����: ', persentage, '% --- ', input_path]);
    end


end


