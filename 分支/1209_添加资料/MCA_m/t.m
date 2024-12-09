addpath(genpath('main'));
addpath(genpath('main'));
addpath(genpath('src'));

while true
    try
        run('main_balance');
    catch ME
        % 捕获错误并显示错误信息
        retryCount = retryCount + 1;
        fprintf('执行失败: %s\n', ME.message);
    end
end