function [ data ] = cleanData( data )

     t = data(:,1);
     dt = t(2:end)- t(1:end-1);
     dt= [0;dt];
     indDel = find(dt(2:end)==0)+1;

     data(indDel,:) = [];
     
end

