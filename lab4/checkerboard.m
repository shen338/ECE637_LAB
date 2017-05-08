function output = checkerboard(g)
output = zeros(256,256);
strip = zeros(16,256);
strip_ref = g*ones(16,256);
A = [255 255 0 0
    255 255 0 0
    0 0 255 255
    0 0 255 255];
for i = 0:3
    for j = 0:63
        strip(4*i+1:4*(i+1),4*j+1:4*(j+1)) = A;
    end
end
temp = [strip;strip_ref];
for i = 0:7
    output(32*i+1:32*(i+1),:) = temp;
end
end
    
        
        