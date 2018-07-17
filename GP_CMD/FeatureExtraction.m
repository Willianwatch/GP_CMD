function [feature] = FeatureExtraction( img1,img2,d)
%UNTITLED 此处显示有关此函数的摘要
%   此处显示详细说明
jzyt=img1;
% if (sum(sum(jzyt))==0)
%     jzyt=jzyt+0.1;
% end
% jzyt=double(jzyt);
[m,n]=size(img1);
[u,v]=find(img2~=0);                
[up,index]=sort(u);                
[uh,ul]=size(u);                
                
%******计算灰度共生矩阵*******%                
glcm0=zeros(256,256);               
glcm45=zeros(256,256);                
glcm90=zeros(256,256);               
glcm135=zeros(256,256);                
for i=1:uh                    
    for j=1:uh                       
        if up(j)-up(i)==0&&v(index(j))-v(index(i))==d                            
            glcm0(jzyt(up(i),v(index(i)))+1,jzyt(up(j),v(index(j)))+1)=glcm0(jzyt(up(i),v(index(i)))+1,jzyt(up(j),v(index(j)))+1)+1;                        
        end        
        if up(i)-up(j)==d&&v(index(j))-v(index(i))==d                            
            glcm45(jzyt(up(i),v(index(i)))+1,jzyt(up(j),v(index(j)))+1)= glcm45(jzyt(up(i),v(index(i)))+1,jzyt(up(j),v(index(j)))+1)+1;                        
        end        
        if up(i)-up(j)==d&&v(index(j))-v(index(i))==0                            
            glcm90(jzyt(up(i),v(index(i)))+1,jzyt(up(j),v(index(j)))+1)=glcm90(jzyt(up(i),v(index(i)))+1,jzyt(up(j),v(index(j)))+1)+1;
        end
        if up(i)-up(j)==d&&v(index(i))-v(index(j))==d                         
            glcm135(jzyt(up(i),v(index(i)))+1,jzyt(up(j),v(index(j)))+1)=glcm135(jzyt(up(i),v(index(i)))+1,jzyt(up(j),v(index(j)))+1)+1;                        
        end        
    end
end

glcm0=glcm0./sum(glcm0(:));                
glcm45=glcm45./sum(glcm45(:));                
glcm90=glcm90./sum(glcm90(:));                
glcm135=glcm135./sum(glcm135(:));

con0=0; 
con45=0;               
con90=0;                
con135=0;
for k=0:255                   
    for i=1:256                       
        for j=1:256
            if abs(i-j)==k                                                              
                con0=k^2*glcm0(i,j)+con0;
                con45=k^2*glcm45(i,j)+con45;                               
                con90=k^2*glcm90(i,j)+con90;
                con135=k^2*glcm135(i,j)+con135;                          
            end
        end        
    end
end

doe0=0;                
doe45=0;               
doe90=0;
doe135=0;                
doe0_s=0;                
doe45_s=0;                
doe90_s=0;                
doe135_s=0;                
for k=0:255
    for i=1:256
        for j=1:256                            
            if abs(i-j)==k                                
                doe0_s=glcm0(i,j)+doe0_s;
                doe45_s=glcm45(i,j)+doe45_s;
                doe90_s=glcm90(i,j)+doe90_s;
                doe135_s=glcm135(i,j)+doe135_s;                            
            end
        end
    end
    if doe0_s>0                        
        doe0=-doe0_s*log(doe0_s)+doe0;                    
    end    
    if doe45_s>0                        
        doe45=-doe45_s*log(doe45_s)+doe45;                    
    end
    if doe90_s>0                        
        doe90=-doe90_s*log(doe90_s)+doe90;
    end    
    if doe135_s>0                        
        doe135=-doe135_s*log(doe135_s)+doe135;                    
    end    
end

