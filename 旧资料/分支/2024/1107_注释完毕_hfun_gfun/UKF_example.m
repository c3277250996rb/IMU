%%%
% UKF��Ŀ������е�Ӧ��
%%%
clear;clc; close all;
N = 100;
Q = 1;w=sqrt(Q)*randn(1,N);
R = 1;v=sqrt(R)*randn(1,N);
P =eye(1);
X=zeros(1,N);
s=zeros(1,N);
Xukf=zeros(1,N);
X(1,1)=0.1;
s(1,1)=0.1;
Xukf(1,1)=X(1,1);
Z=zeros(1,N);
Z(1)=hfun(X(1,1))+v(1);

for k = 2 : N
    % ģ��ϵͳ
    s(:,k) = gfun(s(:,k-1),k-1);
    X(:,k) = gfun(X(:,k-1),k-1)+ w(k-1);
    Z(k) =hfun(X(:,k),k)+ v(k);
end

%UKF�˲���UT�任
L=1;%4������
alpha=1;
kalpha=0;
belta=2;
ramda=3-L;
for j=1:2*L+1
    Wm(j)=1/(2*(L+ramda));
    Wc(j)=1/(2*(L+ramda));
end
Wm(1)=ramda/(L+ramda);%ȨֵWm�ĳ�ֵ��Ҫ���ⶨ
Wc(1)=ramda/(L+ramda)+1-alpha^2+belta;%ȨֵWc�ĳ�ֵ��Ҫ���ⶨ
%%%%%%%%%%%%%%%%
Xukf=zeros(L,N);
Xukf(:,1)=X(:,1);%��X��ֵ�ĳ������ݸ���Xukf
P0=eye(L);%Э�������ʼ��
for t=2:N
    xestimate=Xukf(:,t-1);%��ȡ��һ����UKF��
    P=P0;%Э������
    %��һ�������һ��Sigma�㼯
    cho=(chol(P*(L+ramda)))';
    for k=1:L
        xgamaP1(:,k)=xestimate+cho(:,k);
        xgamaP2(:,k)=xestimate-cho(:,k);
    end
    Xsigma=[xestimate,xgamaP1,xgamaP2];%xestimate����һ���ĵ㣬�൱�ھ�ֵ��
    %�ڶ�������Sigma�㼯����һ��Ԥ��
    Xsigmapre=gfun(Xsigma,t);%F��״̬ת�ƾ���
    %�����������õڶ����Ľ�������ֵ��Э����
    Xpred=zeros(L,1);
    for k=1:2*L+1
        Xpred=Xpred+Wm(k)*Xsigmapre(:,k);%��ֵ
    end
    Ppred=zeros(L,L);
    for k=1:2*L+1
        Ppred=Ppred+Wc(k)*(Xsigmapre(:,k)-Xpred)*(Xsigmapre(:,k)-Xpred)';%Э�������
    end
    Ppred=Ppred+Q;
    %���Ĳ�������Ԥ��ֵ����һ��ʹ��UT�任���õ��µ�sigma�㼯
    chor=(chol((L+ramda)*Ppred))';
    for k=1:L
        XaugsigmaP1(:,k)=Xpred+chor(:,k);
        XaugsigmaP2(:,k)=Xpred-chor(:,k);
    end
    Xaugsigma=[Xpred XaugsigmaP1 XaugsigmaP2];
    %���岽���۲�Ԥ��
    for k=1:2*L+1
        Zsigmapre(1,k)=hfun(Xaugsigma(:,k));
    end
    %������������۲�Ԥ���ֵ��Э����
    Zpred=0;
    for k=1:2*L+1
        Zpred=Zpred+Wm(k)*Zsigmapre(1,k);
    end
    Pzz=0;
    for k=1:2*L+1
        Pzz=Pzz+Wc(k)*(Zsigmapre(1,k)-Zpred)*(Zsigmapre(1,k)-Zpred)';
    end
    Pzz=Pzz+R;
    
    Pxz=zeros(L,1);
    for k=1:2*L+1
        Pxz=Pxz+Wc(k)*(Xaugsigma(:,k)-Xpred)*(Zsigmapre(1,k)-Zpred)';
    end
    %���߲�������kalman����
    K=Pxz*(Pzz)^(-1);
    %�ڰ˲���״̬�ͷ������
    xestimate=Xpred+K*(Z(:,t)-Zpred);
    P=Ppred-K*Pzz*K';
    P0=P;
    Xukf(:,t)=xestimate;
end

%������
for i=1:N
    Err_KalmanFilter(i)=RMS(X(:,i),Xukf(:,i));
end
%%%%%%%%%%%
%��ͼ
figure
hold on ;box on
plot(Z,'o');
plot(X,'-k.');
plot(Xukf,'-r+');
legend('�۲�켣','��ʵ�켣','UKF�켣')
% figure
% hold on ; box on
% plot(Err_KalmanFilter,'-ks','MarkerFace','r')

%%%%%%%%%%%%%
%�Ӻ���
% ����ŷ�Ͼ����Ӻ���
% function dist=RMS(X1,X2)
% if length(X2)<=2
%     dist=sqrt((X1(1)-X2(1))^2);
% else
%     dist=sqrt((X1(1)-X2(1))^2);
% end
% end

% function res=gfun(Xekf,k)
% res=0.5.*Xekf + 25.*Xekf./(1 + Xekf.^2) + 8.*cos(0.5.*(k));
% end

% function res=hfun(X,k)
% res=X^2/20;
% end
%%%%%%%%%%%%%