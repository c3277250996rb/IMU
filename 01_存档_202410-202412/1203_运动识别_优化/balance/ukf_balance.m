function output_status = ukf_balance(input_data, input_start, input_finish, input_mean)

    addpath('tools');
    addpath('data');
    addpath('scripts');
    addpath('balance');
    
    a = 16.384;
    % N = 3000-1;
    % N = public_N;
    N = input_finish - input_start;
    % ��ʼ��
        accelerate = input_data(input_start : input_finish) - input_mean;

        accelerate = accelerate ./ a ./ 10;
        
        figure
        hold on ;box on;
        plot(accelerate ./ 5000 .* 24 ./ 4 .* a .* 10);

        % Q������������Э�������ģ��ϵͳ�е�����Ŷ���
        Q = 1;
        % R���۲�������Э�����ʾ���������е������
        R = 1;
        % L��״̬������ά�ȡ�
        L = 2;    %?������
        % alpha��kalpha��belta��UKF�ĳ����������ڵ���Sigma������ɺ�Ȩ�ء�
        alpha=1;
        kalpha=0;
        belta=2;
        % ramda��UKF����չ������Ӱ��Sigma���ɢ���̶ȡ�
        ramda=3-L;

        % �����UKF�в����õ�һ��`Wm`��`Wc`������Ȩ�أ����ǽ�����Ȩ������Ϊ��ֵͬ���㷢��Ч���������ã������������ԭ���йأ�

        % 1. **״̬ģ�ͺ͹۲�ģ�͵�����**��UKF��Ĭ��Ȩ�����ã�������һ��Sigma���Ȩ������Ϊ���ߣ��ǻ�����ζ�任��һ����裬����ϵͳ�ķ����Գ̶����С�Ȼ����һЩʵ��ϵͳ�У��ر��ǵ�ģ�ͽ�Ϊ���ԣ�����������С����ʼ״̬��׼ȷʱ�������ȵ�Ȩ�طֲ����ܻ����Ч������ϵͳ��ƫ��ĳ������
        
        % 2. **ģ�͵ķ����Գ̶�**��UKFĬ�����õ�����Ȩ�������Ϊ����ǿ�ڸ߶ȷ�����ϵͳ�µ�״̬�������������ڷ����Գ̶Ƚϵ͵�ϵͳ������Ȩ�ؿ��ܻ���������ƫ�ƣ����¹���ֵƫ����ʵֵ���򵥾��ȵ�Ȩ�ط������ʺϴ���ϵͳ��
        
        % 3. **���������͹۲����������Ӱ��**�����ϵͳ�Ĺ���������۲������ϵͣ�ʹ�þ���Ȩ�ؿ��Լ���������״̬���Ƶ�ƫ�ơ���ʱȨ�ص��������ã�ƫ���ڵ�һ��Sigma�㣩���ܻᵼ�¹������γ�ʼ״̬���Ӷ�����ƫ�
        
        % 4. **��ʼ״̬��Э�����趨��Ӱ��**����ʼ״̬��Э�����ѡ���Ӱ��UKFȨ�ص������ԡ���ͬ��ʼ�趨�£�ĳЩȨ����Ͽ��ܻ�ʹϵͳ�����������������Ȩ�ؿ���ʹ״̬��ĳһ��������ƫ�ơ�
        
        % ### �����ʵ�鷽��
        % ���Խ�һ��ʵ�����Ż�UKFȨ�ز������������ `alpha`��`beta` �� `kappa`��Ҳ�������е� `ramda`���������������ܻ��ҵ�һ���ʺ���ϵͳ��Ȩ�����ã�������߹��ƾ��ȣ����ܱ���ƫ�ơ�

        for j=1:2*L+1
            Wm(j)=1/(2*(L+ramda));
            Wc(j)=1/(2*(L+ramda));
        end
        % Wm(1)=ramda/(L+ramda);%ȨֵWm�ĳ�ֵ��Ҫ���ⶨ
        % Wc(1)=ramda/(L+ramda)+1-alpha^2+belta;%ȨֵWc�ĳ�ֵ��Ҫ���ⶨ
        s = zeros(N);
        Z = accelerate;
        Xukf = zeros(L,N);
        covariation_finally=eye(L);%Э�������ʼ��
    %

    % UKF
        T = 0.039;
        % T = 0.0388295;
        is_continuous = 0;
        is_use_com = 0;

        tic; % ��ʼ��ʱ

        % V �ٶ��Ż�
        [Xukf, Z] = my_ukf(  N, T, L , ...
                        alpha, kalpha, belta, ramda, ...
                        Wm, Wc, ...
                        s, Z, Xukf, covariation_finally, ...
                        is_use_com, is_continuous);


        
        elapsedTime = toc; % ������ʱ����ȡ����ʱ��
        fprintf('�����ִ��ʱ��Ϊ %.8f ��\n', elapsedTime);

        Zv = zeros(N);
        % disp(Z);
        for i=2:N
            Zv(i) = Zv(i - 1) + (Z(i - 1) + Z(i)) /2 *T;
        end

        %%%%%%%%%%%
        %��ͼ
        % figure
        hold on ;box on;
        t=(0:1:N-1);
        plot(t,Xukf(2,:));
        % plot(Zv);


        differential = zeros(2,N);
        input_data = Xukf(2,:);
        input_count_size = N;
        input_window_size = 5;
        % differential = calc_differential(input_data, input_count_size, input_window_size);






        is_use_com = 0;
        % distance �����Ż�
        distance = zeros(L, N);
        [distance, Xukf] = my_ukf(  N, T, L , ...
                            alpha, kalpha, belta, ramda, ...
                            Wm, Wc, ...
                            s, Xukf, distance, covariation_finally, ...
                            is_use_com, is_continuous);


        tic; % ��ʼ��ʱ

        Zd = zeros(N);
        for i=2:N
            Zd(i) = Zd(i - 1) + (Xukf(2,i) + Xukf(2,i-1)) /2 *T;
            % Zd(i) = Zd(i - 1) + (Zv(i) + Zv(i-1)) /2 *T;
        end

        elapsedTime = toc; % ������ʱ����ȡ����ʱ��
        fprintf('�����ִ��ʱ��Ϊ %.8f ��\n', elapsedTime);

        %%%%%%%%%%%
        %��ͼ
        % figure
        hold on ;box on;
        t=(0:1:N-1);
        % plot(t, distance(2,:));
        plot(t, Zd);

        legend('a', 'v', 'd');

        % ǿ��ˢ��ͼ�δ��ڣ�������ʾͼ��
        drawnow;
    %
        
    % �ж�
        output_status = 0;
    %

end
        