function checkForEmptyCSV(filename)
    % ��ȡCSV�ļ�
    data = csv2array();  % ʹ��readmatrix��ȡ��ֵ����
    
    % ���ÿ���Ƿ��п�ֵ��NaN��
    rowsWithEmpty = any(isnan(data), 2);  % ���ÿ���Ƿ���NaN�������߼�������
    
    % �ҵ�������ֵ���к�
    emptyRows = find(rowsWithEmpty);  % �ҵ�ֵΪtrue���к�
    
    % ����п�ֵ�У������Щ�к�
    if ~isempty(emptyRows)
        disp('�����а�����ֵ��');
        disp(emptyRows);
    else
        disp('û�з��ֿ�ֵ');
    end
end
