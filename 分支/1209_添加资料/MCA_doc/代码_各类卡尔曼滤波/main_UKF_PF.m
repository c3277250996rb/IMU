clear;clc; close all;
%% initialize the variables    
N = 100;  % ������75�� 
N=N-1;
M = 100; % ��������Խ��Ч��Խ�ã�������ҲԽ��  
x = 0.1; % initial actual state     
Q = 1;w=sqrt(Q)*randn(1,N);
R = 1;v=sqrt(R)*randn(1,N);  
  

X=zeros(1,N);
s=zeros(1,N);
Xpf=zeros(1,N);
Xukf=zeros(1,N);
X(1,1)=0.1;
s(1,1)=0.1;
Xpf(1,1)=X(1,1);
Xukf(1,1)=X(1,1);
Z=zeros(1,N);
Z(1)=X(1,1)^2/20+v(1);

for k = 2 : N+1
    % ģ��ϵͳ
    s(:,k) = gfun(s(:,k-1),k-1);
    X(:,k) = gfun(X(:,k-1),k-1)+ sqrt(Q)*randn;
    Z(:,k) =hfun(X(:,k),k)+ sqrt(R)*randn;
end

%�����˲�pf
tic;
Xpf=PF(Xpf,Z,Q,R,M,N);  
toc;
tic;
Xukf=UKF(Xukf,Z,N+1,Q,R);
toc;

for i=1:N+1
    Err_Obs(i)=RMS(X(:,i),Z(:,i));%�˲�ǰ�����
    Err_UKF(i)=RMS(X(:,i),Xukf(:,i));%�˲�������
    Err_PF(i)=RMS(X(:,i),Xpf(:,i));%�˲�������
end
mean_Obs=mean(Err_Obs);
mean_UKF=mean(Err_UKF);
mean_PF=mean(Err_PF);

%%%%%%%%%%%
%��ͼ
figure
hold on ;box on
t=(0:1:N);
plot(t,X(1,:),'-r','LineWidth',1);%ʵ�ʹ켣
plot(t,Xukf(1,:),'-.k','LineWidth',1);%�������˲��켣
plot(t,Xpf(1,:),'-.ob','LineWidth',1);%�۲�켣
% plot(X(1,:),X(3,:),'-k.');
legend('��ʵ�켣','UKF�켣','PF�켣');
xlabel('������ T/s');
ylabel('������ X/m');

figure
hold on;box on;
plot(t,Err_Obs,'-');
plot(t,Err_UKF,'--');
plot(t,Err_PF,'-.');
% legend('�˲�ǰ���',num2str(mean_Observation),'�����˲������','�̶������˲������');
legend(sprintf('�˲�ǰ���%.03f',mean_Obs),sprintf('UKF�˲������%.03f',mean_UKF),sprintf('PF�˲������%.03f',mean_PF));
xlabel('�۲�ʱ��/s');
ylabel('���ֵ');

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
% function res=gfun(Xekf,t)
% res= 0.5*Xekf + 25*Xekf/(1 + Xekf^2) + 8*cos(0.4*(t));
% end

% function res=hfun(X,k)
% res=X^2/20;
% end

