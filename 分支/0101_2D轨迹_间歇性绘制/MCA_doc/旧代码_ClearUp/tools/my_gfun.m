function x1 = my_gfun(x0, a1, T)
    % ״̬ת��
    x1(1,:) = x0(1,:);
    x1(2,:) = x0(2,:) + (x0(1,:) + a1(:)') ./2 .*T;
    
end
    