imc1_0=0;                
imc1_45=0;                
imc1_90=0;              
imc1_135=0;               
E_0=0;             
E_45=0;              
E_90=0;              
E_135=0;           
Ex_0=0;               
Ex_45=0;               
Ex_90=0;              
Ex_135=0;             
Ey_0=0;             
Ey_45=0;           
Ey_90=0;           
Ey_135=0;           
for i=1:256                 
    for j=1:256                  
        if sum(glcm0(i,:))*sum(glcm0(:,j))>0                                          
            E_0=-glcm0(i,j)*log(sum(glcm0(i,:))*sum(glcm0(:,j)))+E_0;                     
        end        
        if sum(glcm45(i,:))*sum(glcm45(:,j))>0
            E_45=-glcm45(i,j)*log(sum(glcm45(i,:))*sum(glcm45(:,j)))+E_45;
        end
        if sum(glcm90(i,:))*sum(glcm90(:,j))>0                       
            E_90=-glcm90(i,j)*log(sum(glcm90(i,:))*sum(glcm90(:,j)))+E_90;                     
        end
        if sum(glcm135(i,:))*sum(glcm135(:,j))>0
            E_135=-glcm135(i,j)*log(sum(glcm135(i,:))*sum(glcm135(:,j)))+E_135;                       
        end
        if sum(glcm0(i,:))>0
            Ex_0=-sum(glcm0(i,:))*log(sum(glcm0(i,:)))+Ex_0;                       
        end      
        if sum(glcm45(i,:))>0                         
            Ex_45=-sum(glcm45(i,:))*log(sum(glcm45(i,:)))+Ex_45;
        end        
        if sum(glcm90(i,:))>0
            Ex_90=-sum(glcm90(i,:))*log(sum(glcm90(i,:)))+Ex_90;                       
        end
        if sum(glcm135(i,:))>0
            Ex_135=-sum(glcm135(i,:))*log(sum(glcm135(i,:)))+Ex_135;                       
        end        
        if sum(glcm0(:,j))>0                           
            Ey_0=-sum(glcm0(:,j))*log(sum(glcm0(:,j)))+Ex_0;                     
        end        
        if sum(glcm45(:,j))>0
            Ey_45=-sum(glcm45(:,j))*log(sum(glcm45(:,j)))+Ex_45;
        end
        if sum(glcm90(:,j))>0                          
            Ey_90=-sum(glcm90(:,j))*log(sum(glcm90(:,j)))+Ex_90;                       
        end        
        if sum(glcm135(:,j))>0
            Ey_135=-sum(glcm135(:,j))*log(sum(glcm135(:,j)))+Ex_135;
        end        
    end
end
ent0=0;                 
ent45=0;               
ent90=0;              
ent135=0;               
for i=1:256
    for j=1:256
       if(glcm0(i,j)>0)
            ent0=-glcm0(i,j)*log(glcm0(i,j))+ent0;                       
       end       
       if(glcm45(i,j)>0)                           
           ent45=-glcm45(i,j)*log(glcm45(i,j))+ent45;                       
       end       
       if(glcm90(i,j)>0)                        
           ent90=-glcm90(i,j)*log(glcm90(i,j))+ent90;                       
       end       
       if(glcm135(i,j)>0)                          
           ent135=-glcm135(i,j)*log(glcm135(i,j))+ent135;
       end
    end
end
if max(Ex_0,Ey_0)~=0
    imc1_0=(ent0-E_0)/max(Ex_0,Ey_0);
end
if max(Ex_45,Ey_45)~=0
    imc1_45=(ent45-E_45)/max(Ex_45,Ey_45);  
end
if max(Ex_90,Ey_90)~=0
    imc1_90=(ent90-E_90)/max(Ex_90,Ey_90);
end
if max(Ex_135,Ey_135)~=0
    imc1_135=(ent135-E_135)/max(Ex_135,Ey_135);  
end

vod0=0;                
vod45=0;                
vod90=0;                
vod135=0;               
w0=0;                
w45=0;
w90=0;               
w135=0;              
for k=0:255                   
    for i=1:256
        for j=1:256
            if abs(i-j)==k
                w0=k*glcm0(i,j)+w0;                
                w45=k*glcm45(i,j)+w45;                               
                w90=k*glcm90(i,j)+w90;                            
                w135=k*glcm135(i,j)+w135;                           
            end
        end
    end
end
for k=0:255                  
    for i=1:256
        for j=1:256                          
            if i-j==k                           
                vod0=(k-w0)^2*glcm0(i,j)+vod0;
                vod45=(k-w45)^2*glcm45(i,j)+vod45;                               
                vod90=(k-w90)^2*glcm90(i,j)+vod90;                                
                vod135=(k-w135)^2*glcm135(i,j)+vod135;  
            end
        end         
    end     
end


