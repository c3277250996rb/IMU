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


% s：真实状态的存储数组，表示系统的真实状态。
s = zeros(1,N);
s(1,1) = 0.1;

% X：预测状态值的存储数组，模拟系统的实际输出。
        % 定义：预测状态 X 是通过状态转移方程（即 gfun）预测出的状态估计。在 UKF 中，这个状态通过加权求平均来近似真实状态。
        % 在你的加速度计项目中：当我们使用上一时刻的状态去预测当前的位移、速度和加速度时，得到的就是预测状态 X。例如，如果知道上一时刻的速度 v 和加速度 a，可以通过物理公式预测出下一时刻的位移和速度。
X = zeros(1,N);
X(1,1) = 0.1;

% Z：观测值的存储数组，包含受噪声影响的观测数据。
        % 定义：观测值 Z 是传感器提供的直接观测数据。在 UKF 中，观测值是通过 hfun 映射得到的，用来将状态空间映射到传感器的测量空间。
        % 在你的加速度计项目中：加速度计测量到的加速度数据就是观测值 Z。它通常只有加速度一项，因为加速度计只能直接测量加速度。
Z = zeros(1,N);
Z(1) = hfun(X(1,1)) + v(1);

% Xukf：UKF估计的状态，存储滤波后的状态值。
        % 定义：估计状态 Xukf 是 UKF 通过融合预测状态和观测值后的最优估计。它是滤波器计算得出的“最佳”状态，用于接近真实状态。
        % 在你的加速度计项目中：通过 UKF 更新后的状态估计结果，即包含估计的位移 xukf 、速度 vukf 和加速度 aukf 的向量，表示最终估计出的位移、速度和加速度。这些估计结果综合了预测信息和观测信息，是对真实状态的最佳近似。
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
    

% 2 将Sigma点通过状态转移函数 gfun(x,t) 预测得到:
    Xsigmapre=gfun(Xsigma,t);
    

% 3 计算预测均值和协方差

    % 3.1 计算加权均值
        % Xpred 初始化为一个全零的列向量，大小为 L×1，其中 L 是状态的维度。
            Xpred=zeros(L,1);

            for k=1:2*L+1

        % 在每次迭代中，乘以权重后累加到 Xpred 中。最后，Xpred 就是所有 sigma 点的加权平均值，即预测均值。
                Xpred = Xpred +                         ...
...     % Wm(k) 是对应于第 k 个 sigma 点的权重，用于计算均值。
                        Wm(k)                           ...
...     % Xsigmapre(:,k) 是第 k 个 sigma 点的状态向量。
                      * Xsigmapre(:,k);

            end

    % 3.2 计算加权方差
        % Ppred 初始化为 L×L 的全零矩阵，用于存储协方差矩阵。
            Ppred = zeros(L,L);

            for k = 1:2*L+1

                Ppred = Ppred +                         ...
...     % Wc(k) 是第 k 个 sigma 点的协方差权重。
                        Wc(k)                           ...
...     % Xsigmapre(:,k) - Xpred 计算的是第 k 个 sigma 点与预测均值之间的差。
                      * (Xsigmapre(:,k) - Xpred)        ...
...     % (Xsigmapre(:,k)-Xpred)' 是这个差的转置，形成一个 L×1 的列向量和一个 1×L 的行向量的乘积，得到一个 L×L 的矩阵。
                      * (Xsigmapre(:,k) - Xpred)';

            end

    % 3.3 将过程噪声协方差矩阵 Q 加入到预测协方差中，以考虑系统的不确定性。
        Ppred = Ppred + Q;
    
    
% 4 使用UT变换生成新的Sigma点集
    %（5）根据预测值，再一次使用UT变换，得到新的sigma点集

    % 4.1 计算 Cholesky 分解：chor=(chol((L+ramda)*Ppred))';

        % 在无迹变换（UT）生成新的 Sigma 点的过程中，Cholesky 分解主要用于确定 Sigma 点的分布范围。具体来说，它的作用如下：
            % 1.生成扰动向量  ：通过 Cholesky 分解得到的矩阵 chor，用于生成扰动向量。这些扰动向量在均值 Xpred 的周围分布，确保了 Sigma 点能够代表状态分布的协方差结构。
            % 2.保留协方差结构：chol((L+ramda)*Ppred) 的分解结果提供了一组线性变换，使得我们在 Xpred 周围分布的 Sigma 点能够反映预测协方差 Ppred 的特征。这些分布点包含了预测分布的统计特性，例如均值和协方差矩阵。
            % 3.正负方向的扰动：通过 Cholesky 分解后的矩阵 chor，我们可以向 Xpred 加入或减去这些扰动，以形成 Sigma 点的对称分布，确保无偏性。这种对称分布是 UT 的核心，保证了对系统非线性变换后的均值和协方差的准确逼近。
        % 综上，Cholesky 分解在这里的作用是确保生成的 Sigma 点能够准确表征状态的统计特性（尤其是协方差结构），并在均值 Xpred 的基础上加入相应的正向和负向扰动，以构建无偏的分布。

            % 计算缩放因子
                scale_factor = (L + ramda) * Ppred;
            % 进行 Cholesky 分解，得到下三角矩阵
                lower_triangular = chol(scale_factor);
            % 转置下三角矩阵
                chor = lower_triangular';

    % 4.2 生成 sigma 点：
        for k=1:L

        % 为第 k 个 sigma 点添加正向扰动，即在预测均值 Xpred 的基础上加上 Cholesky 矩阵的第 k 列。
            XaugsigmaP1(:,k)=Xpred+chor(:,k);
        % 为第 k 个 sigma 点添加负向扰动，即在预测均值 Xpred 的基础上减去 Cholesky 矩阵的第 k 列。
            XaugsigmaP2(:,k)=Xpred-chor(:,k);

        end

    % 4.3 组合 sigma 点：
        % 将预测均值 Xpred 与生成的正向和负向 sigma 点组合成一个新的 sigma 点集 Xaugsigma。这个新的 sigma 点集包含了一个中心点和 2L 个扰动点，总共 2L+1 个 sigma 点。
            Xaugsigma=[Xpred XaugsigmaP1 XaugsigmaP2];
    
