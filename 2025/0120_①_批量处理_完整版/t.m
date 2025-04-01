addpath(genpath('data'));
addpath(genpath('main'));
addpath(genpath('src'));

format long g; % (全局禁用科学计数法) 将全局格式设置为长数字显示

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
            % 捕获错误并显示错误信息
            fprintf('执行失败: %s\n', ME.message);
        end
    end

else
    run(test_name);
end