soe0=0;
soe45=0;                
soe90=0;
soe135=0;
soe0_s=0;
soe45_s=0;               
soe90_s=0;                
soe135_s=0;              
for k=2:512
    for i=1:256
        for j=1:256
            if i+j==k
                soe0_s=glcm0(i,j)+soe0_s;                                
                soe45_s=glcm45(i,j)+soe45_s;
                soe90_s=glcm90(i,j)+soe90_s;                                
                soe135_s=glcm135(i,j)+soe135_s;                            
            end            
        end
    end
    if soe0_s>0                        
        soe0=-soe0_s*log(soe0_s)+soe0;                    
    end    
    if soe45_s>0                       
        soe45=-soe45_s*log(soe45_s)+soe45;                    
    end    
    if soe90_s>0                        
        soe90=-soe90_s*log(soe90_s)+soe90;                    
    end
    if soe135_s>0                        
        soe135=-soe135_s*log(soe135_s)+soe135;                    
    end
end

soa0=0;                
soa45=0;                
soa90=0;               
soa135=0;                
for k=2:512                    
    for i=1:256
        for j=1:256                            
            if i+j==k
                soa0=k*glcm0(i,j)+soa0;                               
                soa45=k*glcm45(i,j)+soa45;                                               
                soa90=k*glcm90(i,j)+soa90;                               
                soa135=k*glcm135(i,j)+soa135;                           
            end
        end
    end
end

var0=0;
var45=0;
var90=0;                
var135=0;                
var0_m=mean(glcm0(:));
var45_m=mean(glcm45(:));                
var90_m=mean(glcm90(:));
var135_m=mean(glcm135(:));                
for i=1:256                    
    for j=1:256                        
        var0=(i-var0_m)^2*glcm0(i,j)+var0;                        
        var45=(i-var45_m)^2*glcm45(i,j)+var45;                        
        var90=(i-var90_m)^2*glcm90(i,j)+var90;                        
        var135=(i-var135_m)^2*glcm135(i,j)+var135;                   
    end    
end

imc2_0=0;             
imc2_45=0;              
imc2_90=0;               
imc2_135=0;              
E2_0=0;             
E2_45=0;              
E2_90=0;              
E2_135=0;
for i=1:256                  
    for j=1:256                       
        if sum(glcm0(i,:))*sum(glcm0(:,j))>0                           
            E2_0=-sum(glcm0(i,:))*sum(glcm0(:,j))*log(sum(glcm0(i,:))*sum(glcm0(:,j)))+E_0;                      
        end
        if sum(glcm45(i,:))*sum(glcm45(:,j))>0
            E2_45=-sum(glcm45(i,:))*sum(glcm45(:,j))*log(sum(glcm45(i,:))*sum(glcm45(:,j)))+E_45;
        end
        if sum(glcm90(i,:))*sum(glcm90(:,j))>0                          
            E2_90=-sum(glcm90(i,:))*sum(glcm90(:,j))*log(sum(glcm90(i,:))*sum(glcm90(:,j)))+E_90;
        end
        if sum(glcm135(i,:))*sum(glcm135(:,j))>0
            E2_135=-sum(glcm135(i,:))*sum(glcm135(:,j))*log(sum(glcm135(i,:))*sum(glcm135(:,j)))+E_135;                       
        end        
    end    
end
imc2_0=sqrt(1-exp(-2*(E2_0-ent0)));                
imc2_45=sqrt(1-exp(-2*(E2_45-ent45)));
imc2_90=sqrt(1-exp(-2*(E2_90-ent90)));                
imc2_135=sqrt(1-exp(-2*(E2_135-ent135)));                
                 
idm0=0;                 
idm45=0;
idm90=0;                
idm135=0;              
for i=1:256
    for j=1:256
        idm0=glcm0(i,j)/(1+(i-j)^2)+idm0;                        
        idm45=glcm45(i,j)/(1+(i-j)^2)+idm45;                        
        idm90=glcm90(i,j)/(1+(i-j)^2)+idm90;                        
        idm135=glcm135(i,j)/(1+(i-j)^2)+idm135;
    end
end

CON=[con0,con45,con90,con135];
DOE=[doe0,doe45,doe90,doe135];
IMC1=[imc1_0,imc1_45,imc1_90,imc1_135];
VOD=[vod0,vod45,vod90,vod135];
SOE=[soe0,soe45,soe90,soe135];
SOA=[soa0,soa45,soa90,soa135];
VAR=[var0,var45,var90,var135];
IMC2=[imc2_0,imc2_45,imc2_90,imc2_135];
IDM=[idm0,idm45,idm90,idm135];
feature=[CON,DOE,IMC1,VOD,SOE,SOA,VAR,IMC2,IDM];
%feature=[CON,VOD,SOE,SOA,VAR];
end