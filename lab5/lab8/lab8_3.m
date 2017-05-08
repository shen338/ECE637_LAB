clc
clear
I0 = imread('house.tif');
I0 = double(I0);
I = (I0/255).^(2.2)*255;
[r,c] = size(I);


for i = 2:r-1
    for j = 2:c-1
        if I(i,j) > 127
            diff = - 255 + I(i,j);
            I(i,j) = 255;
        else
            diff = I(i,j);
            I(i,j) = 0;
        end
        I(i+1,j) = 5/16*diff + I(i+1,j);
        I(i,j+1) = 7/16*diff + I(i,j+1);
        I(i+1,j+1) = 1/16*diff + I(i+1,j+1);
        I(i+1,j-1) = 3/16*diff + I(i+1,j-1);
    end
end

imshow(I(2:r-1,2:c-1))
truesize
imwrite(I,'i.tif')

temp = 0;
for i = 1:r
    for j = 1:c
        temp = temp + (I0(i,j) - I(i,j))^2/(r*c);
    end
end
RMSE = sqrt(temp)
fidelity(I0,I)
        
           
        
            