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

    % get_public_N: ȫ�ֲ���
                                            N_t = 8;
    public_map('get_public_N') = -1 + 100 * N_t;

    % get_is_use_uart: ʹ�� ����
    public_map('get_is_use_uart') = 1;

    % get_com_number: ���� ��
                                    c_n_t = 'auto'; % �Զ�
                                    % c_n_t = 21;   % �ֶ�
    public_map('get_com_number') =  c_n_t;

    % get_is_use_rotate_accel: ʹ�� ��ת����
    public_map('get_is_use_rotate_accel') = 0;

    % get_judge_stop_times: �жϾ�ֹ �ۼƼ���
                                                j_s_t_t = 1;
    public_map('get_judge_stop_times') = 50 *   j_s_t_t;

    







    



    output_value = public_map(input_key);
end

