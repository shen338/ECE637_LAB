clc
clear

img_race = imread('race.tif');
figure(1)
hist(img_race(:),0:255);

img_kids = imread('kids.tif');
figure(2)
hist(img_kids(:),0:255);

hist_before = hist(img_kids(:),0:255);

y = equalize(img_kids);
figure(3)
imshow(y)

figure(4)
hist(y(:),0:255);


sum_h = sum(hist_before);
F_hist_before = zeros(256);
for i = 1:256
       temp = 0;
       for j = 1:i
           temp = temp + hist_before(j);
           F_hist_before(i) = temp/sum_h;
       end
end
figure(5)
t = 0:255;
plot(t,F_hist_before);

figure(6)
output = stretch(img_kids,70,181);
imshow(output)

figure(7)
hist(output(:),0:255);

imshow(uint8(output))
g = 183;
figure(8)
checkboard = checkerboard(g);
imshow(uint8(checkboard))

x = log(0.5)/log(g/255);  %r

img_l= im2double(imread('linear.tif'));

linear = 255*(img_l).^(1/x);
figure(9)
imshow(uint8(linear))

figure(10)
img_g = im2double(imread('gamma15.tif'));

linear1 = 255*(img_g).^(1.5*1/x);
imshow(uint8(linear1))




