function Xpf=PF(Xpf,Z,Q,R,M,N)
%�����˲���ʼ��
V = 2; %��ʼ�ֲ��ķ���
x_P = []; % ����
% ��һ����˹�ֲ�����Ĳ�����ʼ������
for i = 1:M
    x_P(i) = Xpf(:,1) + sqrt(V) * randn;
end

%�����˲�
for t = 1:N
    for i = 1:M
        %������p(x(k)|x(k-1))�в���
        x_P_update(:,i) = gfun(x_P(:,i),t) + sqrt(Q)*randn;
        %����������ӵ�ֵ��Ϊ���������Ȼȥ����Ȩ�����̵�
        z_update(:,i) = x_P_update(:,i)^2/20;
        %��ÿ�����Ӽ�����Ȩ�أ�����������������Ǹ�˹�ֲ������� w = p(y|x)��Ӧ����ļ��㹫ʽ
        P_w(:,i) = (1/sqrt(2*pi*R)) * exp(-(Z(:,t+1) - z_update(:,i))^2/(2*R));
    end
    % ��һ��.
    P_w = P_w./sum(P_w);
    
    %% Resampling����û���ò�����֮ǰ˵��histc����������Ŀ�ĺ�Ч����һ����
    for i = 1 : M
        x_P(:,i) = x_P_update(find(rand <= cumsum(P_w),1));   % ����Ȩ�ش�Ľ���õ����
    end                                                     % find( ,1) ���ص�һ�� ����ǰ������������ �±�
    
    %״̬���ƣ��ز����Ժ�ÿ�����ӵ�Ȩ�ض������1/N
    Xpf(:,t+1)=mean(x_P);
    
    
end
end

function res=gfun(Xekf,t)
res= 0.5.*Xekf + 25.*Xekf./(1 + Xekf.^2) + 8.*cos(0.4.*(t));
end

function res=hfun(X,k)
res=X^2/20;
end