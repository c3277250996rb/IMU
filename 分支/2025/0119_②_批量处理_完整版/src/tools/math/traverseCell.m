function traverseCell(cellArray)
    % ��������� cell ���������Ԫ��
    for i = 1:numel(cellArray)
        element = cellArray{i};
        if iscell(element)
            % ���Ԫ������ cell���ݹ���ú���
            fprintf('Entering nested cell at index %d\n', i);
            traverseCell(element);
        else
            % ���Ԫ�ز��� cell��ֱ�Ӵ���
            fprintf('Element at index %d: ', i);
            disp(element);
        end
    end
end