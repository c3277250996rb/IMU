function [Xekf1, Z] = EKF(Xekf1, Z);

    N = public_N;



    % 
        N = 50;
        Q = 10;w=sqrt(Q)*randn(1,N);
        R = 1;v=sqrt(R)*randn(1,N);
        P =eye(1);
        x=zeros(1,N);
        delta_x=zeros(1,N);
        s=zeros(1,N);
        Xekf=zeros(1,N);
        Xekf1=zeros(1,N);
        x(1,1)=0.1;
        delta_x(1,1)=0;
        s(1,1)=0.1;
        Xekf(1,1)=x(1,1);
        Xekf1(1,1)=x(1,1);
        z=zeros(1,N);
        delta_z=zeros(1,N);
        z(1)=x(1,1)^2/20+v(1);
        delta_z(1)=x(1,1)/10+v(1);
        zpre=zeros(1,N);
        zpre(1,1)=z(1);
    %


    %按标称状态线性化的卡尔曼滤波
    for k=2:N
        %状态预测(不变)
        % Xpre = 0.5*Xekf1(:,k-1)+ 2.5*Xekf1(:,k-1)/(1+Xekf1(:,k-1).^2) + 8 * cos(1.2*(k-1));%f
        Xpre = my_gfun(Xekf1, Z, T);
        
        F = 0.5 + 2.5 * (1-Xekf1.^2)/((1+Xekf1.^2).^2);%f_n
        % F = s(:)
        H = Xpre/10;
        
        %均方差预测方程
        PP=F*P*F'+Q;
        
        %卡尔曼增益（不变）
        Kk=PP*H'*(H*PP*H'+R)^(-1);
        
        
        delta_x(:,k)=F*delta_x(:,k-1)+Kk*(delta_z(:,k)-H*delta_x(:,k-1));
        
        %状态更新
        Xekf1(k)=Xpre+delta_x(k);
        
        %均方差更新
        P=PP-Kk*H*PP;
    end

end