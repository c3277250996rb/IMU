function icd(my_char)

    a = 16.384;
    % N = 3000-1;
    N = public_N;
    % 初始化
        data = readtable('adjusted_data.csv');
        
        picture_name_v = [my_char, '-v.png']; picture_name_s = [my_char , '-s.png'];
        if my_char == 'x'
            accelerate = data.x;
        end
        if my_char == 'y'
            accelerate = data.y;
        end
        if my_char == 'z'
            accelerate = data.z;
        end 

        accelerate = accelerate ./ a ./ 10;

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

        % 如果在UKF中不设置第一个`Wm`和`Wc`的特殊权重，而是将所有权重设置为相同值，你发现效果反而更好，这可能与以下原因有关：

        % 1. **状态模型和观测模型的特性**：UKF的默认权重设置（即将第一个Sigma点的权重设置为更高）是基于无味变换的一般假设，假设系统的非线性程度适中。然而在一些实际系统中（特别是当模型较为线性，或者噪声较小、初始状态较准确时），均匀的权重分布可能会更有效，避免系统逐步偏向某个方向。
        
        % 2. **模型的非线性程度**：UKF默认设置的特殊权重设计是为了增强在高度非线性系统下的状态估计能力。对于非线性程度较低的系统，特殊权重可能会引入过多的偏移，导致估计值偏离真实值。简单均匀的权重反而更适合此类系统。
        
        % 3. **过程噪声和观测噪声的相对影响**：如果系统的过程噪声或观测噪声较低，使用均匀权重可以减少噪声对状态估计的偏移。此时权重的特殊设置（偏向于第一个Sigma点）可能会导致过度信任初始状态，从而引入偏差。
        
        % 4. **初始状态和协方差设定的影响**：初始状态和协方差的选择会影响UKF权重的适用性。不同初始设定下，某些权重组合可能会使系统更快地收敛，而特殊权重可能使状态在某一方向上有偏移。
        
        % ### 建议的实验方法
        % 可以进一步实验来优化UKF权重参数，比如调整 `alpha`、`beta` 或 `kappa`（也即代码中的 `ramda`）参数。这样可能会找到一种适合你系统的权重配置，既能提高估计精度，又能避免偏移。

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

    T = 0.039;
    % T = 0.0388295;
    is_continuous = 0;
    is_use_com = 0;

    tic; % 开始计时

    % V 速度优化
    [Xukf, Z] = my_ukf(  N, T, L , ...
                    alpha, kalpha, belta, ramda, ...
                    Wm, Wc, ...
                    s, Z, Xukf, covariation_finally, ...
                    is_use_com, is_continuous);


    
    elapsedTime = toc; % 结束计时并获取所用时间
    fprintf('代码块执行时间为 %.4f 秒\n', elapsedTime);

    Zv = zeros(N);
    % disp(Z);
    for i=2:N
        Zv(i) = Zv(i - 1) + (Z(i - 1) + Z(i)) /2 *T;
    end

    %%%%%%%%%%%
    %画图
    % figure
    hold on ;box on;;
    t=(0:1:N-1);
    % plot(t,Zv);
    % plot(t,Xukf(2,:));

    % 保存图像为PNG文件
    % title(picture_name_v);
    % saveas(gcf, picture_name_v);


    for i = 2:N
        if i > 704
            Xukf(2,i) = 0;
        end
    end
    % csvwrite('data\velocity.csv', Xukf(2,:)');
    % insert_first_row('data\velocity.csv','x');

    plot(t,Xukf(2,:));

    is_use_com = 0;
    % distance 距离优化
    distance = zeros(L, N);
    [distance, Xukf] = my_ukf(  N, T, L , ...
                        alpha, kalpha, belta, ramda, ...
                        Wm, Wc, ...
                        s, Xukf, distance, covariation_finally, ...
                        is_use_com, is_continuous);

    % Xukf = readtable('data\velocity.csv');
    % Xukf_x = Xukf.x;
    % plot(t,Xukf_x(2,:));


    tic; % 开始计时

    Zd = zeros(N);
    for i=2:N
        % Zd(i) = Zd(i - 1) + (Zv(i) + Zv(i-1)) /2 *T;
        Zd(i) = Zd(i - 1) + (Xukf(2,i) + Xukf(2,i-1)) /2 *T;
        % Zd(i) = Zd(i - 1) + (Xukf_x(i) + Xukf_x(i-1)) /2 *T;
    end


    elapsedTime = toc; % 结束计时并获取所用时间
    fprintf('代码块执行时间为 %.4f 秒\n', elapsedTime);




    %%%%%%%%%%%
    %画图
    % figure
    hold on ;box on;
    t=(0:1:N-1);
    plot(Zd);
    plot(t, distance(2,:));

    % 保存图像为PNG文件
    title(picture_name_s);
    % saveas(gcf, picture_name_s);

    % iwrite('data\share.csv', 1, 1, distance(2,N-1));

    %误差分析
        % for i=1:N
        %     Err_KalmanFilter(i)=RMS(X(:,i),Xukf(:,i));
        % end

end
        