%%%
clear;clc; close all;



% ��ʼ��
    data = readtable('com_data.csv');
    accelerate = data.accel;
    N = 299;
    T = 0.01;

    velocity = zeros(1,N);

    for i = 2:N
        velocity(i) = my_gfun(velocity(i - 1), accelerate(i - 1), accelerate(i), T);
    end


    % N����ʾģ���ʱ�䲽�������ݵ����������
    N = 299;


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
    % Z(1) = hfun(X(1,1));

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



Xsigma_0 = zeros(3);
Xaugsigma_0 = zeros(3);
Zsigmapre_0 = zeros(3);
T = 0.01;


% V �ٶ��Ż�
for t=2:N

    % 1 ��� 2L + 1 ��Sigma�㼰��Ȩֵ:
                xestimate = Xukf(t-1); % ��ȡ��һ����UKF��
                P = P0; % Э������
                cho=(chol(P*(L+ramda)))';

                for k=1:L
                    % Sigma�㼯���е���ƫ�Ƶ㡣cho(:,k)��ʾ��xestimate����һ������ƫ�ơ������ƫ�ƴ�С��Cholesky�ֽ�ľ���ȷ����
                        xgamaP1(k)=xestimate+cho(:,k);
                    % Sigma�㼯���еĸ�ƫ�Ƶ㡣��xestimateʩ�Ӹ�ƫ�ƣ���������һ��ԳƵ�Sigma�㡣
                        xgamaP2(k)=xestimate-cho(:,k);
                end
        
                Xsigma  = [xestimate, xgamaP1, xgamaP2]; % xestimate����һ���ĵ㣬�൱�ھ�ֵ��

    % 2 ��Sigma��ͨ��״̬ת�ƺ��� gfun(x,t) Ԥ��õ�:
        % Xsigmapre=gfun(Xsigma,t);

        %                   v0      , a0      , accelerate 
        Xsigmapre = my_gfun(Xsigma_0, accelerate(t - 1), accelerate(t), T);
        Xsigma = Xsigma_0;
        % Xsigmapre = my_gfun(Xsigma_0, Z(t - 1), Z(t), T);

    % 3 ����Ԥ���ֵ��Э����
                Xpred=zeros(1);

                for k=1:2*L+1
            % ��ÿ�ε����У�����Ȩ�غ��ۼӵ� Xpred �С����Xpred �������� sigma ��ļ�Ȩƽ��ֵ����Ԥ���ֵ��
                    Xpred = Xpred +                         ...
        ...% Wm(k) �Ƕ�Ӧ�ڵ� k �� sigma ���Ȩ�أ����ڼ����ֵ��
                            Wm(k)                           ...
        ...% Xsigmapre(:,k) �ǵ� k �� sigma ���״̬������
                        * Xsigmapre(k);
                end

                Ppred = zeros(L,L);

                for k = 1:2*L+1

                    Ppred = Ppred +                         ...
        ...% Wc(k) �ǵ� k �� sigma ���Э����Ȩ�ء�
                            Wc(k)                           ...
        ...% Xsigmapre(:,k) - Xpred ������ǵ� k �� sigma ����Ԥ���ֵ֮��Ĳ
                        * (Xsigmapre(k) - Xpred)        ...
        ...% (Xsigmapre(:,k)-Xpred)' ��������ת�ã��γ�һ�� L��1 ����������һ�� 1��L ���������ĳ˻����õ�һ�� L��L �ľ���
                        * (Xsigmapre(k) - Xpred)';
                end

                            % % 3.3 ����������Э������� Q ���뵽Ԥ��Э�����У��Կ���ϵͳ�Ĳ�ȷ���ԡ�
                                Ppred = Ppred + Q;
        
        
    % 4 ʹ��UT�任�����µ�Sigma�㼯
        % 4.1 ���� Cholesky �ֽ⣺chor=(chol((L+ramda)*Ppred))';
                % ������������
                    scale_factor = (L + ramda) * Ppred;
                % ���� Cholesky �ֽ⣬�õ������Ǿ���
                    lower_triangular = chol(scale_factor);
                % ת�������Ǿ���
                    chor = lower_triangular';

            for k=1:L

                XaugsigmaP1(k)=Xpred+chor(:,k);
                XaugsigmaP2(k)=Xpred-chor(:,k);

            end
                Xaugsigma=[Xpred, XaugsigmaP1, XaugsigmaP2];
        
    % 5 ��Sigma�����۲⺯�� hfun(x,t) ����Ԥ��۲�ֵ
            for k=1:2*L+1

        % Zsigmapre(1,k) ������õ��Ĺ۲�ֵ�洢�� Zsigmapre �ĵ�һ�к͵� k ���С�������Zsigmapre �ͻ�洢���� sigma ���Ӧ�Ĺ۲�Ԥ��ֵ��
                % Zsigmapre(1,k)=hfun(Xaugsigma(:,k),k);

                %                        a0              , v0              , velocity
                Zsigmapre(k) = my_hfun(Zsigmapre_0(k), Xaugsigma_0(k), Xaugsigma(k), T);

            end
                Zsigmapre_0 = Zsigmapre;
                Xaugsigma_0 = Xaugsigma;

    % 6 ����Ԥ��۲��ֵ��Э����
        % 6.1 ����۲�Ԥ���Ȩ��ֵ
            Zpred=0;
            for k=1:2*L+1
                Zpred = Zpred + Wm(k) * Zsigmapre(k);
            end
        % 6.2 ����۲��Ȩ����
            Pzz=0;
            for k=1:2*L+1

                % Pzz = Pzz + Wc(k) * (Zsigmapre(:,k) - Zpred) * (Zsigmapre(:,k) - Zpred)';
                % ����۲�ֵ���ֵ�Ĳ�
                    diff = Zsigmapre(k) - Zpred;                
                % �������
                    outer_product = diff * diff';           
                % ��Ȩ���
                    weighted_contribution = Wc(k) * outer_product; 
                % �ۼӵ��������
                    Pzz = Pzz + weighted_contribution;            

            end
                                % % ���Ϲ۲�����Э������� R���õ����յĹ۲�Ԥ�ⷽ�
                                Pzz=Pzz+R;
        % 6.3 ����״̬��۲�֮���Э����
            %��9������Ԥ��Э����
            % Pxz = Pxz + Wc(k) * (Xaugsigma(:,k) - Xpred) * (Zsigmapre(:,k) - Zpred)';
            Pxz = zeros(L, 1);
            for k = 1:2*L + 1  
            % ����״̬��
                state_diff = Xaugsigma(k) - Xpred;      
            % ����۲��
                obs_diff = Zsigmapre(k) - Zpred;         
            % �������
                outer_product = state_diff * obs_diff';     
            % ��Ȩ���
                weighted_contribution = Wc(k) * outer_product; 
            % �ۼӵ�Ԥ��Э����
                Pxz = Pxz + weighted_contribution;          
            end
            
    % 7 ���㿨��������
            K = Pxz * Pzz^-1;
            

    % 8 ����״̬��Э����
        % 8.1 ״̬����
                % Xukf(:,t)=Xpred+K*(Z(:,t)-Zpred);
                Xukf(t) = Xpred + K * (accelerate(t) - Zpred);
        % 8.2 �������
                P0 = Ppred - K * Pzz * K';

