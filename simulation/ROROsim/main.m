%% Main 
clear all; clc;

% Global varialbes
%global roro
global env
global log
global t_Burnout;

t_RailExit=0;
t_Burnout=0;
% Create rocket class

roro = rocket(init_rocket());% creates class with the initial values
%%
motor_init( roro ); %loads rocket motor
%%
% Initilize Environmental variables 
%To Do: read to do doc


% optional argument: Elevation(m) Temperature(C)and Pressure(Pa)
env = environement(350, 15, 97190.44, roro );

% Calculate Xcp Barrowman for documentation
[Xcp_Barrowman, Xcp_Planform, Ssm_Barrowman] = Xcp_Barrowman(roro);

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
axis([-500 500 -500 500 0 800])

h_max=max(state(:,3))
%%
clog = log; %clean_log(t); %extract_data ( state,t);

Xcp_B = ones(length(clog),1) * Xcp_Barrowman;
Xcp_P= ones(length(clog),1) * Xcp_Planform;
%%
figure(3)

plot(clog(:,10),clog(:,2))

xlabel('Time')
ylabel('Value1, Value2')
axis([0 20 0.25 1])
% 
figure(4)
plot(clog(:,10),clog(:,1))
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

%% --- Figure Flight Information ---

figure('Name','Flight information','Position', [400 0 400 1000]);
subplot(4,1,1)
plot(clog(:,10),clog(:,6))
xlabel('Time')
ylabel('Xdot [m/s]')
title('Velocity')
axis([0 25 0 300])
line([t_RailExit t_RailExit],[0 300],'Color',[0 1 0])
line([t_Burnout t_Burnout],[0 300],'Color',[1 0 0])

subplot(4,1,2)
plot(clog(:,10),clog(:,5))
xlabel('Time')
ylabel('Mass [kg]')
title('Mass')
axis([0 25 20 30])
line([t_RailExit t_RailExit],[0 300],'Color',[0 1 0])
line([t_Burnout t_Burnout],[0 300],'Color',[1 0 0])

subplot(4,1,3)
plot(clog(:,10),clog(:,2))
xlabel('Time(s)')
ylabel('Cd []')
title('Drag Coefficient')
axis([0 25 0 1])
line([t_RailExit t_RailExit],[0 300],'Color',[0 1 0])
line([t_Burnout t_Burnout],[0 300],'Color',[1 0 0])

subplot(4,1,4)
plot(clog(:,10),clog(:,1))
xlabel('Time')
ylabel('alpha [rad]')
title('Angle of Attack')
axis([0 25 0 0.4])
line([t_RailExit t_RailExit],[0 300],'Color',[0 1 0])
line([t_Burnout t_Burnout],[0 300],'Color',[1 0 0])

%% --- Figures Stability Analysis ---
% COP
figure('Name','Stability Analysis', 'Position', [0 0 400 1000])
subplot(4,1,1)
hold on
plot(clog(:,10),clog(:,7))
plot(clog(:,10),Xcp_B)
plot(clog(:,10),Xcp_P)
hold off
xlabel('Time')
ylabel('Xcp [m]')
title('Center of Pressure')
axis([0 25 1.5 1.9])
line([t_RailExit t_RailExit],[0 300],'Color',[0 1 0])
line([t_Burnout t_Burnout],[0 300],'Color',[1 0 0])
legend('Barrowman+BodyLift','Barrowman','Planform','rail exit','burnout')
% COG
subplot(4,1,2)
plot(clog(:,10),clog(:,4))
xlabel('Time')
ylabel('Xcm [m]')
title('Center of Mass')
axis([0 25 1.4 1.7])
line([t_RailExit t_RailExit],[0 300],'Color',[0 1 0])
line([t_Burnout t_Burnout],[0 300],'Color',[1 0 0])
% STABILITY MARGIN
subplot(4,1,3)
hold on
plot(clog(:,10),clog(:,9))
plot(clog(:,10),clog(:,9))
hold off
xlabel('Time')
ylabel('Margin []')
title('Static Stability Margin')
axis([0 25 0 3])
line([t_RailExit t_RailExit],[0 300],'Color',[0 1 0])
line([t_Burnout t_Burnout],[0 300],'Color',[1 0 0])
legend('Barrowman+BodyLift','Barrowman','rail exit','burnout')
% DAMPING RATIO
subplot(4,1,4)
plot(clog(:,10),clog(:,8))
xlabel('Time')
ylabel('Zeta []')
title('Damping Ratio')
axis([0 25 0.0 0.3])
line([t_RailExit t_RailExit],[0 300],'Color',[0 1 0])
line([t_Burnout t_Burnout],[0 300],'Color',[1 0 0])
