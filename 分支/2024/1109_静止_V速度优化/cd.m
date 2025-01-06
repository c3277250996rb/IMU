%%%
clear;clc; close all;



% 初始化
    data = readtable('com_data.csv');
    accelerate = data.accel;
    N = 299;
    T = 0.01;

    velocity = zeros(1,N);

    for i = 2:N
        velocity(i) = my_gfun(velocity(i - 1), accelerate(i - 1), accelerate(i), T);
    end


    % N：表示模拟的时间步数（数据点的数量）。
    N = 299;


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
    % Z(1) = hfun(X(1,1));

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



Xsigma_0 = zeros(3);
Xaugsigma_0 = zeros(3);
Zsigmapre_0 = zeros(3);
T = 0.01;


% V 速度优化
for t=2:N

    % 1 获得 2L + 1 个Sigma点及其权值:
                xestimate = Xukf(t-1); % 获取上一步的UKF点
                P = P0; % 协方差阵
                cho=(chol(P*(L+ramda)))';

                for k=1:L
                    % Sigma点集合中的正偏移点。cho(:,k)表示对xestimate加入一个“正偏移”，这个偏移大小由Cholesky分解的矩阵确定。
                        xgamaP1(k)=xestimate+cho(:,k);
                    % Sigma点集合中的负偏移点。对xestimate施加负偏移，以生成另一组对称的Sigma点。
                        xgamaP2(k)=xestimate-cho(:,k);
                end
        
                Xsigma  = [xestimate, xgamaP1, xgamaP2]; % xestimate是上一步的点，相当于均值点

    % 2 将Sigma点通过状态转移函数 gfun(x,t) 预测得到:
        % Xsigmapre=gfun(Xsigma,t);

        %                   v0      , a0      , accelerate 
        Xsigmapre = my_gfun(Xsigma_0, accelerate(t - 1), accelerate(t), T);
        Xsigma = Xsigma_0;
        % Xsigmapre = my_gfun(Xsigma_0, Z(t - 1), Z(t), T);

    % 3 计算预测均值和协方差
                Xpred=zeros(1);

                for k=1:2*L+1
            % 在每次迭代中，乘以权重后累加到 Xpred 中。最后，Xpred 就是所有 sigma 点的加权平均值，即预测均值。
                    Xpred = Xpred +                         ...
        ...% Wm(k) 是对应于第 k 个 sigma 点的权重，用于计算均值。
                            Wm(k)                           ...
        ...% Xsigmapre(:,k) 是第 k 个 sigma 点的状态向量。
                        * Xsigmapre(k);
                end

                Ppred = zeros(L,L);

                for k = 1:2*L+1

                    Ppred = Ppred +                         ...
        ...% Wc(k) 是第 k 个 sigma 点的协方差权重。
                            Wc(k)                           ...
        ...% Xsigmapre(:,k) - Xpred 计算的是第 k 个 sigma 点与预测均值之间的差。
                        * (Xsigmapre(k) - Xpred)        ...
        ...% (Xsigmapre(:,k)-Xpred)' 是这个差的转置，形成一个 L×1 的列向量和一个 1×L 的行向量的乘积，得到一个 L×L 的矩阵。
                        * (Xsigmapre(k) - Xpred)';
                end

                            % % 3.3 将过程噪声协方差矩阵 Q 加入到预测协方差中，以考虑系统的不确定性。
                                Ppred = Ppred + Q;
        
        
    % 4 使用UT变换生成新的Sigma点集
        % 4.1 计算 Cholesky 分解：chor=(chol((L+ramda)*Ppred))';
                % 计算缩放因子
                    scale_factor = (L + ramda) * Ppred;
                % 进行 Cholesky 分解，得到下三角矩阵
                    lower_triangular = chol(scale_factor);
                % 转置下三角矩阵
                    chor = lower_triangular';

            for k=1:L

                XaugsigmaP1(k)=Xpred+chor(:,k);
                XaugsigmaP2(k)=Xpred-chor(:,k);

            end
                Xaugsigma=[Xpred, XaugsigmaP1, XaugsigmaP2];
        
    % 5 将Sigma点代入观测函数 hfun(x,t) 计算预测观测值
            for k=1:2*L+1

        % Zsigmapre(1,k) 将计算得到的观测值存储在 Zsigmapre 的第一行和第 k 列中。这样，Zsigmapre 就会存储所有 sigma 点对应的观测预测值。
                % Zsigmapre(1,k)=hfun(Xaugsigma(:,k),k);

                %                        a0              , v0              , velocity
                Zsigmapre(k) = my_hfun(Zsigmapre_0(k), Xaugsigma_0(k), Xaugsigma(k), T);

            end
                Zsigmapre_0 = Zsigmapre;
                Xaugsigma_0 = Xaugsigma;

    % 6 计算预测观测均值和协方差
        % 6.1 计算观测预测加权均值
            Zpred=0;
            for k=1:2*L+1
                Zpred = Zpred + Wm(k) * Zsigmapre(k);
            end
        % 6.2 计算观测加权方差
            Pzz=0;
            for k=1:2*L+1

                % Pzz = Pzz + Wc(k) * (Zsigmapre(:,k) - Zpred) * (Zsigmapre(:,k) - Zpred)';
                % 计算观测值与均值的差
                    diff = Zsigmapre(k) - Zpred;                
                % 计算外积
                    outer_product = diff * diff';           
                % 加权外积
                    weighted_contribution = Wc(k) * outer_product; 
                % 累加到方差矩阵
                    Pzz = Pzz + weighted_contribution;            

            end
                                % % 加上观测噪声协方差矩阵 R，得到最终的观测预测方差。
                                Pzz=Pzz+R;
        % 6.3 计算状态与观测之间的协方差
            %（9）计算预测协方差
            % Pxz = Pxz + Wc(k) * (Xaugsigma(:,k) - Xpred) * (Zsigmapre(:,k) - Zpred)';
            Pxz = zeros(L, 1);
            for k = 1:2*L + 1  
            % 计算状态差
                state_diff = Xaugsigma(k) - Xpred;      
            % 计算观测差
                obs_diff = Zsigmapre(k) - Zpred;         
            % 计算外积
                outer_product = state_diff * obs_diff';     
            % 加权外积
                weighted_contribution = Wc(k) * outer_product; 
            % 累加到预测协方差
                Pxz = Pxz + weighted_contribution;          
            end
            
    % 7 计算卡尔曼增益
            K = Pxz * Pzz^-1;
            

    % 8 更新状态和协方差
        % 8.1 状态更新
                % Xukf(:,t)=Xpred+K*(Z(:,t)-Zpred);
                Xukf(t) = Xpred + K * (accelerate(t) - Zpred);
        % 8.2 方差更新
                P0 = Ppred - K * Pzz * K';

