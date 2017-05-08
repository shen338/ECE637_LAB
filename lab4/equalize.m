function Z = equalize(x)
   histogram = hist(x(:),0:255);
   sum_h = sum(histogram);
   F_hist = zeros(256);
   for i = 1:256
       temp = 0;
       for j = 1:i
           temp = temp + histogram(j);
           F_hist(i) = temp/sum_h;
       end
   end
   Y_max = F_hist(max(x(:)));
   Y_min = F_hist(min(x(:)));
   Z = uint8(255*(F_hist(x) - Y_min)/(Y_max - Y_min));
end  
