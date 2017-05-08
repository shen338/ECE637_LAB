
clc
clear

[img] = imread('img04g.tif');

%%%%%%%%% block size 64
X = double(img)/255;

i=100;
j=100;

N = 64;
z = X(i:(N+i-1), j:(N+j-1));

Z = (1/N^2)*abs(fft2(z)).^2;

Z = fftshift(Z);

Z_64 = log( Z );

x = 2*pi*((0:(N-1)) - N/2)/N;
y = 2*pi*((0:(N-1)) - N/2)/N;
figure(1)
surf(x,y,Z_64)
xlabel('\mu axis')
ylabel('\nu axis')
print(1,'-dpng','block64.png')

%%%%%%%%% block size 128

X = double(img)/255;

i=100;
j=100;

N = 128;
z = X(i:(N+i-1), j:(N+j-1));

Z = (1/N^2)*abs(fft2(z)).^2;

Z = fftshift(Z);

Z_128 = log( Z );

x = 2*pi*((0:(N-1)) - N/2)/N;
y = 2*pi*((0:(N-1)) - N/2)/N;
figure(2) 
surf(x,y,Z_128)
xlabel('\mu axis')
ylabel('\nu axis')
print(2,'-dpng','block128.png')

%%%%%%%%%%%%%% block size 256 

X = double(img)/255;

i=100;
j=100;

N = 256;
z = X(i:(N+i-1), j:(N+j-1));

Z = (1/N^2)*abs(fft2(z)).^2;

Z = fftshift(Z);

Z_256 = log( Z );

x = 2*pi*((0:(N-1)) - N/2)/N;
y = 2*pi*((0:(N-1)) - N/2)/N;
figure(3) 
surf(x,y,Z_256)
xlabel('\mu axis')
ylabel('\nu axis')
print(3,'-dpng','block256.png')

%%%%%%% Improved Power Spectral Density 

N = 64;
Improved_PSD = BetterSpecAnal(img);
x = 2*pi*((0:(N-1)) - N/2)/N;
y = 2*pi*((0:(N-1)) - N/2)/N;
figure(4) 
surf(x,y,Improved_PSD)
xlabel('\mu axis')
ylabel('\nu axis')
print(4,'-dpng','Improved_PSD.png')

%%%%%%% random img

A = rand(512) - 0.5;

img = (A+0.5)*255;
figure(5)
map=gray(256);
colormap(gray(256));
image(img)
axis('image')
print(5,'-dpng','Random_img.png')

output_img = zeros(512,512);

for i = 1:512
    for j = 1:512
        temp = 0;
        temp = temp + 3*A(i,j);
        if(i>2)
            temp = temp + 0.99*output_img(i-1,j);
        end
        if(j>2)
            temp = temp + 0.99*output_img(i,j-1);
        end
        if(i>2&&j>2)
            temp = temp - 0.9801*output_img(i-1,j-1);
        end
        output_img(i,j) = temp;
    end
end


fig = figure(6);
map = gray(256);
colormap(fig,map);
image(uint8(output_img+127))
axis('image')
print(6,'-dpng','filtered_img.png')
N = 64;

u = -pi:0.1:pi;
v = -pi:0.1:pi;
[U,V] = meshgrid(u,v);
sigma = 1/12;
PSD_t = sigma*abs(3./((1-0.99*exp(-1i*U)).*(1-0.99*exp(-1i*V)))).^2;
figure(7)
surf(U,V,log(PSD_t));
xlabel('\mu');
ylabel('\nu');
print(7,'-dpng','theoryPSD.png')

result = BetterSpecAnal(output_img);
x = 2*pi*((0:(N-1)) - N/2)/N;
y = 2*pi*((0:(N-1)) - N/2)/N;
figure(8)
surf(x,y,result)
xlabel('\mu axis')
ylabel('\nu axis')
print(8,'-dpng','estimatedPSD.png')

