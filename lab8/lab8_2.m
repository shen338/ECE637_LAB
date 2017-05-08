
clear
imager = imread('house.tif');
imager = double(imager);
[r,c] = size(imager);

%%%%%% Un-gamma %%%%
image_g = 255*(imager/255).^(2.2);
imshow(uint8(image_g))

Bayer_2 = [1 2;3 0]
Bayer_4 = [Bayer_2*4 + 1 Bayer_2*4 + 2;Bayer_2*4 + 3 Bayer_2*4]
Bayer_8 = [Bayer_4*4 + 1 Bayer_4*4 + 2;Bayer_4*4 + 3 Bayer_4*4]

filter_2 = 255*(Bayer_2+0.5)/4;
filter_4 = 255*(Bayer_4+0.5)/16;
filter_8 = 255*(Bayer_8+0.5)/64;

image_b2 = zeros(r,c);
image_b4 = zeros(r,c);
image_b8 = zeros(r,c);

for i = 1:r
    for j = 1:c
        if image_g(i,j) > filter_2(rem(i-1,2)+1,rem(j-1,2)+1)
            image_b2(i,j) = 255;
        end
        if image_g(i,j) > filter_4(rem(i-1,4)+1,rem(j-1,4)+1)
            image_b4(i,j) = 255;
        end
        if image_g(i,j) > filter_8(rem(i-1,8)+1,rem(j-1,8)+1)
            image_b8(i,j) = 255;
        end
    end
end

figure(2)
imshow(image_b2);
truesize;

figure(3)
imshow(image_b4);
truesize;

figure(4)
imshow(image_b8);
truesize;

temp1 = 0;
temp2 = 0;
temp3 = 0;
for i = 1:r
    for j = 1:c
        temp1 = temp1 + (imager(i,j) - image_b2(i,j))^2/(r*c);
        temp2 = temp2 + (imager(i,j) - image_b4(i,j))^2/(r*c);
        temp3 = temp3 + (imager(i,j) - image_b8(i,j))^2/(r*c);
    end
end
sqrt(temp1)
fidelity(imager,image_b2)
sqrt(temp2)
fidelity(imager,image_b4)
sqrt(temp3)
fidelity(imager,image_b8)


        
        