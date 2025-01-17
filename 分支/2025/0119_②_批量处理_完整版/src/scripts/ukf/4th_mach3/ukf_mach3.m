function output_map = ukf_3dimensional(input_data, input_start, input_finish, input_mean, input_judge_stop_times)

    % 初始化
        addpath(genpath('data'));
        addpath(genpath('main'));
        addpath(genpath('src'));

        output_map = containers.Map();

        input_is_use_uart = get_is_use_uart();
        input_is_use_rotate_accel = get_is_use_rotate_accel();
        
        a = 16.384;
        N = input_finish - input_start;
        T = 0.039;

        output_calc_data = zeros(N,5);

        % 使用 旋转 零飘
        if input_is_use_rotate_accel
            accelerate = input_data(input_start : input_finish);

        % 不 使用 旋转 零飘
        else
            
            accelerate = input_data(input_start : input_finish);

        end

        accelerate = accelerate - mean(accelerate(1:40));

        accelerate = accelerate ./ a ./ 10;


        % 限幅
            
        %


        % 绘图加速度
            % figure
            % hold on ;box on;
            % plot(accelerate ./ 5000 .* 24 ./ 4 .* a .* 10);
            output_calc_data(:,1) = accelerate(1:end-1) ./ 5000 .* 24 ./ 4 .* a .* 10;
        %

        % UKF参数初始化
            % Q：过程噪声的协方差，用于模拟系统中的随机扰动。
            Q = 1;
            % R：观测噪声的协方差，表示测量过程中的随机误差。
            R = 1;
            % L：状态变量的维度。
            L = 2;    %?个变量
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
            % Wm(1)=ramda/(L+ramda);%权值Wm的初值需要另外定
            % Wc(1)=ramda/(L+ramda)+1-alpha^2+belta;%权值Wc的初值需要另外定
            s = zeros(N);
            Z = accelerate;
            Xukf = zeros(L,N);
            covariation_finally=eye(L);%协方差阵初始化
        %
    %


%%% V 速度处理
        % V 速度 计算
            % a -> v (UKF)
                [Xukf, Z] = my_ukf(  N, T, L , ...
                                alpha, kalpha, belta, ramda, ...
                                Wm, Wc, ...
                                s, Z, Xukf, covariation_finally);
            %

            % a -> v (直接积分)
                Zv = zeros(N);
                for i=2:N
                    Zv(i) = Zv(i - 1) + (Z(i - 1) + Z(i)) /2 *T;
                end
            %
        %

        % V 速度 画图 (UKF)
            % hold on ;box on;
            % t=(0:1:N-1);
            % plot(t,Xukf(2,:));
            original_v_Xukf = Xukf;
            output_calc_data(:,2) = original_v_Xukf(2,:)';
        %

        % V 速度 画图 (直接积分)
            % plot(t,Xukf(2,:));
            output_calc_data(:,3) = Zv(:,1);
            
            if 0 == get_is_use_direct_intergration
                output_calc_data(:,3) = output_calc_data(:,2);
            end
        %
%%%


%%% D 距离处理
        % D 距离 计算
            % v -> d (UKF)
                distance = zeros(L, N);
                [distance, original_v_Xukf] = my_ukf(  N, T, L , ...
                                    alpha, kalpha, belta, ramda, ...
                                    Wm, Wc, ...
                                    s, original_v_Xukf, distance, covariation_finally);
            %

            % v -> d (直接积分) (原始 UKF)
                Zd = zeros(N);
                Zd2 = zeros(N);
                for i=2:N
                    Zd(i) = Zd(i - 1) + (original_v_Xukf(2,i) + original_v_Xukf(2,i-1)) /2 *T;
                    Zd2(i) = Zd2(i - 1) + (Zv(i) + Zv(i-1)) /2 *T;
                end
            %

        %
        
        % D 距离 画图 (UKF 直接积分)
            % plot(t, Zd); 
            output_calc_data(:,4) = Zd(:,1);
        %
        
        
        % D 距离 画图 (直接积分 直接积分)
            % plot(t, Zd);
            output_calc_data(:,5) = Zd2(:,1);

            if 0 == get_is_use_direct_intergration
                output_calc_data(:,5) = output_calc_data(:,4);
            end
        %
%%%


        % 画图 标注
            % legend('a', 'v', 'v', 'd', 'd');

            % 强制刷新图形窗口，立即显示图形
            % drawnow;
        %

        % Map 赋值
            output_map('output_calc_data') = output_calc_data;
            % output_map('output_I_index') = output_I_index;
        %
    %
end
        