clc
clear
image = imread('house.tif');
figure(1)
imshow(image)
[r, c] = size(image);
image_T = zeros(r,c);
for i = 1:r
    for j = 1:c
        if image(i,j) > 127
            image_T(i,j) = 255;
        end
    end
end
figure(2)
imshow(image_T)


image = double(image);
image_T = double(image_T);
temp = 0;

for i = 1:r
    for j = 1:c
        temp = temp + (image(i,j) - image_T(i,j))^2/(r*c);
    end
end

RMSE = sqrt(temp)

Fidelity = fidelity(image,image_T)




