function I3 = test_tontai(I)  
%I=double(rgb2gray(I));  
I = double(I);
[M,N]=size(I);  
rL=0.5;  
rH=4.7;%可根据需要效果调整参数  
c=2;  
d0=10;  
I1=log(I+1);%取对数  
FI=fft2(I1);%傅里叶变换  
n1=floor(M/2);  
n2=floor(N/2);  
for i=1:M  
    for j=1:N  
        D(i,j)=((i-n1).^2+(j-n2).^2);  
        H(i,j)=(rH-rL).*(exp(c*(-D(i,j)./(d0^2))))+rL;%高斯同态滤波  
    end  
end  
I2=ifft2(H.*FI);%傅里叶逆变换  
I3=real(exp(I2));  

