% function output_map = csv2data()
function output_array = csv2array()
    
    % example
        % ����һ�� map
        % keySet = {'a', 'b', 'c'}; % ��
        % valueSet = [1, 2, 3];     % ֵ
        % public_map = containers.Map(keySet, valueSet);
        output_map = containers.Map();

        % % ����ֵ
        % disp(public_map('a')); % �����1

        % % ����¼�ֵ��
        % public_map('d') = 4;

        % % ɾ����ֵ��
        % remove(public_map, 'b');

        % % �����Ƿ����
        % isKey(public_map, 'a'); % ���� true �� false

        % % ��ȡ���м�
        % keys(public_map);

        % % ��ȡ����ֵ
        % values(public_map);
    %

    all_data_table = readtable(public_csv());
    all_data_array = table2array(all_data_table);
%     for(
    
    % all_data: {x, y, z, roll, pitch, yaw}
    % output_map('all_data')  = all_data_array;

    % output_map('x')     = all_data.x;
    % output_map('y')     = all_data.y;
    % output_map('z')     = all_data.z;

    % output_map('roll')  = all_data.roll;
    % output_map('pitch') = all_data.pitch;
    % output_map('yaw')   = all_data.yaw;


    output_array = all_data_array;

end