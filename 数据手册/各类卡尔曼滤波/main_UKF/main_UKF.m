%%%
clear;clc; close all;


% N：表示模拟的时间步数（数据点的数量）。
N = 50;
% Q：过程噪声的协方差，用于模拟系统中的随机扰动。
Q = 1;
% w：过程噪声，表示在系统状态转移过程中加入的随机噪声。它是从均值为0、方差为Q的正态分布中生成的随机数，用于模拟真实系统中的随机扰动。
w = sqrt(Q) * randn(1,N);
% R：观测噪声的协方差，表示测量过程中的随机误差。
R = 1;
% v：观测噪声，表示在测量过程中引入的随机噪声。它是从均值为0、方差为R的正态分布中生成的随机数，模拟在测量时的误差。
v = sqrt(R) * randn(1,N);
% P：协方差矩阵的初始值，表示状态估计的不确定性。
P = eye(3);


% s：   真实状态的存储数组，表示系统的真实状态。
s = zeros(1,N);
s(1,1) = 0.1;

% X：   真实状态值的存储数组，模拟系统的实际输出。
X = zeros(1,N);
X(1,1) = 0.1;

% Z：   观测值的存储数组，包含受噪声影响的观测数据。
Z = zeros(1,N);
Z(1) = hfun(X(1,1)) + v(1);

% Xukf：UKF估计的状态，存储滤波后的状态值。
Xukf = zeros(1,N);
Xukf(1,1) = X(1,1); 


for k = 2 : N
% 模拟系统
    % 真实状态
        % 直接使用模拟数据
            s(:,k)  = gfun(s(:,k-1), k-1);
    % 真实数据
        % 模拟数据       + 过程噪声
            X(:,k)  = gfun(X(:,k-1), k-1) + w(k-1);
    % 观测数据
        % 真实数据 * LSB + 观测噪声
            Z(k)    = hfun(X(:,k)  , k  ) + v(k);
end


%UKF滤波，UT变换

% L：状态变量的维度，这里为1，表示一维状态。
L = 1;    %?个变量

% alpha、kalpha、belta：UKF的超参数，用于调整Sigma点的生成和权重。
alpha=1;
kalpha=0;
belta=2;

% ramda：UKF的扩展参数，影响Sigma点的散布程度。
ramda=3-L;

for j=1:2*L+1
    Wm(j)=1/(2*(L+ramda));
    Wc(j)=1/(2*(L+ramda));
end
Wm(1)=ramda/(L+ramda);%权值Wm的初值需要另外定
Wc(1)=ramda/(L+ramda)+1-alpha^2+belta;%权值Wc的初值需要另外定
%%%%%%%%%%%%%%%%
Xukf = zeros(L,N);
Xukf(:,1) = X(:, 1);
P0=eye(L);%协方差阵初始化
for t=2:N

% 1 获得2n+1个Sigma点及其权值:

    % 1.1
        % 获取上一个时间步的UKF状态估计值（均值），并将其赋值给xestimate。
            xestimate = Xukf(:, t-1); % 获取上一步的UKF点

    % 1.2
        % P用于描述状态变量之间的方差和协方差，反映了当前估计的不确定程度。    
            P = P0; % 协方差阵

    % 1.3
        % 通过Cholesky分解来获取协方差矩阵P*(L+ramda)的下三角矩阵（cho），得到一个适合于生成Sigma点的矩阵。这个分解操作将协方差矩阵按“平方根”拆解，以便为每个Sigma点施加一个偏移量。
            cho=(chol(P*(L+ramda)))';

    % 1.4
        % 循环生成一组位于均值xestimate周围的Sigma点
            for k=1:L
                % Sigma点集合中的正偏移点。cho(:,k)表示对xestimate加入一个“正偏移”，这个偏移大小由Cholesky分解的矩阵确定。
                    xgamaP1(:,k)=xestimate+cho(:,k);
                % Sigma点集合中的负偏移点。对xestimate施加负偏移，以生成另一组对称的Sigma点。
                    xgamaP2(:,k)=xestimate-cho(:,k);
            end
    
    % 1.5   
        % 将均值点xestimate以及其两组正负偏移点xgamaP1和xgamaP2组合成最终的Sigma点集合Xsigma，该集合用于下一步的状态传播和更新。
        % 这些Sigma点捕捉了均值和协方差信息，通过非线性函数进行传播后，将近似新的状态分布。
            Xsigma = [xestimate, xgamaP1, xgamaP2]; % xestimate是上一步的点，相当于均值点
    

