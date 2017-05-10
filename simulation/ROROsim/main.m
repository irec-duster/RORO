%% Main 
clear all; clc;

% Global varialbes
global roro
global env 
% Create rocket class

roro = rocket(init_rocket());% creates class with the initial values

motor_init( roro ); %loads rocket motor

% Initilize Environmental variables 
%To Do: read to do doc

env = environement();


%%
% Phase: Accent
accent_calc()


