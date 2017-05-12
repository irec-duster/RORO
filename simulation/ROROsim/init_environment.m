function [ env ] = init_environment(  )

env = readtable('environment.txt','Format','%s%f');
end

