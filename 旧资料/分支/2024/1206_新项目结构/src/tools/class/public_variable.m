function output_value = public_variables(input_key)

    % example
        % 创建一个 map
        keySet = {'a', 'b', 'c'}; % 键
        valueSet = [1, 2, 3];     % 值
        public_map = containers.Map(keySet, valueSet);

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

    % get_public_N: 全局步长
                                            N_t = 8;
    public_map('get_public_N') = -1 + 100 * N_t;

    % get_is_use_uart: 使用 串口
    public_map('get_is_use_uart') = 0;

    % get_is_use_rotate_accel: 使用 旋转矩阵
    public_map('get_is_use_rotate_accel') = 0;

    
    % % is_use_uart: 使用串口
    % public_map('is_use_uart') = 1;

    % % is_use_uart: 使用串口
    % public_map('is_use_uart') = 1;

    % % is_use_uart: 使用串口
    % public_map('is_use_uart') = 1;

    % % is_use_uart: 使用串口
    % public_map('is_use_uart') = 1;

    % % is_use_uart: 使用串口
    % public_map('is_use_uart') = 1;

    % % is_use_uart: 使用串口
    % public_map('is_use_uart') = 1;



    output_value = public_map(input_key);
end

