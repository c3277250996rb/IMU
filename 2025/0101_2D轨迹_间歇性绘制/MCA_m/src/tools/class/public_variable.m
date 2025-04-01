function output_value = public_variables(input_key)

    % example
        % 创建一个 map
        % keySet = {'a', 'b', 'c'}; % 键
        % valueSet = [1, 2, 3];     % 值
        % public_map = containers.Map(keySet, valueSet);
        public_map = containers.Map();

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

%%%
    % get_is_use_uart: 使用 串口
    public_map('get_is_use_uart') = 1;

    % get_is_use_rotate_accel: 使用 旋转矩阵
    public_map('get_is_use_rotate_accel') = 0;

    % get_is_save_to_csv: 保存 csv
    public_map('get_is_save_to_csv') = 1;

    % get_is_run_catch_exception: run 捕获异常
    public_map('get_is_run_catch_exception') = 0;

    % get_is_test_catch_exception: test 捕获异常
    public_map('get_is_test_catch_exception') = 0;
%%%


%%%
    % get_public_N: 全局步长
                                            N_t = 8;
                                            % N_t = 4;
                                            % N_t = 3;
    public_map('get_public_N') = -1 + 100 * N_t;

    % get_com_number: 串口 号
                                    c_n_t = 'auto'; % 自动
                                    % c_n_t = 21;   % 手动
    public_map('get_com_number') =  c_n_t;

    % get_judge_stop_times: 判断静止 累计计数
                                                j_s_t_t = 1;
    public_map('get_judge_stop_times') = 50 *   j_s_t_t;

    % get_Kp: Kp
                                    kp_t = 45;
    public_map('get_Kp') = 0.1 *    kp_t;

    % get_Ki: Ki
                                    ki_t = 10;
    public_map('get_Ki') = 0.1 *    ki_t;
%%%
    







    



    output_value = public_map(input_key);
end

