function output_value = public_variables(input_key)

    % example
        % ����һ�� map
        % keySet = {'a', 'b', 'c'}; % ��
        % valueSet = [1, 2, 3];     % ֵ
        % public_map = containers.Map(keySet, valueSet);
        public_map = containers.Map();

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

%%%
    % get_is_use_uart: ʹ�� ����
    public_map('get_is_use_uart') = 1;

    % get_is_use_rotate_accel: ʹ�� ��ת����
    public_map('get_is_use_rotate_accel') = 0;

    % get_is_save_to_csv: ���� csv
    public_map('get_is_save_to_csv') = 1;

    % get_is_run_catch_exception: run �����쳣
    public_map('get_is_run_catch_exception') = 0;

    % get_is_test_catch_exception: test �����쳣
    public_map('get_is_test_catch_exception') = 0;
%%%


%%%
    % get_public_N: ȫ�ֲ���
                                            N_t = 8;
                                            % N_t = 4;
                                            % N_t = 3;
    public_map('get_public_N') = -1 + 100 * N_t;

    % get_com_number: ���� ��
                                    c_n_t = 'auto'; % �Զ�
                                    % c_n_t = 21;   % �ֶ�
    public_map('get_com_number') =  c_n_t;

    % get_judge_stop_times: �жϾ�ֹ �ۼƼ���
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