% 5 将Sigma点代入观测函数 hfun(x,t) 计算预测观测值
    %（6）观测预测

        for k=1:2*L+1
    % Zsigmapre(1,k) 将计算得到的观测值存储在 Zsigmapre 的第一行和第 k 列中。这样，Zsigmapre 就会存储所有 sigma 点对应的观测预测值。
            Zsigmapre(1,k)=hfun(Xaugsigma(:,k),k);
        end
    

% 6 计算预测观测均值和协方差

    % 6.1 计算观测预测加权均值
        Zpred=0;
        for k=1:2*L+1
        % 对每个 sigma 点，通过权重 Wm(k) 加权观测预测值 Zsigmapre(:,k)，并累加到 Zpred。
        % 这一步的结果是 Zpred 变成了所有 sigma 点观测值的加权均值，即预测的观测值。
            Zpred = Zpred + Wm(k) * Zsigmapre(:,k);
        end
    % 6.2 计算观测加权方差
        Pzz=0;
        for k=1:2*L+1

            % Pzz = Pzz + Wc(k) * (Zsigmapre(:,k) - Zpred) * (Zsigmapre(:,k) - Zpred)';
            % 计算观测值与均值的差
                diff = Zsigmapre(:,k) - Zpred;                
            % 计算外积
                outer_product = diff * diff';           
            % 加权外积
                weighted_contribution = Wc(k) * outer_product; 
            % 累加到方差矩阵
                Pzz = Pzz + weighted_contribution;            

        end
        % 加上观测噪声协方差矩阵 R，得到最终的观测预测方差。
        Pzz=Pzz+R;
    
    % 6.3 计算状态与观测之间的协方差
        %（9）计算预测协方差
        % Pxz = Pxz + Wc(k) * (Xaugsigma(:,k) - Xpred) * (Zsigmapre(:,k) - Zpred)';
            
        Pxz = zeros(L, 1);
        for k = 1:2*L + 1  
        % 计算状态差
            state_diff = Xaugsigma(:, k) - Xpred;      
        % 计算观测差
            obs_diff = Zsigmapre(:, k) - Zpred;         
        % 计算外积
            outer_product = state_diff * obs_diff';     
        % 加权外积
            weighted_contribution = Wc(k) * outer_product; 
        % 累加到预测协方差
            Pxz = Pxz + weighted_contribution;          
        end

        
% 7 计算卡尔曼增益
    %（10）计算kalman增益
    % Pxz 是状态与观测之间的协方差。
    % Pzz^-1 是观测预测方差的逆矩阵，表示观测的不确定性。
    % 卡尔曼增益 K 通过将状态与观测的协方差乘以观测方差的逆得到。这个增益用于确定在更新状态估计时，观测数据的影响程度。
        K = Pxz * Pzz^-1;
        

% 8 更新状态和协方差
    % 8.1 状态更新

        % Z(t) 是在时间 t 时刻的实际观测值。
        % Zpred 是根据当前状态预测的观测值。
        % (Z(t) - Zpred) 计算了观测残差，即实际观测与预测观测之间的差。
        % 将这个残差乘以卡尔曼增益 K，然后加到预测状态 Xpred 上，得到更新后的状态估计 Xukf(:, t)。
            Xukf(:, t) = Xpred + K * (Z(t) - Zpred);

    % 8.2 方差更新

        % Ppred 是预测的状态协方差矩阵。
        % K * Pzz * K' 计算了卡尔曼增益与观测方差之间的乘积。这一项表示更新后状态的不确定性。
        % 最终将这项从预测协方差中减去，得到更新后的协方差矩阵 P0，反映了在观测更新后状态的不确定性减少。
            P0 = Ppred - K * Pzz * K';

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
plot(t,Z(1,:),'-or','LineWidth',1);%观测轨迹
plot(t,Xukf(1,:),':k','LineWidth',2);%卡尔曼滤波轨迹
% plot(X(1,:),X(3,:),'-k.');
% plot(Xukf(1,:),Xukf(3,:),'-r+');
legend('真实轨迹','观测轨迹','UKF轨迹');
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
% function dist=RMS(X1,X2)
% if length(X2)<=2
%     dist=sqrt((X1(1)-X2(1))^2);
% else
%     dist=sqrt((X1(1)-X2(1))^2);
% end
% end



% % 状态转移函数, 已知当前位移和速度，通过gfun，结合加速度和时间步长，可以预测下一时刻的位移和速度。
% function res=gfun(Xekf,k)
%     res=0.5.*Xekf + 25.*Xekf./(1 + Xekf.^2) + 8.*cos(0.3.*(k));
% end



% % hfun 从状态向量 x 中“提取”出加速度这个变量，因为这是加速度计观测到的内容。
% function res=hfun(X,k)
% res=X^2/20;
% end
%%%%%%%%%%%%%