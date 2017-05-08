function output = stretch(img,T1,T2)
   [height,width] = size(img);
   img = double(img);
   output = zeros(height,width);
   for i = 1:height
       for j = 1:width
           if(img(i,j) < T2 && img(i,j) > T1)
               output(i,j) = 255*(img(i,j) - T1)/(T2 - T1);

           end
       end
   end
   output = uint8(output);
end