load('data.mat')

figure(1)
t = 400:10:700;
plot(t,[x;y;z])
legend('x_0(\lambda)','y_0(\lambda)','z_0(\lambda)')


A_inv = [0.2430 0.8560 -0.0440
-0.3910 1.1650 0.0870
0.0100 -0.0080 0.5630];
figure(2)
plot(t,A_inv*[x;y;z])
legend('l_0(\lambda)','m_0(\lambda)','s_0(\lambda)')


figure(3)
plot(t,[illum1;illum2])
legend('D_{65}','Fluorescence')

figure(4)
total = x + y + z;
plot(x./total,y./total)
hold on
CIE_1931 = [0.16658 0.00886 0.82456
    0.73467 0.26533 0.0
    0.27376 0.71741 0.00883
    0.16658 0.00886 0.82456];

RGB_709 = [0.15 0.06 0.79
           0.64 0.33 0.03
          0.3 0.6 0.1
          0.15 0.06 0.79];
      
plot(CIE_1931(:,1),CIE_1931(:,2),'r-')
text(CIE_1931(:,1),CIE_1931(:,2),'CIE_{1931}')

plot(RGB_709(:,1),RGB_709(:,2),'g-')
text(RGB_709(:,1),RGB_709(:,2),'RGB')

D_65 = [0.3127, 0.3290, 0.3583];
EE = [0.3333, 0.3333, 0.3333];

plot(D_65(1),D_65(2),'r*')
text(D_65(1),D_65(2),'D_{65}')

plot(EE(1),EE(2),'g*')
text(EE(1),EE(2),'EE')

orient tall
hold off
print('Chromaticity_diagram.tif')
%%%%%%%%%%%%%%%%%section 4
load('data.mat')
load('reflect.mat')
[sizeX sizeY sizeZ] = size(R);
I_1 = zeros([sizeX sizeY sizeZ]);
I_2 = zeros([sizeX sizeY sizeZ]);
for i = 1:sizeX
    for j = 1:sizeY
        for p = 1:sizeZ
        I_1(i,j,p) = R(i,j,p)*illum1(p);
        I_2(i,j,p) = R(i,j,p)*illum2(p);
        end
    end
end
XYZ_1 = zeros([sizeX sizeY 3]);
XYZ_2 = zeros([sizeX sizeY 3]);
for i = 1:sizeX
    for j = 1:sizeY
        XYZ_1(i,j,:) = permute(I_1(i,j,:),[2 3 1])*[x;y;z]';
        XYZ_2(i,j,:) = permute(I_2(i,j,:),[2 3 1])*[x;y;z]';
    end
end

RGB_709 = [0.64 0.33 0.03
          0.3 0.6 0.1
          0.15 0.06 0.79];
RGB_709 = RGB_709';
D_65_wp = [0.3127, 0.3290, 0.3583];
Wp = D_65_wp/D_65_wp(2);
k = inv(RGB_709)*Wp';
M = RGB_709*diag(k)

RGB_image_1 = zeros([sizeX sizeY 3]);
RGB_image_2 = zeros([sizeX sizeY 3]);
for i = 1:sizeX
    for j = 1:sizeY
        RGB_image_1(i,j,:) = inv(M) * permute(XYZ_1(i,j,:),[3 2 1]);  
        RGB_image_2(i,j,:) = inv(M) * permute(XYZ_2(i,j,:),[3 2 1]);  
    end
end

RGB_image_1(RGB_image_1 < 0) = 0;
RGB_image_1(RGB_image_1 > 1) = 1;
RGB_image_2(RGB_image_2 < 0) = 0;
RGB_image_2(RGB_image_2 > 1) = 1;

RGB_gamma1 = uint8(255*RGB_image_1.^(1/2.2));
RGB_gamma2 = uint8(255*RGB_image_2.^(1/2.2));
figure(5)
image(RGB_gamma1)
imwrite(RGB_gamma1,'illum1.tif')
figure(6)
image(RGB_gamma2)


%%%%%%%%%%%%%section 5

clc
clear
[x_c y_c] = meshgrid(0:0.005:1);
z = 1 - x_c - y_c;

RGB_709 = [0.64 0.33 0.03
          0.3 0.6 0.1
          0.15 0.06 0.79];
RGB_709 = RGB_709';

M = RGB_709;

[sizeX sizeY] = size(x_c);
XYZ = zeros(sizeX,sizeY,3);
XYZ(:,:,1) = x_c;
XYZ(:,:,2) = y_c;
XYZ(:,:,3) = z;

RGB_image = zeros(sizeX,sizeY,3);
for i = 1:sizeX
    for j = 1:sizeY
        RGB_image(i,j,:) = inv(M)*permute(XYZ(i,j,:),[3 2 1]);
        if min(RGB_image(i,j,:)) < 0
            RGB_image(i,j,:) = ones(3,1);
        end
    end
end


RGB_image(RGB_image < 0) = 1;

RGB_gamma = uint8(255*RGB_image.^(1/2.2));
figure(7)
image([0:0.005:1],[0:0.005:1],RGB_gamma)
axis('xy')
hold on

load('data.mat')
total = x + y + z;

plot(x./total,y./total)




