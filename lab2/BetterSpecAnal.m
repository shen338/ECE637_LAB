function y = BetterSpecAnal(img)
   N = 64;
   windows = zeros(64,64,25);
   center = size(img)/2;
   t = 1;
   for i = -2:1:2
     for j = -2:1:2
        windows(:,:,t) = img(center + (-0.5+i)*N:center + (0.5+i)*N - 1,center + (-0.5+j)*N:center + (0.5+j)*N - 1);
        t = t + 1;
     end
   end
   w = hamming(64)*hamming(64)';
   spec = zeros(64,64,25);
   for i = 1:25
      hamminged  = windows(:,:,i).*w;
      Z = (1/N^2)*abs(fft2(hamminged)).^2;
      Z = fftshift(Z);
      spec(:,:,i) = log(Z);
   end
   y = mean(spec,3);
end

