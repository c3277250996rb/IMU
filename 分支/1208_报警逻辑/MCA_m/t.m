addpath(genpath('main'));
addpath(genpath('main'));
addpath(genpath('src'));

while true
    try
        run('main_balance');
    catch ME
        % ���������ʾ������Ϣ
        retryCount = retryCount + 1;
        fprintf('ִ��ʧ��: %s\n', ME.message);
    end
end