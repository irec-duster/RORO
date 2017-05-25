%% Main 
clear all; clc;

% Global varialbes
%global roro
global env
global log
% Create rocket class

roro = rocket(init_rocket());% creates class with the initial values
%%
motor_init( roro ); %loads rocket motor
%%
% Initilize Environmental variables 
%To Do: read to do doc


% optional argument: Elevation(m) Temperature(C)and Pressure(Pa)
env = environement(1400, 25, 85900, roro );


%%
% Phase: Accent
tend=30;
[t, state] = accent_calc(roro,tend);
%%
figure(1);
plot(t,state(:,3))
xlabel('Time(s)')
ylabel('Height (m)')

figure(2);
plot3(state(:,1),state(:,2),state(:,3))
xlabel('x(m)')
ylabel('y (m)')
zlabel('Height (m)')
axis([-500 500 -500 500 0 4000])

h_max=max(state(:,3))
%%
clog = clean_log(t); %extract_data ( state,t);

%%
figure(3)

plot(clog(:,3),clog(:,2))

xlabel('Time')
ylabel('Value1, Value2')
axis([0 20 0.25 1])
% 
figure(4)
plot(clog(:,3),clog(:,1))
xlabel('Time')
ylabel('Value1, Value2')
%axis([0 25 0 01])
% 
% figure(5);
% plot(state(:,2),state(:,3))
% xlabel('y (m)')
% ylabel('Height (m)')
% 
% figure(6);
% plot(state(:,1),state(:,3))
% xlabel('x (m)')
% ylabel('Height (m)')
