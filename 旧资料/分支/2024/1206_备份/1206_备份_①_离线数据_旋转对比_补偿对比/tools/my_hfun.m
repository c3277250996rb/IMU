function a0 = my_hfun(X, v0, T)
    % �۲�ӳ��
    a1 = X(1, :);
    v1 = X(2, :);
    a0 = (v1 - v0) *2 /T - a1;
end
