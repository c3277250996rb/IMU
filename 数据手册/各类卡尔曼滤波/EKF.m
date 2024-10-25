clear;clc; close all;
N = 50;
Q = 10;w=sqrt(Q)*randn(1,N);
R = 1;v=sqrt(R)*randn(1,N);
P =eye(1);
x=zeros(1,N);
delta_x=zeros(1,N);
s=zeros(1,N);
Xekf=zeros(1,N);
Xekf1=zeros(1,N);
x(1,1)=0.1;
delta_x(1,1)=0;
s(1,1)=0.1;
Xekf(1,1)=x(1,1);
Xekf1(1,1)=x(1,1);
z=zeros(1,N);
delta_z=zeros(1,N);
z(1)=x(1,1)^2/20+v(1);
delta_z(1)=x(1,1)/10+v(1);
zpre=zeros(1,N);
zpre(1,1)=z(1);

for k = 2 : N
    % ģ��ϵͳ
    s(:,k) = 0.5 * s(:,k-1) + (2.5 * s(:,k-1) / (1 + s(:,k-1).^2)) + 8 * cos(1.2*(k-1));
    x(:,k) = 0.5 * x(:,k-1) + (2.5 * x(:,k-1) / (1 + x(:,k-1).^2)) + 8 * cos(1.2*(k-1)) + w(k-1);
    delta_x(:,k)=(0.5 + 2.5 * (1-x(:,k-1).^2)/((1+x(:,k-1).^2).^2))*delta_x(:,k-1)+w(k-1);
    z(k) = x(:,k).^2 / 20 + v(k);
    delta_z(k)=x(:,k)/10*delta_x(:,k)+v(1);
end

%�����״̬���Ի��Ŀ������˲�
for k=2:N
    %״̬Ԥ��(����)
    Xpre = 0.5*Xekf1(:,k-1)+ 2.5*Xekf1(:,k-1)/(1+Xekf1(:,k-1).^2) + 8 * cos(1.2*(k-1));%f
    
    F = 0.5 + 2.5 * (1-Xekf1.^2)/((1+Xekf1.^2).^2);%f_n
    H = Xpre/10;
    
    %������Ԥ�ⷽ��
    PP=F*P*F'+Q;
    
    %���������棨���䣩
    Kk=PP*H'*(H*PP*H'+R)^(-1);
    
    
    delta_x(:,k)=F*delta_x(:,k-1)+Kk*(delta_z(:,k)-H*delta_x(:,k-1));
    
    %״̬����
    Xekf1(k)=Xpre+delta_x(k);
    
    %���������
    P=PP-Kk*H*PP;
end
% ���忨�����˲�
for k = 2 : N
    %״̬Ԥ��
    Xpre = 0.5*Xekf(:,k-1)+ 2.5*Xekf(:,k-1)/(1+Xekf(:,k-1).^2) + 8 * cos(1.2*(k-1));
    
    F = 0.5 + 2.5 * (1-Xekf.^2)/((1+Xekf.^2).^2);
    H = Xpre/10;
    
    %������Ԥ�ⷽ��
    PP=F*P*F'+Q;
    
    %����������
    Kk=PP*H'*(H*PP*H'+R)^(-1);
    
    %״̬����
    zpre =Xpre.^2/20;
    Xekf(k)=Xpre+Kk*(z(k)-zpre);
    
    %���������
    P=PP-Kk*H*PP;
end

%������
for i=1:N
    Err_Obs(i)=RMS(x(:,i),z(:,i));%�˲�ǰ�����
    Err_EKF(i)=RMS(x(:,i),Xekf(:,i));%�˲�������
    Err_EKF1(i)=RMS(x(:,i),Xekf1(:,i));%�˲�������
end
mean_Obs=mean(Err_Obs);
mean_EKF=mean(Err_EKF);
mean_EKF1=mean(Err_EKF1);
% t = 2 : N;
% figure;
% plot(t,x(1,t),'b',t,Xekf(1,t),'r*');
% legend('��ʵֵ','EKF����ֵ');
figure
hold on;box on;
t=(0:1:N-1);
plot(t,s(1,:),'g','LineWidth',1);%���۹켣
plot(t,x(1,:),'--b','LineWidth',1);%ʵ�ʹ켣
plot(t,z(1,:),'-or','LineWidth',1);%�۲�켣
plot(t,Xekf(1,:),':k','LineWidth',2);%�������˲��켣
plot(t,Xekf1(1,:),'-.m','LineWidth',2);%�������˲��켣
% M=M';
% plot(M(1,:),'k','LineWidth',1);%һ��Ԥ��켣
legend('���۹켣','ʵ���˶��켣','�۲�켣','��չ�������˲���켣','���״̬���Ի�');
xlabel('������ T/s');
ylabel('������ X/m');

figure
hold on;box on;
plot(t,Err_Obs,'-');
plot(t,Err_EKF,'--');
plot(t,Err_EKF1,'-.');
% legend('�˲�ǰ���',num2str(mean_Observation),'�����˲������','�̶������˲������');
legend(sprintf('�˲�ǰ���%.03f',mean_Obs),sprintf('��չ�������˲������%.03f',mean_EKF),sprintf('��������Ի��˲������%.03f',mean_EKF1));
xlabel('�۲�ʱ��/s');
ylabel('���ֵ');


% ����ŷ�Ͼ����Ӻ���
function dist=RMS(X1,X2)
if length(X2)<=2
    dist=sqrt((X1(1)-X2(1))^2);
else
    dist=sqrt((X1(1)-X2(1))^2);
end
end

