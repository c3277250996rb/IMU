addpath(genpath('data'));
addpath(genpath('main'));
addpath(genpath('src'));

run_name = 'main_balance';

if get_is_run_catch_exception

    while true
        try
            run(run_name);
        catch ME
            % 捕获错误并显示错误信息
            fprintf('执行失败: %s\n', ME.message);
        end
    end

else
    run(run_name);
end