function x1 = my_gfun(x0, a1, T)
    % ״̬ת��
    % x0(1,:): a0
    % a1
    % x0(2,:): v0
    x1(1,:) = x0(1,:);
    x1(2,:) = x0(2,:) + (x0(1,:) + a1(:)') ./2 .*T;
    
    % x1(1,:): a0
    % x1(2,:): v1
end
    