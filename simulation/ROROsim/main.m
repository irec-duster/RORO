%% Main 
clear all; clc;

%% Global varialbes
global roro 
global env 
%% Create rocket class

roro = rocket(init_rocket());% creates class with the initial values
%% Initilize Environmental variables 
%To Do: read to do doc

env = environement();

% Phase: Accent Rail

% Phase: Accent
accent_calc()
