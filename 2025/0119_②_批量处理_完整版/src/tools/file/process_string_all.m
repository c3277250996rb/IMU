% ��ȡ�ļ�

file = 'circle_r-0.1_01';
filename = [file, '.csv'];  % �滻Ϊʵ���ļ���
output_dir = 'data\storage\mach3\mutied\';
output_file = [output_dir, file, '_mutied.csv'];  % ����ļ���
delimiter = ',';  % �滻Ϊʵ�ʷָ���
target_column = 3;  % ָ���б��




% �������ļ�
fid = fopen(filename, 'r');
if fid == -1
    error('�޷����ļ� %s', filename);
end

% ������ļ�
out_fid = fopen(output_file, 'w');
if out_fid == -1
    fclose(fid);
    error('�޷���������ļ� %s', output_file);
end

% ���ж�ȡ������
line = fgetl(fid);
while ischar(line)
    % ���ָ����ָ�
    parts = strsplit(line, delimiter);
    
    % ������Ƿ����
    if length(parts) >= target_column
        original_str = parts{target_column};
        
        % �ҵ�С���㲢���� 6 ���ַ�
        dot_index = strfind(original_str, '.');
        if ~isempty(dot_index)
            
            modified_str = process_string(original_str);

            % �滻Ŀ��������
            parts{target_column} = modified_str;
        end
    end
    
    % д�����ļ�
    fprintf(out_fid, '%s\n', strjoin(parts, delimiter));
    
    % ��ȡ��һ��
    line = fgetl(fid);
end




% �ر��ļ�
fclose(fid);
fclose(out_fid);

disp('������ɣ�');
