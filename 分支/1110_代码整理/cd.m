%%%
clear;clc; close all;



% 初始化
    data = readtable('com_data.csv');
    accelerate = data.accel;
    N = 299;
    T = 0.01;

    % N：表示模拟的时间步数（数据点的数量）。
    N = 299;
    % Q：过程噪声的协方差，用于模拟系统中的随机扰动。
    Q = 1;
    % R：观测噪声的协方差，表示测量过程中的随机误差。
    R = 1;
    % covariation_finally：协方差矩阵的初始值，表示状态估计的不确定性。
    covariation_finally = eye(3);

    X = zeros(1,N);

    Z = zeros(1,N);

    Xukf = zeros(1,N);



    % L：状态变量的维度，这里为1，表示一维状态。
    L = 4;    %?个变量

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
    covariation_finally=eye(L);%协方差阵初始化

    T = 0.01;
%

% V 速度优化
for t=2:N


    % 1 获得 2L + 1 个Sigma点及其权值:
        X0 = Xukf(t-1); % 获取上一步的UKF点
        C0 = covariation_finally; % 协方差阵
        cho = (chol(C0 * (L + ramda)))';

        for k=1:L
                Xsigma_1_I(k)=X0+cho(:,k);
                Xsigma_1_II(k)=X0-cho(:,k);
        end

        Xsigma_1  = [X0, Xsigma_1_I, Xsigma_1_II]; % X0是上一步的点，相当于均值点


    % 2 将Sigma点通过状态转移函数 gfun(x,t) 预测得到:
        % Xsigma_1_predict=gfun(Xsigma_1,t);
        %                            v0      , a0      , accelerate 
        Xsigma_1_predict = my_gfun(Xsigma_1, accelerate(t - 1), accelerate(t), T);


    % 3 计算预测均值和协方差
        Xs_1_p_weight=zeros(1);

        for k=1:2*L+1
            Xs_1_p_weight = Xs_1_p_weight + Wm(k) * Xsigma_1_predict(k);
        end

        Xs_1_p_covariation = zeros(L,L);

        for k = 1:2*L+1
            Xs_1_p_covariation =    Xs_1_p_covariation + ...
                                    Wc(k) * ...
                                    (Xsigma_1_predict(k) - Xs_1_p_weight) * ...
                                    (Xsigma_1_predict(k) - Xs_1_p_weight)';
        end

        Xs_1_p_covariation = Xs_1_p_covariation + Q;
        

    % 4 使用UT变换生成新的Sigma点集
        chor = chol((L + ramda) * Xs_1_p_covariation)';

        for k=1:L
            Xsigma_UT_I(k)  = Xs_1_p_weight + chor(:,k);
            Xsigma_UT_II(k) = Xs_1_p_weight - chor(:,k);
        end

        Xsigma_UT = [Xs_1_p_weight, Xsigma_UT_I, Xsigma_UT_II];
        

    % 5 将Sigma点代入观测函数 hfun(x,t) 计算预测观测值
        for k=1:2*L+1
            % Zsigma_UT_predict(1,k)=hfun(Xsigma_UT(:,k),k);
            %                        a0              , v0              , velocity
            Zsigma_UT_predict(k) = my_hfun(accelerate(t - 1), Xsigma(k), Xsigma_UT(k), T);
        end


    % 6 计算预测观测均值和协方差
        % 6.1 计算观测预测加权均值
            Zs_UT_p_weight = 0;
            for k=1:2*L+1
                Zs_UT_p_weight = Zs_UT_p_weight + Wm(k) * Zsigma_UT_predict(k);
            end

        % 6.2 计算观测加权方差
            Zs_UT_p_covariation = 0;

            for k=1:2*L+1
                Zs_UT_p_covariation =   Zs_UT_p_covariation + ...
                                        Wc(k) * ...
                                        (Zsigma_UT_predict(k) - Zs_UT_p_weight) * ...
                                        (Zsigma_UT_predict(k) - Zs_UT_p_weight)'; 
            end
            
            Zs_UT_p_covariation = Zs_UT_p_covariation + R;

        % 6.3 计算状态与观测之间的协方差
            XZs_UT_p_covariation = zeros(L, 1);
            for k = 1:2*L + 1  
                XZs_UT_p_covariation =  XZs_UT_p_covariation + ...
                                        Wc(k) * ...
                                        (Xsigma_UT(:,k) - Xs_1_p_weight) * ...
                                        (Zsigma_UT_predict(:,k) - Zs_UT_p_weight)';
            end
            

    % 7 计算卡尔曼增益
        K = XZs_UT_p_covariation * Zs_UT_p_covariation^-1;
            

    % 8 更新状态和协方差
        % 8.1 状态更新
            % Xukf(:,t)=Xs_1_p_weight+K*(Z(:,t)-Zs_UT_p_weight);
            Xukf(t) = Xs_1_p_weight + K * (accelerate(t) - Zs_UT_p_weight);
        % 8.2 方差更新
            covariation_finally = Xs_1_p_covariation - K * Zs_UT_p_covariation * K';


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



