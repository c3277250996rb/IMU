function output = com2data(s)
    % 最大重试次数
    maxRetries = 2;
    retryCount = 0;
    
    dataNumeric = zeros(1,6);

    % 读取数据
    while retryCount < maxRetries
        try
            data = (fgetl(s));
            dataArray = strsplit(data, ',');
            dataNumeric(1,:) = str2double(dataArray);  
            break;
        catch ME
            % 捕获错误并显示错误信息
            retryCount = retryCount + 1;
            fprintf('执行失败: %s\n', ME.message);
            fprintf('重试第 %d 次...\n', retryCount);
            % 检查是否达到最大重试次数
            if retryCount >= maxRetries
                close_uart();
                fprintf('达到最大重试次数，停止执行。\n');
            end
        end
    end
    
    output = str2double(dataArray);
end
