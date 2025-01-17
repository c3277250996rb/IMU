addpath(genpath('data'));
addpath(genpath('main'));
addpath(genpath('src'));

format long g; % (ȫ�ֽ��ÿ�ѧ������) ��ȫ�ָ�ʽ����Ϊ��������ʾ

test_name = 'test_RT_rotate';
% test_name = 'test_PID4Euler';
% test_name = 'test_PID4Euler_PRO';
% test_name = 'test_3dimensional';

if get_is_test_catch_exception()

    while true
        try
            run(test_name);
            % run('main_balance');
        catch ME
            % ���������ʾ������Ϣ
            fprintf('ִ��ʧ��: %s\n', ME.message);
        end
    end

else
    run(test_name);
end

