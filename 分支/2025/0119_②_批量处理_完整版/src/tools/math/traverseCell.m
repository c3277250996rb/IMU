function traverseCell(cellArray)
    % 遍历传入的 cell 数组的所有元素
    for i = 1:numel(cellArray)
        element = cellArray{i};
        if iscell(element)
            % 如果元素是子 cell，递归调用函数
            fprintf('Entering nested cell at index %d\n', i);
            traverseCell(element);
        else
            % 如果元素不是 cell，直接处理
            fprintf('Element at index %d: ', i);
            disp(element);
        end
    end
end