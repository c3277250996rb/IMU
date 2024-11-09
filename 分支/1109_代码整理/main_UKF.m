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


% s����ʵ״̬�Ĵ洢���飬��ʾϵͳ����ʵ״̬��
s = zeros(1,N);
s(1,1) = 0.1;

% X��Ԥ��״ֵ̬�Ĵ洢���飬ģ��ϵͳ��ʵ�������
        % ���壺Ԥ��״̬ X ��ͨ��״̬ת�Ʒ��̣��� gfun��Ԥ�����״̬���ơ��� UKF �У����״̬ͨ����Ȩ��ƽ����������ʵ״̬��
        % ����ļ��ٶȼ���Ŀ�У�������ʹ����һʱ�̵�״̬ȥԤ�⵱ǰ��λ�ơ��ٶȺͼ��ٶ�ʱ���õ��ľ���Ԥ��״̬ X�����磬���֪����һʱ�̵��ٶ� v �ͼ��ٶ� a������ͨ������ʽԤ�����һʱ�̵�λ�ƺ��ٶȡ�
X = zeros(1,N);
X(1,1) = 0.1;

% Z���۲�ֵ�Ĵ洢���飬����������Ӱ��Ĺ۲����ݡ�
        % ���壺�۲�ֵ Z �Ǵ������ṩ��ֱ�ӹ۲����ݡ��� UKF �У��۲�ֵ��ͨ�� hfun ӳ��õ��ģ�������״̬�ռ�ӳ�䵽�������Ĳ����ռ䡣
        % ����ļ��ٶȼ���Ŀ�У����ٶȼƲ������ļ��ٶ����ݾ��ǹ۲�ֵ Z����ͨ��ֻ�м��ٶ�һ���Ϊ���ٶȼ�ֻ��ֱ�Ӳ������ٶȡ�
Z = zeros(1,N);
Z(1) = hfun(X(1,1)) + v(1);

% Xukf��UKF���Ƶ�״̬���洢�˲����״ֵ̬��
        % ���壺����״̬ Xukf �� UKF ͨ���ں�Ԥ��״̬�͹۲�ֵ������Ź��ơ������˲�������ó��ġ���ѡ�״̬�����ڽӽ���ʵ״̬��
        % ����ļ��ٶȼ���Ŀ�У�ͨ�� UKF ���º��״̬���ƽ�������������Ƶ�λ�� xukf ���ٶ� vukf �ͼ��ٶ� aukf ����������ʾ���չ��Ƴ���λ�ơ��ٶȺͼ��ٶȡ���Щ���ƽ���ۺ���Ԥ����Ϣ�͹۲���Ϣ���Ƕ���ʵ״̬����ѽ��ơ�
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
    

% 2 ��Sigma��ͨ��״̬ת�ƺ��� gfun(x,t) Ԥ��õ�:
    Xsigmapre=gfun(Xsigma,t);
    

% 3 ����Ԥ���ֵ��Э����

    % 3.1 �����Ȩ��ֵ
        % Xpred ��ʼ��Ϊһ��ȫ�������������СΪ L��1������ L ��״̬��ά�ȡ�
            Xpred=zeros(L,1);

            for k=1:2*L+1

        % ��ÿ�ε����У�����Ȩ�غ��ۼӵ� Xpred �С����Xpred �������� sigma ��ļ�Ȩƽ��ֵ����Ԥ���ֵ��
                Xpred = Xpred +                         ...
...     % Wm(k) �Ƕ�Ӧ�ڵ� k �� sigma ���Ȩ�أ����ڼ����ֵ��
                        Wm(k)                           ...
...     % Xsigmapre(:,k) �ǵ� k �� sigma ���״̬������
                      * Xsigmapre(:,k);

            end

    % 3.2 �����Ȩ����
        % Ppred ��ʼ��Ϊ L��L ��ȫ��������ڴ洢Э�������
            Ppred = zeros(L,L);

            for k = 1:2*L+1

                Ppred = Ppred +                         ...
...     % Wc(k) �ǵ� k �� sigma ���Э����Ȩ�ء�
                        Wc(k)                           ...
...     % Xsigmapre(:,k) - Xpred ������ǵ� k �� sigma ����Ԥ���ֵ֮��Ĳ
                      * (Xsigmapre(:,k) - Xpred)        ...
