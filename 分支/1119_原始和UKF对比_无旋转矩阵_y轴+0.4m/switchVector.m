function result = switchVector(vec)
    % switchVector - 将行向量转换为列向量，或将列向量转换为行向量
    %
    % 输入参数:
    % vec - 输入向量 (行向量或列向量)
    %
    % 输出参数:
    % result - 转换后的向量 (行向量或列向量)

    % 检查输入是否为向量
    if ~isvector(vec)
        error('输入必须是向量');
    end

    % 转置向量
    result = vec(:).'; % 如果是列向量变为行向量，否则变为列向量
end
