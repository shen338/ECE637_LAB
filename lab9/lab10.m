I = imread('img03y.tif');
I = double(I);
I = I - 128;

gamma = 1;
fn = @(x) round(dct2(x.data,[8,8])./Quant*gamma);
dct_blk = blockproc(I,[8,8],fn);
dct_blk = dct_blk';
fwrite(dct_blk,'img03y.pq');
