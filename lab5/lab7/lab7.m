format short
X = imread('img14sp.tif');

X = double(X);

Y = imread('img14g.tif');

Y = double(Y);


[sizeX sizeY] = size(X);
numR = floor((sizeY - 3*2)/20);
numC = floor((sizeX - 3*2)/20); 

Z = zeros(numR*numC,49);
Y1 = zeros(numR*numC,1);
for i = 1:numC
    for j = 1:numR
        Z((i-1)*numR + j,:) = reshape(X(i*20-3:i*20+3,j*20-3:j*20+3),[1 49]);
        Y1((i-1)*numR + j) = Y(i*20,j*20);
    end
end

R_zz = Z'*Z/numR/numC;
Y_zy = Z'*Y1/numR/numC;
theta = reshape(inv(R_zz)*Y_zy,[7 7])

imshow(uint8(X));
figure(2)
imshow(uint8(imfilter(X,theta)));





