function Xkf=KF(Xkf,u,Z,H,P0,Q,R,phi,Tau,N)
    %% ������ɢkalman�˲�
    for i=2:N
        Xn=phi*Xkf(:,i-1)+Tau*u;%Ԥ��
        P1=phi*P0*phi'+Q;%Ԥ�����Э����
        K=P1*H'*(H*P1*H'+R)^(-1);%����
        Xkf(:,i)=Xn+K*(Z(:,i)-H*Xn);%״̬����
        P0=(eye(2)-K*H)*P1;             %�˲����Э�������
    end
end