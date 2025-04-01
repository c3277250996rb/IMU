% function output_map = csv2data()
function output_array = csv2array()
    
    % example
        % 创建一个 map
        % keySet = {'a', 'b', 'c'}; % 键
        % valueSet = [1, 2, 3];     % 值
        % public_map = containers.Map(keySet, valueSet);
        output_map = containers.Map();

        % % 访问值
        % disp(public_map('a')); % 输出：1

        % % 添加新键值对
        % public_map('d') = 4;

        % % 删除键值对
        % remove(public_map, 'b');

        % % 检查键是否存在
        % isKey(public_map, 'a'); % 返回 true 或 false

        % % 获取所有键
        % keys(public_map);

        % % 获取所有值
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