...     % (Xsigmapre(:,k)-Xpred)' ��������ת�ã��γ�һ�� L��1 ����������һ�� 1��L ���������ĳ˻����õ�һ�� L��L �ľ���
                      * (Xsigmapre(:,k) - Xpred)';

            end

    % 3.3 ����������Э������� Q ���뵽Ԥ��Э�����У��Կ���ϵͳ�Ĳ�ȷ���ԡ�
        Ppred = Ppred + Q;
    
    
% 4 ʹ��UT�任�����µ�Sigma�㼯
    %��5������Ԥ��ֵ����һ��ʹ��UT�任���õ��µ�sigma�㼯

    % 4.1 ���� Cholesky �ֽ⣺chor=(chol((L+ramda)*Ppred))';

        % ���޼��任��UT�������µ� Sigma ��Ĺ����У�Cholesky �ֽ���Ҫ����ȷ�� Sigma ��ķֲ���Χ��������˵�������������£�
            % 1.�����Ŷ�����  ��ͨ�� Cholesky �ֽ�õ��ľ��� chor�����������Ŷ���������Щ�Ŷ������ھ�ֵ Xpred ����Χ�ֲ���ȷ���� Sigma ���ܹ�����״̬�ֲ���Э����ṹ��
            % 2.����Э����ṹ��chol((L+ramda)*Ppred) �ķֽ����ṩ��һ�����Ա任��ʹ�������� Xpred ��Χ�ֲ��� Sigma ���ܹ���ӳԤ��Э���� Ppred ����������Щ�ֲ��������Ԥ��ֲ���ͳ�����ԣ������ֵ��Э�������
            % 3.����������Ŷ���ͨ�� Cholesky �ֽ��ľ��� chor�����ǿ����� Xpred ������ȥ��Щ�Ŷ������γ� Sigma ��ĶԳƷֲ���ȷ����ƫ�ԡ����ֶԳƷֲ��� UT �ĺ��ģ���֤�˶�ϵͳ�����Ա任��ľ�ֵ��Э�����׼ȷ�ƽ���
        % ���ϣ�Cholesky �ֽ��������������ȷ�����ɵ� Sigma ���ܹ�׼ȷ����״̬��ͳ�����ԣ�������Э����ṹ�������ھ�ֵ Xpred �Ļ����ϼ�����Ӧ������͸����Ŷ����Թ�����ƫ�ķֲ���

            % ������������
                scale_factor = (L + ramda) * Ppred;
            % ���� Cholesky �ֽ⣬�õ������Ǿ���
                lower_triangular = chol(scale_factor);
            % ת�������Ǿ���
                chor = lower_triangular';

    % 4.2 ���� sigma �㣺
        for k=1:L

        % Ϊ�� k �� sigma ����������Ŷ�������Ԥ���ֵ Xpred �Ļ����ϼ��� Cholesky ����ĵ� k �С�
            XaugsigmaP1(:,k)=Xpred+chor(:,k);
        % Ϊ�� k �� sigma ����Ӹ����Ŷ�������Ԥ���ֵ Xpred �Ļ����ϼ�ȥ Cholesky ����ĵ� k �С�
            XaugsigmaP2(:,k)=Xpred-chor(:,k);

        end

    % 4.3 ��� sigma �㣺
        % ��Ԥ���ֵ Xpred �����ɵ�����͸��� sigma ����ϳ�һ���µ� sigma �㼯 Xaugsigma������µ� sigma �㼯������һ�����ĵ�� 2L ���Ŷ��㣬�ܹ� 2L+1 �� sigma �㡣
            Xaugsigma=[Xpred XaugsigmaP1 XaugsigmaP2];
    
% 5 ��Sigma�����۲⺯�� hfun(x,t) ����Ԥ��۲�ֵ
    %��6���۲�Ԥ��

        for k=1:2*L+1
    % Zsigmapre(1,k) ������õ��Ĺ۲�ֵ�洢�� Zsigmapre �ĵ�һ�к͵� k ���С�������Zsigmapre �ͻ�洢���� sigma ���Ӧ�Ĺ۲�Ԥ��ֵ��
            Zsigmapre(1,k)=hfun(Xaugsigma(:,k),k);
        end
    

