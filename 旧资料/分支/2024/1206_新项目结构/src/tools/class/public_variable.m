function output_value = public_variables(input_key)

    % example
        % ����һ�� map
        keySet = {'a', 'b', 'c'}; % ��
        valueSet = [1, 2, 3];     % ֵ
        public_map = containers.Map(keySet, valueSet);

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
    public_map('get_is_use_uart') = 0;

    % get_is_use_rotate_accel: ʹ�� ��ת����
    public_map('get_is_use_rotate_accel') = 0;

    
    % % is_use_uart: ʹ�ô���
    % public_map('is_use_uart') = 1;

    % % is_use_uart: ʹ�ô���
    % public_map('is_use_uart') = 1;

    % % is_use_uart: ʹ�ô���
    % public_map('is_use_uart') = 1;

    % % is_use_uart: ʹ�ô���
    % public_map('is_use_uart') = 1;

    % % is_use_uart: ʹ�ô���
    % public_map('is_use_uart') = 1;

    % % is_use_uart: ʹ�ô���
    % public_map('is_use_uart') = 1;



    output_value = public_map(input_key);
end

