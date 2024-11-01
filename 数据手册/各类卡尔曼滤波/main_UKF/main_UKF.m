%%%
clear;clc; close all;


% N����ʾģ���ʱ�䲽�������ݵ����������
N = 50;
% Q������������Э�������ģ��ϵͳ�е�����Ŷ���
Q = 1;
% w��������������ʾ��ϵͳ״̬ת�ƹ����м����������������ǴӾ�ֵΪ0������ΪQ����̬�ֲ������ɵ������������ģ����ʵϵͳ�е�����Ŷ���
w = sqrt(Q) * randn(1,N);
% R���۲�������Э�����ʾ���������е������
R = 1;
% v���۲���������ʾ�ڲ��������������������������ǴӾ�ֵΪ0������ΪR����̬�ֲ������ɵ��������ģ���ڲ���ʱ����
v = sqrt(R) * randn(1,N);
% P��Э�������ĳ�ʼֵ����ʾ״̬���ƵĲ�ȷ���ԡ�
P = eye(3);


% s��   ��ʵ״̬�Ĵ洢���飬��ʾϵͳ����ʵ״̬��
s = zeros(1,N);
s(1,1) = 0.1;

% X��   ��ʵ״ֵ̬�Ĵ洢���飬ģ��ϵͳ��ʵ�������
X = zeros(1,N);
X(1,1) = 0.1;

% Z��   �۲�ֵ�Ĵ洢���飬����������Ӱ��Ĺ۲����ݡ�
Z = zeros(1,N);
Z(1) = hfun(X(1,1)) + v(1);

% Xukf��UKF���Ƶ�״̬���洢�˲����״ֵ̬��
Xukf = zeros(1,N);
Xukf(1,1) = X(1,1); 


for k = 2 : N
% ģ��ϵͳ
    % ��ʵ״̬
        % ֱ��ʹ��ģ������
            s(:,k)  = gfun(s(:,k-1), k-1);
    % ��ʵ����
        % ģ������       + ��������
            X(:,k)  = gfun(X(:,k-1), k-1) + w(k-1);
    % �۲�����
        % ��ʵ���� * LSB + �۲�����
            Z(k)    = hfun(X(:,k)  , k  ) + v(k);
end


%UKF�˲���UT�任

% L��״̬������ά�ȣ�����Ϊ1����ʾһά״̬��
L = 1;    %?������

% alpha��kalpha��belta��UKF�ĳ����������ڵ���Sigma������ɺ�Ȩ�ء�
alpha=1;
kalpha=0;
belta=2;

% ramda��UKF����չ������Ӱ��Sigma���ɢ���̶ȡ�
ramda=3-L;

for j=1:2*L+1
    Wm(j)=1/(2*(L+ramda));
    Wc(j)=1/(2*(L+ramda));
end
Wm(1)=ramda/(L+ramda);%ȨֵWm�ĳ�ֵ��Ҫ���ⶨ
Wc(1)=ramda/(L+ramda)+1-alpha^2+belta;%ȨֵWc�ĳ�ֵ��Ҫ���ⶨ
%%%%%%%%%%%%%%%%
Xukf = zeros(L,N);
Xukf(:,1) = X(:, 1);
P0=eye(L);%Э�������ʼ��
for t=2:N

% 1 ���2n+1��Sigma�㼰��Ȩֵ:

    % 1.1
        % ��ȡ��һ��ʱ�䲽��UKF״̬����ֵ����ֵ���������丳ֵ��xestimate��
            xestimate = Xukf(:, t-1); % ��ȡ��һ����UKF��

    % 1.2
        % P��������״̬����֮��ķ����Э�����ӳ�˵�ǰ���ƵĲ�ȷ���̶ȡ�    
            P = P0; % Э������

    % 1.3
        % ͨ��Cholesky�ֽ�����ȡЭ�������P*(L+ramda)�������Ǿ���cho�����õ�һ���ʺ�������Sigma��ľ�������ֽ������Э������󰴡�ƽ��������⣬�Ա�Ϊÿ��Sigma��ʩ��һ��ƫ������
            cho=(chol(P*(L+ramda)))';

    % 1.4
        % ѭ������һ��λ�ھ�ֵxestimate��Χ��Sigma��
            for k=1:L
                % Sigma�㼯���е���ƫ�Ƶ㡣cho(:,k)��ʾ��xestimate����һ������ƫ�ơ������ƫ�ƴ�С��Cholesky�ֽ�ľ���ȷ����
                    xgamaP1(:,k)=xestimate+cho(:,k);
                % Sigma�㼯���еĸ�ƫ�Ƶ㡣��xestimateʩ�Ӹ�ƫ�ƣ���������һ��ԳƵ�Sigma�㡣
                    xgamaP2(:,k)=xestimate-cho(:,k);
            end
    
    % 1.5   
        % ����ֵ��xestimate�Լ�����������ƫ�Ƶ�xgamaP1��xgamaP2��ϳ����յ�Sigma�㼯��Xsigma���ü���������һ����״̬�����͸��¡�
        % ��ЩSigma�㲶׽�˾�ֵ��Э������Ϣ��ͨ�������Ժ������д����󣬽������µ�״̬�ֲ���
            Xsigma = [xestimate, xgamaP1, xgamaP2]; % xestimate����һ���ĵ㣬�൱�ھ�ֵ��
    