% 6 ����Ԥ��۲��ֵ��Э����

    % 6.1 ����۲�Ԥ���Ȩ��ֵ
        Zpred=0;
        for k=1:2*L+1
        % ��ÿ�� sigma �㣬ͨ��Ȩ�� Wm(k) ��Ȩ�۲�Ԥ��ֵ Zsigmapre(:,k)�����ۼӵ� Zpred��
        % ��һ���Ľ���� Zpred ��������� sigma ��۲�ֵ�ļ�Ȩ��ֵ����Ԥ��Ĺ۲�ֵ��
            Zpred = Zpred + Wm(k) * Zsigmapre(:,k);
        end
    % 6.2 ����۲��Ȩ����
        Pzz=0;
        for k=1:2*L+1

            % Pzz = Pzz + Wc(k) * (Zsigmapre(:,k) - Zpred) * (Zsigmapre(:,k) - Zpred)';
            % ����۲�ֵ���ֵ�Ĳ�
                diff = Zsigmapre(:,k) - Zpred;                
            % �������
                outer_product = diff * diff';           
            % ��Ȩ���
                weighted_contribution = Wc(k) * outer_product; 
            % �ۼӵ��������
                Pzz = Pzz + weighted_contribution;            

        end
        % ���Ϲ۲�����Э������� R���õ����յĹ۲�Ԥ�ⷽ�
        Pzz=Pzz+R;
    
    % 6.3 ����״̬��۲�֮���Э����
        %��9������Ԥ��Э����
        % Pxz = Pxz + Wc(k) * (Xaugsigma(:,k) - Xpred) * (Zsigmapre(:,k) - Zpred)';
            
        Pxz = zeros(L, 1);
        for k = 1:2*L + 1  
        % ����״̬��
            state_diff = Xaugsigma(:, k) - Xpred;      
        % ����۲��
            obs_diff = Zsigmapre(:, k) - Zpred;         
        % �������
            outer_product = state_diff * obs_diff';     
        % ��Ȩ���
            weighted_contribution = Wc(k) * outer_product; 
        % �ۼӵ�Ԥ��Э����
            Pxz = Pxz + weighted_contribution;          
        end

        
% 7 ���㿨��������
    %��10������kalman����
    % Pxz ��״̬��۲�֮���Э���
    % Pzz^-1 �ǹ۲�Ԥ�ⷽ�������󣬱�ʾ�۲�Ĳ�ȷ���ԡ�
    % ���������� K ͨ����״̬��۲��Э������Թ۲ⷽ�����õ��������������ȷ���ڸ���״̬����ʱ���۲����ݵ�Ӱ��̶ȡ�
        K = Pxz * Pzz^-1;
        

% 8 ����״̬��Э����
    % 8.1 ״̬����

        % Z(t) ����ʱ�� t ʱ�̵�ʵ�ʹ۲�ֵ��
        % Zpred �Ǹ��ݵ�ǰ״̬Ԥ��Ĺ۲�ֵ��
        % (Z(t) - Zpred) �����˹۲�в��ʵ�ʹ۲���Ԥ��۲�֮��Ĳ
        % ������в���Կ��������� K��Ȼ��ӵ�Ԥ��״̬ Xpred �ϣ��õ����º��״̬���� Xukf(:, t)��
            Xukf(:, t) = Xpred + K * (Z(t) - Zpred);

    % 8.2 �������

        % Ppred ��Ԥ���״̬Э�������
        % K * Pzz * K' �����˿�����������۲ⷽ��֮��ĳ˻�����һ���ʾ���º�״̬�Ĳ�ȷ���ԡ�
        % ���ս������Ԥ��Э�����м�ȥ���õ����º��Э������� P0����ӳ���ڹ۲���º�״̬�Ĳ�ȷ���Լ��١�
            P0 = Ppred - K * Pzz * K';

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
plot(t,Z(1,:),'-or','LineWidth',1);%�۲�켣
plot(t,Xukf(1,:),':k','LineWidth',2);%�������˲��켣
% plot(X(1,:),X(3,:),'-k.');
% plot(Xukf(1,:),Xukf(3,:),'-r+');
legend('��ʵ�켣','�۲�켣','UKF�켣');
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
% function dist=RMS(X1,X2)
% if length(X2)<=2
%     dist=sqrt((X1(1)-X2(1))^2);
% else
%     dist=sqrt((X1(1)-X2(1))^2);
% end
% end



% % ״̬ת�ƺ���, ��֪��ǰλ�ƺ��ٶȣ�ͨ��gfun����ϼ��ٶȺ�ʱ�䲽��������Ԥ����һʱ�̵�λ�ƺ��ٶȡ�
% function res=gfun(Xekf,k)
%     res=0.5.*Xekf + 25.*Xekf./(1 + Xekf.^2) + 8.*cos(0.3.*(k));
% end



% % hfun ��״̬���� x �С���ȡ�������ٶ������������Ϊ���Ǽ��ٶȼƹ۲⵽�����ݡ�
% function res=hfun(X,k)
% res=X^2/20;
% end
%%%%%%%%%%%%%