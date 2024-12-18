function Xukf=UKF(Xukf,Z,N,Q,R)

%UKF�˲���UT�任
L=1;        %L������
alpha=1;    %0~1
kalpha=0;
belta=2;    %����Ϊ2
ramda=3-L;
for j=1:2*L+1
    Wm(j)=1/(2*(L+ramda));
    Wc(j)=1/(2*(L+ramda));
end
Wm(1)=ramda/(L+ramda);%ȨֵWm�ĳ�ֵ��Ҫ���ⶨ
Wc(1)=ramda/(L+ramda)+1-alpha^2+belta;%ȨֵWc�ĳ�ֵ��Ҫ���ⶨ
%%%%%%%%%%%%%%%%

P0=eye(L);%Э�������ʼ��
for t=2:N
    xestimate=Xukf(:,t-1);%��ȡ��һ����UKF��
    P=P0;%Э������
    
    %(1) ���һ��Sigma�㼯
    cho=(chol(P*(L+ramda)))';
    for k=1:L
        xgamaP1(:,k)=xestimate+cho(:,k);
        xgamaP2(:,k)=xestimate-cho(:,k);
    end
    Xsigma=[xestimate,xgamaP1,xgamaP2];%xestimate����һ���ĵ㣬�൱�ھ�ֵ��
    
    %(2) ��Sigma�㼯����һ��Ԥ��
    Xsigmapre=gfun(Xsigma,t);
    
    %��3�������Ȩ��ֵ
    Xpred=zeros(L,1);
    for k=1:2*L+1
        Xpred=Xpred+Wm(k)*Xsigmapre(:,k);%��ֵ
    end
    %��4�������Ȩ����
    Ppred=zeros(L,L);
    for k=1:2*L+1
        Ppred=Ppred+Wc(k)*(Xsigmapre(:,k)-Xpred)*(Xsigmapre(:,k)-Xpred)';%Э�������
    end
    Ppred=Ppred+Q;
    
    %��5������Ԥ��ֵ����һ��ʹ��UT�任���õ��µ�sigma�㼯
    chor=(chol((L+ramda)*Ppred))';
    for k=1:L
        XaugsigmaP1(:,k)=Xpred+chor(:,k);
        XaugsigmaP2(:,k)=Xpred-chor(:,k);
    end
    Xaugsigma=[Xpred XaugsigmaP1 XaugsigmaP2];
    
    %��6���۲�Ԥ��
    for k=1:2*L+1
        Zsigmapre(1,k)=hfun(Xaugsigma(:,k),k);
    end
    
    %��7������۲�Ԥ���Ȩ��ֵ
    Zpred=0;
    for k=1:2*L+1
        Zpred=Zpred+Wm(k)*Zsigmapre(:,k);
    end
    %��8������۲��Ȩ����
    Pzz=0;
    for k=1:2*L+1
        Pzz=Pzz+Wc(k)*(Zsigmapre(:,k)-Zpred)*(Zsigmapre(:,k)-Zpred)';
    end
    Pzz=Pzz+R;
    
    %��9������Ԥ��Э����
    Pxz=zeros(L,1);
    for k=1:2*L+1
        Pxz=Pxz+Wc(k)*(Xaugsigma(:,k)-Xpred)*(Zsigmapre(:,k)-Zpred)';
    end
    
    %��10������kalman����
    K=Pxz*Pzz^-1;
    
    %��11��״̬����
    Xukf(:,t)=Xpred+K*(Z(t)-Zpred);
    
    %��12���������
    P0=Ppred-K*Pzz*K';
end
end

function res=gfun(Xekf,k)
res=0.5.*Xekf + 25.*Xekf./(1 + Xekf.^2) + 8.*cos(0.4.*(k));
end

function res=hfun(X,k)
res=X^2/20;
end