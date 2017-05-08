I1 = imread('img03y.tif');
I = double(I1);
I = I - 128;

gamma = 4;
fn = @(x) round(dct2(x.data,[8,8])./(Quant*gamma));
dct_blk = blockproc(I,[8,8],fn);
dct_blk = dct_blk';
[r,c] = size(dct_blk);
fileID = fopen('img03y.dq','w');
fwrite(fileID,r,'integer*2');
fwrite(fileID,c,'integer*2');
fwrite(fileID,dct_blk,'integer*2');
fclose(fileID);

image_1 = readfile('img03y.dq',gamma);

imshow(image_1')
image_different = image_1' - I1;
figure(2)
imshow(uint8(10*image_different+128))