% 2 将Sigma点通过状态转移函数f(x,t)预测得到:
    Xsigmapre=gfun(Xsigma,t);
    

% 3 计算预测均值和协方差

    %（3）计算加权均值
        Xpred=zeros(L,1);
        for k=1:2*L+1
            Xpred=Xpred+Wm(k)*Xsigmapre(:,k);%均值
        end
    %（4）计算加权方差
        Ppred=zeros(L,L);
        for k=1:2*L+1
            Ppred=Ppred+Wc(k)*(Xsigmapre(:,k)-Xpred)*(Xsigmapre(:,k)-Xpred)';%协方差矩阵
        end
        Ppred=Ppred+Q;
    
% 4 使用UT变换生成新的Sigma点集
    %（5）根据预测值，再一次使用UT变换，得到新的sigma点集
        chor=(chol((L+ramda)*Ppred))';
        for k=1:L
            XaugsigmaP1(:,k)=Xpred+chor(:,k);
            XaugsigmaP2(:,k)=Xpred-chor(:,k);
        end
        Xaugsigma=[Xpred XaugsigmaP1 XaugsigmaP2];
    
% 5 将Sigma点代入观测函数h(x,t)计算预测观测值
    %（6）观测预测
        for k=1:2*L+1
            Zsigmapre(1,k)=hfun(Xaugsigma(:,k),k);
        end
    

% 6 计算预测观测均值和协方差

    % 6.1 计算观测预测加权均值
        Zpred=0;
        for k=1:2*L+1
            Zpred=Zpred+Wm(k)*Zsigmapre(:,k);
        end
    % 6.2 计算观测加权方差
        Pzz=0;
        for k=1:2*L+1
            Pzz=Pzz+Wc(k)*(Zsigmapre(:,k)-Zpred)*(Zsigmapre(:,k)-Zpred)';
        end
        Pzz=Pzz+R;
    
    %（9）计算预测协方差
        Pxz=zeros(L,1);
        for k=1:2*L+1
            Pxz=Pxz+Wc(k)*(Xaugsigma(:,k)-Xpred)*(Zsigmapre(:,k)-Zpred)';
        end
        

% 7 计算卡尔曼增益
    %（10）计算kalman增益
        K=Pxz*Pzz^-1;
        

% 8 更新状态和协方差
    %（11）状态更新
        Xukf(:,t)=Xpred+K*(Z(t)-Zpred);
        
    %（12）方差更新
        P0=Ppred-K*Pzz*K';

end





for i=1:N
    Err_Obs(i)=RMS(X(:,i),Z(:,i));%滤波前的误差
    Err_UKF(i)=RMS(X(:,i),Xukf(:,i));%滤波后的误差
end
mean_Obs=mean(Err_Obs);
mean_UKF=mean(Err_UKF);

%%%%%%%%%%%
%画图
figure
hold on ;box on
t=(0:1:N-1);
plot(t,X(1,:),'--b','LineWidth',1);%实际轨迹
% plot(t,Z(1,:),'-or','LineWidth',1);%观测轨迹
plot(t,Xukf(1,:),':k','LineWidth',2);%卡尔曼滤波轨迹
% plot(X(1,:),X(3,:),'-k.');
% plot(Xukf(1,:),Xukf(3,:),'-r+');
legend('真实轨迹','UKF轨迹');
xlabel('横坐标 T/s');
ylabel('纵坐标 X/m');

figure
hold on;box on;
plot(t,Err_Obs,'-');
plot(t,Err_UKF,'--');
% plot(t,Err_EKF1,'-.');
% legend('滤波前误差',num2str(mean_Observation),'基本滤波后误差','固定增益滤波后误差');
legend(sprintf('滤波前误差%.03f',mean_Obs),sprintf('UKF滤波后误差%.03f',mean_UKF));
xlabel('观测时间/s');
ylabel('误差值');

%%%%%%%%%%%%%
%子函数
% 计算欧氏距离子函数
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