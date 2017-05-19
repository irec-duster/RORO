%% Main 
clear all; clc;

% Global varialbes
global roro
global env
global log
global t_RailExit
global v_RailExit
global t_Burnout

t_Burnout = 0;
% Create rocket class
roro = rocket(init_rocket());% creates class with the initial values
init_motor( roro ); %loads rocket motor

% Initilize Environmental variables 
%To Do: read to do doc

% optional argument: Elevation(m) Temperature(C)and Pressure(Pa)
env = environment(init_environment());



%% --- Run Simulation
% Phase: ascent
tend=30;
[t, state] = ascent_calc(roro,tend);

clog = clean_log(t);

%% --- Output Console Window
Xcp_ExtendedBarrowman = mean(log(:,1))
CnXcp = roro.CnXcp;
Xcp_Barrowman = CnXcp(3)
Xcp_Planform = CnXcp(4)
t_Burnout = t_Burnout
t_RailExit = t_RailExit
t_Burnout = t_Burnout

h_max=max(state(:,3))

%% --- Figure Flight Information
% figure('Name','Flight 3D','Position', [800 0 400 1000])
% plot3(state(:,1),state(:,2),state(:,3))
% xlabel('x(m)')
% ylabel('y (m)')
% zlabel('Height (m)')
% title('Height 3D')
% axis([-700 0 -200 200 0 3500])

figure('Name','Flight information','Position', [400 0 400 1000]);
subplot(4,1,1)
plot(clog(:,11),clog(:,7))
xlabel('Time')
ylabel('Xdot (m/s)')
title('Velocity')
axis([0 25 0 300])
line([t_RailExit t_RailExit],[0 300],'Color',[0 1 0])
line([t_Burnout t_Burnout],[0 300],'Color',[1 0 0])

subplot(4,1,2)
plot(clog(:,11),clog(:,8))
xlabel('Time')
ylabel('Mass [kg]')
title('Mass')
axis([0 25 15 30])
line([t_RailExit t_RailExit],[0 300],'Color',[0 1 0])
line([t_Burnout t_Burnout],[0 300],'Color',[1 0 0])

subplot(4,1,3)
plot(clog(:,11),clog(:,10))
xlabel('Time(s)')
ylabel('Cd []')
title('Drag Coefficient')
axis([0 25 0 1])
line([t_RailExit t_RailExit],[0 300],'Color',[0 1 0])
line([t_Burnout t_Burnout],[0 300],'Color',[1 0 0])

subplot(4,1,4)
plot(clog(:,11),clog(:,9))
xlabel('Time')
ylabel('alpha [rad]')
title('Angle of Attack')
axis([0 25 0 0.2])
line([t_RailExit t_RailExit],[0 300],'Color',[0 1 0])
line([t_Burnout t_Burnout],[0 300],'Color',[1 0 0])

%% --- Figures Stability Analysis
figure('Name','Stability Analysis', 'Position', [0 0 400 1000])
subplot(4,1,1)
hold on
plot(clog(:,11),clog(:,1))
plot(clog(:,11),clog(:,2))
hold off
xlabel('Time')
ylabel('Xcp [m]')
title('Center of Pressure')
axis([0 25 1.4 2])
line([t_RailExit t_RailExit],[0 300],'Color',[0 1 0])
line([t_Burnout t_Burnout],[0 300],'Color',[1 0 0])
legend('Barrowman+BodyLift','Barrowman','rail exit','burnout')

% 
subplot(4,1,2)
plot(clog(:,11),clog(:,3))
xlabel('Time')
ylabel('Xcm [m]')
title('Center of Mass')
axis([0 25 1.4 2])
line([t_RailExit t_RailExit],[0 300],'Color',[0 1 0])
line([t_Burnout t_Burnout],[0 300],'Color',[1 0 0])
%
subplot(4,1,3)
hold on
plot(clog(:,11),clog(:,4))
plot(clog(:,11),clog(:,5))
hold off
xlabel('Time')
ylabel('Margin []')
title('Static Stability Margin')
axis([0 25 1 3.5])
line([t_RailExit t_RailExit],[0 300],'Color',[0 1 0])
line([t_Burnout t_Burnout],[0 300],'Color',[1 0 0])
legend('Barrowman+BodyLift','Barrowman','rail exit','burnout')

%
subplot(4,1,4)
plot(clog(:,11),clog(:,6))
xlabel('Time')
ylabel('Zeta []')
title('Damping Ratio')
axis([0 25 0.05 0.09])
line([t_RailExit t_RailExit],[0 300],'Color',[0 1 0])
line([t_Burnout t_Burnout],[0 300],'Color',[1 0 0])

