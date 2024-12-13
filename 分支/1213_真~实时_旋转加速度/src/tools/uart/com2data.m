function output = com2data(s)
    % ������Դ���
    maxRetries = 2;
    retryCount = 0;
    
    dataNumeric = zeros(1,6);

    % ��ȡ����
    while retryCount < maxRetries
        try
            data = (fgetl(s));
            dataArray = strsplit(data, ',');
            dataNumeric(1,:) = str2double(dataArray);  
            break;
        catch ME
            % ���������ʾ������Ϣ
            retryCount = retryCount + 1;
            fprintf('ִ��ʧ��: %s\n', ME.message);
            fprintf('���Ե� %d ��...\n', retryCount);
            % ����Ƿ�ﵽ������Դ���
            if retryCount >= maxRetries
                close_uart();
                fprintf('�ﵽ������Դ�����ִֹͣ�С�\n');
            end
        end
    end
    
    output = str2double(dataArray);
end
