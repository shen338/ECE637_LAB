I = imread('img03y.tif');
I = double(I);
I = I - 128;

gamma = 1;
fn = @(x) round(dct2(x.data,[8,8])./(Quant*gamma));
dct_blk = blockproc(I,[8,8],fn);

[r,c] = size(dct_blk);
DC = zeros(r/8,c/8);
AC = zeros(r*c/64,63);

for i = 0:r/8-1
    for j = 0:c/8-1
        DC(i+1,j+1) = dct_blk(i*8+1,j*8+1) + 128;
        temp = dct_blk(i*8+1:i*8+8,j*8+1:j*8+8);
        temp = temp(Zig);
        AC(i*c/8 + j + 1,:) = temp(2:end);
    end
end

DC = uint8(DC);
figure(1)
imshow(DC)
AC_mean = mean(abs(AC));
t = 1:63;
figure(2)
plot(t,AC_mean)
