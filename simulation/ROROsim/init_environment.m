function [ env ] = init_environment(  )
% This function is not used the values are stored directly in the class
env = readtable('environment.txt','Format','%s%f');
end