%
end


% S 位移优化
for t=2:N

    % 1 获得 2L + 1 个Sigma点及其权值:
                xestimate = Xukf(t-1); % 获取上一步的UKF点
                P = P0; % 协方差阵
                cho=(chol(P*(L+ramda)))';

                for k=1:L
                    % Sigma点集合中的正偏移点。cho(:,k)表示对xestimate加入一个“正偏移”，这个偏移大小由Cholesky分解的矩阵确定。
                        xgamaP1(k)=xestimate+cho(:,k);
                    % Sigma点集合中的负偏移点。对xestimate施加负偏移，以生成另一组对称的Sigma点。
                        xgamaP2(k)=xestimate-cho(:,k);
                end
        
                Xsigma  = [xestimate, xgamaP1, xgamaP2]; % xestimate是上一步的点，相当于均值点

    % 2 将Sigma点通过状态转移函数 gfun(x,t) 预测得到:
        % Xsigmapre=gfun(Xsigma,t);

        %                   v0      , a0      , accelerate 
        Xsigmapre = my_gfun(Xsigma_0, accelerate(t - 1), accelerate(t), T);
        Xsigma = Xsigma_0;
        % Xsigmapre = my_gfun(Xsigma_0, Z(t - 1), Z(t), T);

    % 3 计算预测均值和协方差
                Xpred=zeros(1);

                for k=1:2*L+1
            % 在每次迭代中，乘以权重后累加到 Xpred 中。最后，Xpred 就是所有 sigma 点的加权平均值，即预测均值。
                    Xpred = Xpred +                         ...
        ...% Wm(k) 是对应于第 k 个 sigma 点的权重，用于计算均值。
                            Wm(k)                           ...
        ...% Xsigmapre(:,k) 是第 k 个 sigma 点的状态向量。
                        * Xsigmapre(k);
                end

                Ppred = zeros(L,L);

                for k = 1:2*L+1

                    Ppred = Ppred +                         ...
        ...% Wc(k) 是第 k 个 sigma 点的协方差权重。
                            Wc(k)                           ...
        ...% Xsigmapre(:,k) - Xpred 计算的是第 k 个 sigma 点与预测均值之间的差。
                        * (Xsigmapre(k) - Xpred)        ...
        ...% (Xsigmapre(:,k)-Xpred)' 是这个差的转置，形成一个 L×1 的列向量和一个 1×L 的行向量的乘积，得到一个 L×L 的矩阵。
                        * (Xsigmapre(k) - Xpred)';
                end

                            % % 3.3 将过程噪声协方差矩阵 Q 加入到预测协方差中，以考虑系统的不确定性。
                                Ppred = Ppred + Q;
        
        
    % 4 使用UT变换生成新的Sigma点集
        % 4.1 计算 Cholesky 分解：chor=(chol((L+ramda)*Ppred))';
                % 计算缩放因子
                    scale_factor = (L + ramda) * Ppred;
                % 进行 Cholesky 分解，得到下三角矩阵
                    lower_triangular = chol(scale_factor);
                % 转置下三角矩阵
                    chor = lower_triangular';

            for k=1:L

                XaugsigmaP1(k)=Xpred+chor(:,k);
                XaugsigmaP2(k)=Xpred-chor(:,k);

            end
                Xaugsigma=[Xpred, XaugsigmaP1, XaugsigmaP2];
        
    % 5 将Sigma点代入观测函数 hfun(x,t) 计算预测观测值
            for k=1:2*L+1

        % Zsigmapre(1,k) 将计算得到的观测值存储在 Zsigmapre 的第一行和第 k 列中。这样，Zsigmapre 就会存储所有 sigma 点对应的观测预测值。
                % Zsigmapre(1,k)=hfun(Xaugsigma(:,k),k);

                %                        a0              , v0              , velocity
                Zsigmapre(k) = my_hfun(Zsigmapre_0(k), Xaugsigma_0(k), Xaugsigma(k), T);

            end
                Zsigmapre_0 = Zsigmapre;
                Xaugsigma_0 = Xaugsigma;

    % 6 计算预测观测均值和协方差
        % 6.1 计算观测预测加权均值
            Zpred=0;
            for k=1:2*L+1
                Zpred = Zpred + Wm(k) * Zsigmapre(k);
            end
        % 6.2 计算观测加权方差
            Pzz=0;
            for k=1:2*L+1

                % Pzz = Pzz + Wc(k) * (Zsigmapre(:,k) - Zpred) * (Zsigmapre(:,k) - Zpred)';
                % 计算观测值与均值的差
                    diff = Zsigmapre(k) - Zpred;                
                % 计算外积
                    outer_product = diff * diff';           
                % 加权外积
                    weighted_contribution = Wc(k) * outer_product; 
                % 累加到方差矩阵
                    Pzz = Pzz + weighted_contribution;            

            end
                                % % 加上观测噪声协方差矩阵 R，得到最终的观测预测方差。
                                Pzz=Pzz+R;
        % 6.3 计算状态与观测之间的协方差
            %（9）计算预测协方差
            % Pxz = Pxz + Wc(k) * (Xaugsigma(:,k) - Xpred) * (Zsigmapre(:,k) - Zpred)';
            Pxz = zeros(L, 1);
            for k = 1:2*L + 1  
            % 计算状态差
                state_diff = Xaugsigma(k) - Xpred;      
            % 计算观测差
                obs_diff = Zsigmapre(k) - Zpred;         
            % 计算外积
                outer_product = state_diff * obs_diff';     
            % 加权外积
                weighted_contribution = Wc(k) * outer_product; 
            % 累加到预测协方差
                Pxz = Pxz + weighted_contribution;          
            end
            
    % 7 计算卡尔曼增益
            K = Pxz * Pzz^-1;
            

    % 8 更新状态和协方差
        % 8.1 状态更新
                % Xukf(:,t)=Xpred+K*(Z(:,t)-Zpred);
                Xukf(t) = Xpred + K * (accelerate(t) - Zpred);
        % 8.2 方差更新
                P0 = Ppred - K * Pzz * K';

%
end


for i=2:N
    Z(1) = 0;
    Z(i) = my_gfun(Z(i - 1), accelerate(i - 1), accelerate(i), T);
end


%误差分析
    for i=1:N
        Err_KalmanFilter(i)=RMS(X(:,i),Xukf(:,i));
    end

%%%%%%%%%%%
%画图
    figure
    hold on ;box on
    plot(Z,'o');
    plot(0,'-k.');
    plot(Xukf,'-r+');
    legend('观测轨迹','真实轨迹','UKF轨迹')



