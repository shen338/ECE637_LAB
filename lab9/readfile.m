function [image] = readfile(file,gamma)
    run('Qtables');
    f = fopen(file,'r');
    data = fread(f,'integer*2');
    image = reshape(data(3:end),[data(1),data(2)]);
    fn = @(x) round(idct2(x.data.*Quant*gamma,[8,8]));
    image = blockproc(image,[8,8],fn);
    image = image + 128;
    image = uint8(image);
end