% 2 ��Sigma��ͨ��״̬ת�ƺ���f(x,t)Ԥ��õ�:
    Xsigmapre=gfun(Xsigma,t);
    

% 3 ����Ԥ���ֵ��Э����

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
    
% 4 ʹ��UT�任�����µ�Sigma�㼯
    %��5������Ԥ��ֵ����һ��ʹ��UT�任���õ��µ�sigma�㼯
        chor=(chol((L+ramda)*Ppred))';
        for k=1:L
            XaugsigmaP1(:,k)=Xpred+chor(:,k);
            XaugsigmaP2(:,k)=Xpred-chor(:,k);
        end
        Xaugsigma=[Xpred XaugsigmaP1 XaugsigmaP2];
    
% 5 ��Sigma�����۲⺯��h(x,t)����Ԥ��۲�ֵ
    %��6���۲�Ԥ��
        for k=1:2*L+1
            Zsigmapre(1,k)=hfun(Xaugsigma(:,k),k);
        end
    

% 6 ����Ԥ��۲��ֵ��Э����

    % 6.1 ����۲�Ԥ���Ȩ��ֵ
        Zpred=0;
        for k=1:2*L+1
            Zpred=Zpred+Wm(k)*Zsigmapre(:,k);
        end
    % 6.2 ����۲��Ȩ����
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
        

% 7 ���㿨��������
    %��10������kalman����
        K=Pxz*Pzz^-1;
        

% 8 ����״̬��Э����
    %��11��״̬����
        Xukf(:,t)=Xpred+K*(Z(t)-Zpred);
        
    %��12���������
        P0=Ppred-K*Pzz*K';

end





for i=1:N
    Err_Obs(i)=RMS(X(:,i),Z(:,i));%�˲�ǰ�����
    Err_UKF(i)=RMS(X(:,i),Xukf(:,i));%�˲�������
end
mean_Obs=mean(Err_Obs);
mean_UKF=mean(Err_UKF);

%%%%%%%%%%%
%��ͼ
figure
hold on ;box on
t=(0:1:N-1);
plot(t,X(1,:),'--b','LineWidth',1);%ʵ�ʹ켣
% plot(t,Z(1,:),'-or','LineWidth',1);%�۲�켣
plot(t,Xukf(1,:),':k','LineWidth',2);%�������˲��켣
% plot(X(1,:),X(3,:),'-k.');
% plot(Xukf(1,:),Xukf(3,:),'-r+');
legend('��ʵ�켣','UKF�켣');
xlabel('������ T/s');
ylabel('������ X/m');

figure
hold on;box on;
plot(t,Err_Obs,'-');
plot(t,Err_UKF,'--');
% plot(t,Err_EKF1,'-.');
% legend('�˲�ǰ���',num2str(mean_Observation),'�����˲������','�̶������˲������');
legend(sprintf('�˲�ǰ���%.03f',mean_Obs),sprintf('UKF�˲������%.03f',mean_UKF));
xlabel('�۲�ʱ��/s');
ylabel('���ֵ');

%%%%%%%%%%%%%
%�Ӻ���
% ����ŷ�Ͼ����Ӻ���
function dist=RMS(X1,X2)
if length(X2)<=2
    dist=sqrt((X1(1)-X2(1))^2);
else
    dist=sqrt((X1(1)-X2(1))^2);
end
end

function res=gfun(Xekf,k)
    res=0.5.*Xekf + 25.*Xekf./(1 + Xekf.^2) + 8.*cos(0.3.*(k));
end

function res=hfun(X,k)
res=X^2/20;
end
%%%%%%%%%%%%%