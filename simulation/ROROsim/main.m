%% Main 
clear all; clc;

% Global varialbes
global roro
global env
global log

% Create rocket class
roro = rocket(init_rocket());% creates class with the initial values
init_motor( roro ); %loads rocket motor

% Initilize Environmental variables 
%To Do: read to do doc

% optional argument: Elevation(m) Temperature(C)and Pressure(Pa)
env = environement(1400, 35, 85900 );



%% --- Figures Flight Info ---
% Phase: ascent
tend=30;
[t, state] = ascent_calc(roro,tend);

clog = clean_log(t);
h_max=max(state(:,3))
Xcp_ExtendedBarrowman = mean(log(:,1))
Xcp_Barrowman = mean(log(:,2))

% figure('Name','Flight 3D','Position', [800 0 400 1000])
% plot3(state(:,1),state(:,2),state(:,3))
% xlabel('x(m)')
% ylabel('y (m)')
% zlabel('Height (m)')
% title('Height 3D')
% axis([-700 0 -200 200 0 3500])

figure('Name','Flight information','Position', [400 0 400 1000]);
subplot(4,1,1)
plot(t,state(:,3))
xlabel('Time(s)')
ylabel('Height (m)')
title('Height 1D')

subplot(4,1,2)
plot(clog(:,10),clog(:,7))
xlabel('Time')
ylabel('Xdot (m/s)')
title('Velocity')
axis([0 25 0 300])

subplot(4,1,3)
plot(clog(:,10),clog(:,8))
xlabel('Time')
ylabel('Mass [kg]')
title('Mass')
axis([0 25 0 30])

subplot(4,1,4)
plot(clog(:,10),clog(:,9))
xlabel('Time')
ylabel('alpha [rad]')
title('Angle of Attack')
axis([0 25 0 0.2])

%% --- Figures Stability ---
figure('Name','Stability Analysis', 'Position', [0 0 400 1000])
subplot(4,1,1)
hold on
plot(clog(:,10),clog(:,1))
plot(clog(:,10),clog(:,2))
hold off
xlabel('Time')
ylabel('Xcp [m]')
title('Center of Pressure')
axis([0 25 1 2])
% 
subplot(4,1,2)
plot(clog(:,10),clog(:,3))
xlabel('Time')
ylabel('Xcm [m]')
title('Center of Mass')
axis([0 25 1 2])
%
subplot(4,1,3)
hold on
plot(clog(:,10),clog(:,4))
plot(clog(:,10),clog(:,5))
xlabel('Time')
ylabel('Margin []')
title('Static Stability Margin')
axis([0 25 1 3.5])
%
subplot(4,1,4)
plot(clog(:,10),clog(:,6))
xlabel('Time')
ylabel('Zeta []')
title('Damping Ratio')
axis([0 25 0.05 0.10])

