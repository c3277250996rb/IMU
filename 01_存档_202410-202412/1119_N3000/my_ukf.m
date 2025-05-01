function [Xukf, Z] = my_ukf( N, T, L , ...
                        alpha, kalpha, belta, ramda, ...
                        Wm, Wc, ...
                        s, Z, Xukf, covariation_finally, ...
                        is_use_com, is_continuous);

if is_use_com
    % 打开串口
    s = serial('COM3', 'BaudRate', 115200); % 根据实际情况调整COM端口和波特率
    fopen(s);
    a = 0.061 / 1000;

    % 初始化图形窗口并持久绘图
    figure;
    hold on; % 保留之前绘制的内容
    windowSize = 100; % 窗口大小
end

is_wm_wc = 1;
if is_wm_wc
    % Q：过程噪声的协方差，用于模拟系统中的随机扰动。
    Q = 1;
    % R：观测噪声的协方差，表示测量过程中的随机误差。
    R = 1;
    % Wm(1)=ramda/(L+ramda);%权值Wm的初值需要另外定
    % Wc(1)=ramda/(L+ramda)+1-alpha^2+belta;%权值Wc的初值需要另外定
end

Z(1) = 0;
Zv = zeros(N);

    for t=2:N

        if is_use_com
            temp_Zt = str2double(fgetl(s));
            Z(t) = temp_Zt * a * 10;
            % 暂停一小段时间以控制更新频率（可根据需要调整）
            pause(0.012);
        end

        % 1 获得 2L + 1 个Sigma点及其权值:
            X0 = Xukf(:, t-1); % 获取上一步的UKF点
            C0 = covariation_finally; % 协方差阵
            cho = (chol(C0 * (L + ramda)))';

            for k=1:L
                Xsigma_1_I (:, k)=X0+cho(:,k);
                Xsigma_1_II(:, k)=X0-cho(:,k);
            end

            Xsigma_1  = [X0, Xsigma_1_I, Xsigma_1_II]; % X0是上一步的点，相当于均值点
            % sx = size(Xsigma_1);
            % disp(['sx size: ', num2str(sx)]);


        % 2 将Sigma点通过状态转移函数 gfun(x,t) 预测得到:
            Xsigma_1_predict = my_gfun(Xsigma_1, Z(t), T);


        % 3 计算预测均值和协方差
            Xs_1_p_weight=zeros(1);

            for k=1:2*L+1
                % 检查 Xsigma_1_predict 的大小
                % size_Xsigma_1_predict = size(Xsigma_1_predict);
                % disp(['Xsigma_1_predict size: ', num2str(size_Xsigma_1_predict)]);

                Xs_1_p_weight = Xs_1_p_weight + Wm(k) * Xsigma_1_predict(:,k);
            end

            Xs_1_p_covariation = zeros(L,L);

            for k = 1:2*L+1
                Xs_1_p_covariation =    Xs_1_p_covariation + ...
                                        Wc(k) * ...
                                        (Xsigma_1_predict(:,k) - Xs_1_p_weight) * ...
                                        (Xsigma_1_predict(:,k) - Xs_1_p_weight)';
            end

            if is_wm_wc
                Xs_1_p_covariation = Xs_1_p_covariation + Q;
            end


        % 4 使用UT变换生成新的Sigma点集
            chor = chol((L + ramda) * Xs_1_p_covariation)';

            for k=1:L
                Xsigma_UT_I (:,k) = Xs_1_p_weight + chor(:,k);
                Xsigma_UT_II(:,k) = Xs_1_p_weight - chor(:,k);
            end

            Xsigma_UT = [Xs_1_p_weight, Xsigma_UT_I, Xsigma_UT_II];
            

        % 5 将Sigma点代入观测函数 hfun(x,t) 计算预测观测值
            for k=1:2*L+1
                % Zsigma_UT_predict(1,k)=hfun(Xsigma_UT(:,k),k);
                %                        a0              , v0              , velocity
                Zsigma_UT_predict(1,k) = my_hfun(Xsigma_UT(:,k), Xukf(2, t-1), T);
            end


        % 6 计算预测观测均值和协方差
            % 6.1 计算观测预测加权均值
                Zs_UT_p_weight = 0;
                for k=1:2*L+1
                    Zs_UT_p_weight = Zs_UT_p_weight + Wm(k) * Zsigma_UT_predict(1,k);
                end

            % 6.2 计算观测加权方差
                Zs_UT_p_covariation = 0;

                for k=1:2*L+1
                    Zs_UT_p_covariation =   Zs_UT_p_covariation + ...
                                            Wc(k) * ...
                                            (Zsigma_UT_predict(1,k) - Zs_UT_p_weight) * ...
                                            (Zsigma_UT_predict(1,k) - Zs_UT_p_weight)'; 
                end
                
                if is_wm_wc
                    Zs_UT_p_covariation = Zs_UT_p_covariation + R;
                end

            % 6.3 计算状态与观测之间的协方差
                XZs_UT_p_covariation = zeros(L, 1);
                for k = 1:2*L + 1  
                    XZs_UT_p_covariation =  XZs_UT_p_covariation + ...
                                            Wc(k) * ...
                                            (Xsigma_UT(:,k) - Xs_1_p_weight) * ...
                                            (Zsigma_UT_predict(1,k) - Zs_UT_p_weight)';
                end
                

        % 7 计算卡尔曼增益
            K = XZs_UT_p_covariation * Zs_UT_p_covariation^-1;
                

        % 8 更新状态和协方差
            % 8.1 状态更新
                Xukf(:,t) = Xs_1_p_weight + K * (Z(t) - Zs_UT_p_weight);
                % Xukf(1,t) = Z(t);
            % 8.2 方差更新
                covariation_finally = Xs_1_p_covariation - K * Zs_UT_p_covariation * K';

        % 
            if mod(N , 100) == 0
                disp(N);
                covariation_finally = eye(L);
            end

        % figure 持续绘图
            if is_continuous
                % 更新图形
                fprintf('Xukf: %f\n', Xukf(2,t));
                % Zv(t) = Zv(t - 1) + (Z(i - 1) + Z(i)) /2 *T;
                % plot(t, Zv(t), 'o');
                plot(t, 0, '-k.'); % 真实
                plot(t, Xukf(2,t), '-r+'); % UKF
                title('Real-time Data Plot');
                drawnow;
            end

    %
    end

end