%
end


% S λ���Ż�
for t=2:N

    % 1 ��� 2L + 1 ��Sigma�㼰��Ȩֵ:
                xestimate = Xukf(t-1); % ��ȡ��һ����UKF��
                P = P0; % Э������
                cho=(chol(P*(L+ramda)))';

                for k=1:L
                    % Sigma�㼯���е���ƫ�Ƶ㡣cho(:,k)��ʾ��xestimate����һ������ƫ�ơ������ƫ�ƴ�С��Cholesky�ֽ�ľ���ȷ����
                        xgamaP1(k)=xestimate+cho(:,k);
                    % Sigma�㼯���еĸ�ƫ�Ƶ㡣��xestimateʩ�Ӹ�ƫ�ƣ���������һ��ԳƵ�Sigma�㡣
                        xgamaP2(k)=xestimate-cho(:,k);
                end
        
                Xsigma  = [xestimate, xgamaP1, xgamaP2]; % xestimate����һ���ĵ㣬�൱�ھ�ֵ��

    % 2 ��Sigma��ͨ��״̬ת�ƺ��� gfun(x,t) Ԥ��õ�:
        % Xsigmapre=gfun(Xsigma,t);

        %                   v0      , a0      , accelerate 
        Xsigmapre = my_gfun(Xsigma_0, accelerate(t - 1), accelerate(t), T);
        Xsigma = Xsigma_0;
        % Xsigmapre = my_gfun(Xsigma_0, Z(t - 1), Z(t), T);

    % 3 ����Ԥ���ֵ��Э����
                Xpred=zeros(1);

                for k=1:2*L+1
            % ��ÿ�ε����У�����Ȩ�غ��ۼӵ� Xpred �С����Xpred �������� sigma ��ļ�Ȩƽ��ֵ����Ԥ���ֵ��
                    Xpred = Xpred +                         ...
        ...% Wm(k) �Ƕ�Ӧ�ڵ� k �� sigma ���Ȩ�أ����ڼ����ֵ��
                            Wm(k)                           ...
        ...% Xsigmapre(:,k) �ǵ� k �� sigma ���״̬������
                        * Xsigmapre(k);
                end

                Ppred = zeros(L,L);

                for k = 1:2*L+1

                    Ppred = Ppred +                         ...
        ...% Wc(k) �ǵ� k �� sigma ���Э����Ȩ�ء�
                            Wc(k)                           ...
        ...% Xsigmapre(:,k) - Xpred ������ǵ� k �� sigma ����Ԥ���ֵ֮��Ĳ
                        * (Xsigmapre(k) - Xpred)        ...
        ...% (Xsigmapre(:,k)-Xpred)' ��������ת�ã��γ�һ�� L��1 ����������һ�� 1��L ���������ĳ˻����õ�һ�� L��L �ľ���
                        * (Xsigmapre(k) - Xpred)';
                end

                            % % 3.3 ����������Э������� Q ���뵽Ԥ��Э�����У��Կ���ϵͳ�Ĳ�ȷ���ԡ�
                                Ppred = Ppred + Q;
        
        
    % 4 ʹ��UT�任�����µ�Sigma�㼯
        % 4.1 ���� Cholesky �ֽ⣺chor=(chol((L+ramda)*Ppred))';
                % ������������
                    scale_factor = (L + ramda) * Ppred;
                % ���� Cholesky �ֽ⣬�õ������Ǿ���
                    lower_triangular = chol(scale_factor);
                % ת�������Ǿ���
                    chor = lower_triangular';

            for k=1:L

                XaugsigmaP1(k)=Xpred+chor(:,k);
                XaugsigmaP2(k)=Xpred-chor(:,k);

            end
                Xaugsigma=[Xpred, XaugsigmaP1, XaugsigmaP2];
        
    % 5 ��Sigma�����۲⺯�� hfun(x,t) ����Ԥ��۲�ֵ
            for k=1:2*L+1

        % Zsigmapre(1,k) ������õ��Ĺ۲�ֵ�洢�� Zsigmapre �ĵ�һ�к͵� k ���С�������Zsigmapre �ͻ�洢���� sigma ���Ӧ�Ĺ۲�Ԥ��ֵ��
                % Zsigmapre(1,k)=hfun(Xaugsigma(:,k),k);

                %                        a0              , v0              , velocity
                Zsigmapre(k) = my_hfun(Zsigmapre_0(k), Xaugsigma_0(k), Xaugsigma(k), T);

            end
                Zsigmapre_0 = Zsigmapre;
                Xaugsigma_0 = Xaugsigma;

    % 6 ����Ԥ��۲��ֵ��Э����
        % 6.1 ����۲�Ԥ���Ȩ��ֵ
            Zpred=0;
            for k=1:2*L+1
                Zpred = Zpred + Wm(k) * Zsigmapre(k);
            end
        % 6.2 ����۲��Ȩ����
            Pzz=0;
            for k=1:2*L+1

                % Pzz = Pzz + Wc(k) * (Zsigmapre(:,k) - Zpred) * (Zsigmapre(:,k) - Zpred)';
                % ����۲�ֵ���ֵ�Ĳ�
                    diff = Zsigmapre(k) - Zpred;                
                % �������
                    outer_product = diff * diff';           
                % ��Ȩ���
                    weighted_contribution = Wc(k) * outer_product; 
                % �ۼӵ��������
                    Pzz = Pzz + weighted_contribution;            

            end
                                % % ���Ϲ۲�����Э������� R���õ����յĹ۲�Ԥ�ⷽ�
                                Pzz=Pzz+R;
        % 6.3 ����״̬��۲�֮���Э����
            %��9������Ԥ��Э����
            % Pxz = Pxz + Wc(k) * (Xaugsigma(:,k) - Xpred) * (Zsigmapre(:,k) - Zpred)';
            Pxz = zeros(L, 1);
            for k = 1:2*L + 1  
            % ����״̬��
                state_diff = Xaugsigma(k) - Xpred;      
            % ����۲��
                obs_diff = Zsigmapre(k) - Zpred;         
            % �������
                outer_product = state_diff * obs_diff';     
            % ��Ȩ���
                weighted_contribution = Wc(k) * outer_product; 
            % �ۼӵ�Ԥ��Э����
                Pxz = Pxz + weighted_contribution;          
            end
            
    % 7 ���㿨��������
            K = Pxz * Pzz^-1;
            

    % 8 ����״̬��Э����
        % 8.1 ״̬����
                % Xukf(:,t)=Xpred+K*(Z(:,t)-Zpred);
                Xukf(t) = Xpred + K * (accelerate(t) - Zpred);
        % 8.2 �������
                P0 = Ppred - K * Pzz * K';

%
end


for i=2:N
    Z(1) = 0;
    Z(i) = my_gfun(Z(i - 1), accelerate(i - 1), accelerate(i), T);
end


%������
    for i=1:N
        Err_KalmanFilter(i)=RMS(X(:,i),Xukf(:,i));
    end

%%%%%%%%%%%
%��ͼ
    figure
    hold on ;box on
    plot(Z,'o');
    plot(0,'-k.');
    plot(Xukf,'-r+');
    legend('�۲�켣','��ʵ�켣','UKF�켣')



