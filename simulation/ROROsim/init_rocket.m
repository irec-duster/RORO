function [ prop ] = init_rocket(  )

prop = readtable('rocket_prop.txt','Format','%s%f');
prop=table2array(prop(:,2));
end

