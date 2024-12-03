%%%
clear;clc; close all;



% ��ʼ��
    data = readtable('com_data.csv');
    accelerate = data.accel;
    N = 299;
    T = 0.01;

    % N����ʾģ���ʱ�䲽�������ݵ����������
    N = 299;
    % Q������������Э�������ģ��ϵͳ�е�����Ŷ���
    Q = 1;
    % R���۲�������Э�����ʾ���������е������
    R = 1;
    % covariation_finally��Э�������ĳ�ʼֵ����ʾ״̬���ƵĲ�ȷ���ԡ�
    covariation_finally = eye(3);

    X = zeros(1,N);

    Z = zeros(1,N);

    Xukf = zeros(1,N);



    % L��״̬������ά�ȣ�����Ϊ1����ʾһά״̬��
    L = 4;    %?������

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
    covariation_finally=eye(L);%Э�������ʼ��

    T = 0.01;
%

% V �ٶ��Ż�
for t=2:N


    % 1 ��� 2L + 1 ��Sigma�㼰��Ȩֵ:
        X0 = Xukf(t-1); % ��ȡ��һ����UKF��
        C0 = covariation_finally; % Э������
        cho = (chol(C0 * (L + ramda)))';

        for k=1:L
                Xsigma_1_I(k)=X0+cho(:,k);
                Xsigma_1_II(k)=X0-cho(:,k);
        end

        Xsigma_1  = [X0, Xsigma_1_I, Xsigma_1_II]; % X0����һ���ĵ㣬�൱�ھ�ֵ��


    % 2 ��Sigma��ͨ��״̬ת�ƺ��� gfun(x,t) Ԥ��õ�:
        % Xsigma_1_predict=gfun(Xsigma_1,t);
        %                            v0      , a0      , accelerate 
        Xsigma_1_predict = my_gfun(Xsigma_1, accelerate(t - 1), accelerate(t), T);


    % 3 ����Ԥ���ֵ��Э����
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
        

    % 4 ʹ��UT�任�����µ�Sigma�㼯
        chor = chol((L + ramda) * Xs_1_p_covariation)';

        for k=1:L
            Xsigma_UT_I(k)  = Xs_1_p_weight + chor(:,k);
            Xsigma_UT_II(k) = Xs_1_p_weight - chor(:,k);
        end

        Xsigma_UT = [Xs_1_p_weight, Xsigma_UT_I, Xsigma_UT_II];
        

    % 5 ��Sigma�����۲⺯�� hfun(x,t) ����Ԥ��۲�ֵ
        for k=1:2*L+1
            % Zsigma_UT(1,k)=hfun(Xsigma_UT(:,k),k);
            %                        a0              , v0              , velocity
            Zsigma_UT(k) = my_hfun(accelerate(t - 1), Xsigma(k), Xsigma_UT(k), T);
        end


    % 6 ����Ԥ��۲��ֵ��Э����
        % 6.1 ����۲�Ԥ���Ȩ��ֵ
            Zs_UT_weight = 0;
            for k=1:2*L+1
                Zs_UT_weight = Zs_UT_weight + Wm(k) * Zsigma_UT(k);
            end

        % 6.2 ����۲��Ȩ����
            Zs_UT_covariation = 0;

            for k=1:2*L+1
                Zs_UT_covariation =   Zs_UT_covariation + ...
                                        Wc(k) * ...
                                        (Zsigma_UT(k) - Zs_UT_weight) * ...
                                        (Zsigma_UT(k) - Zs_UT_weight)'; 
            end
            
            Zs_UT_covariation = Zs_UT_covariation + R;

        % 6.3 ����״̬��۲�֮���Э����
            XZs_UT_covariation = zeros(L, 1);
            for k = 1:2*L + 1  
                XZs_UT_covariation =  XZs_UT_covariation + ...
                                        Wc(k) * ...
                                        (Xsigma_UT(:,k) - Xs_1_p_weight) * ...
                                        (Zsigma_UT(:,k) - Zs_UT_weight)';
            end
            

    % 7 ���㿨��������
        K = XZs_UT_covariation * Zs_UT_covariation^-1;
            

    % 8 ����״̬��Э����
        % 8.1 ״̬����
            % Xukf(:,t)=Xs_1_p_weight+K*(Z(:,t)-Zs_UT_weight);
            Xukf(t) = Xs_1_p_weight + K * (accelerate(t) - Zs_UT_weight);
        % 8.2 �������
            covariation_finally = Xs_1_p_covariation - K * Zs_UT_covariation * K';


%
end


%%%%%%%%%%%%%%%%%%
% �Ӻ���


function x1 = my_gfun(x0, a1, T)
    % ״̬ת��
    % a1
    x1(1,:) = x0(1,:);
    % v1
    x1(2,:) = x0(2,:) + (x0(1,:) + a1(:)') ./2 .*T;
    
end
    

function a0 = my_hfun(X, v0, T)
    % �۲�ӳ��
    a1 = X(1, :);
    v1 = X(2, :);
    a0 = (v1 - v0) *2 /T - a1;
end
