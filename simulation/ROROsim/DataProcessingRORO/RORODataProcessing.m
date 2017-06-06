% RORO Data 
clear all;

dataRaw = csvread('mpu6000.csv', 1, 0);
 %%
liftoff = 2399.3;
touchdown = 81.8500 + 2399.3;
separation = 10.4700 + 2399.3;
burnout = 1.7 + 2399.3;
toffset= liftoff- 40;
t = dataRaw(:,1)-toffset; 
% relevent data 3495 sec after turning on IMU
indStart = find(t>0,1);
  
indEnd = find(t>90,1);
  
%cliped data
flightdataRaw = dataRaw(indStart:indEnd,:);

flightdataRaw = cleanData(flightdataRaw); % removes values wiht dt=0


t= flightdataRaw(:,1)-toffset;
indextEnd = length(t);
gyro_x =flightdataRaw(:,2);
gyro_y =flightdataRaw(:,3);
gyro_z =flightdataRaw(:,4);
acc_x =flightdataRaw(:,5);
acc_y =flightdataRaw(:,6);
acc_z =flightdataRaw(:,7);

% Gyro calibration
gyro_bias_t = 35;
ind= find(t>gyro_bias_t, 1);
gyro_bias = [mean(gyro_x(1:ind)); mean(gyro_y(1:ind)); mean(gyro_z(1:ind))];
rate_x =gyro_x-gyro_bias(1);
rate_y =gyro_y-gyro_bias(2);
rate_z =gyro_z-gyro_bias(3);

% Acceleration Value calibration
xbias=[0.12570896416609578, 0.9984256389897912];
ybias=[0.0031463042356696391, 0.99443041010383559];
zbias=[-0.028494989398200055, 1.0144324415852917];
acc_x = (acc_x - xbias(1))/xbias(2);
acc_y = (acc_y - ybias(1))/ybias(2);
acc_z = (acc_z - zbias(1))/zbias(2);

%starting att
acc_mean = [mean(acc_x(1:ind)); mean(acc_y(1:ind)); mean(acc_z(1:ind))];
gMag = norm(acc_mean);
roll= 0;
pitch = atan(acc_mean(1)/acc_mean(3));
yaw =0;
flightdata = [t, acc_x, acc_y, acc_z, rate_x, rate_y, rate_z];
t = t -35;
indStart = find(t>0,1);
  
indEnd = find(t>17,1);
flightdata = flightdata(indStart:indEnd,:);
%%
t =flightdata(:,1);
indextEnd = length(t);
acc_x=flightdata(:,2);
acc_y=flightdata(:,3);
acc_z=flightdata(:,4);
rate_x=flightdata(:,5);
rate_y=flightdata(:,6);
rate_z=flightdata(:,7);
% Shift Accel to CG
imu_pos = [0.013, 0.005, 1.782]';% meters from nose cone

Xcm = [0,0,1.6]';

r = imu_pos - Xcm;
omega = [rate_x,rate_y,rate_z]';

acc_x_cm = zeros(indextEnd,1);
acc_y_cm = zeros(indextEnd,1);
acc_z_cm = zeros(indextEnd,1);
for i=1:indextEnd
a_cent =  -cross(omega(:,i),cross(omega(:,i),r));
acc_x_cm(i) = acc_x(i) + a_cent(1);
acc_y_cm(i) = acc_y(i) + a_cent(2);
acc_z_cm(i) = acc_z(i) + a_cent(3);
end 

%% Inital angle from acceleration vector

[w, x, y, z] = ang2quat(yaw, pitch, roll); % Q0

Q = zeros(indextEnd,4);
Qdot = zeros(indextEnd,4);
Q(1,:) = [w, x, y, z];
Qdot(1,:) = [0, 0, 0, 0];
%% https://fgiesen.wordpress.com/2012/08/24/quaternion-differentiation/
for i=2:indextEnd
    dt = t(i) - t(i-1);
    Qdot(i-1,:)=0.5*quatmultiply([0, rate_x(i-1), rate_y(i-1), rate_z(i-1)],Q(i-1,:));
    Q(i,:)= Q(i-1,:) + dt*Qdot(i-1,:);
    Q(i,:)= normalize(Q(i,:));

end
%%
acc = zeros(indextEnd,3);
for i=1:indextEnd
    Rmatrix= quat2rotm(Q(i,:));
    accTemp =  Rmatrix*[acc_x_cm(i), acc_y_cm(i), -acc_z_cm(i)]';
    acc(i,:) = accTemp'- [0,0,gMag];
end
    
vel = zeros(indextEnd,3);
pos = zeros(indextEnd,3);
angAccel = zeros(indextEnd,3);
% for i=2:indextEnd
%     dt = t(i) - t(i-1);
%     att(i,:)= att(i-1,:)+[rate_x(i)*dt, rate_y(i)*dt,rate_z(i)*dt,]; 
% end


%% Vel and Pos
for i=2:indextEnd
    dt = t(i) - t(i-1);  
    vel(i,:)= vel(i-1,:)+[acc(i-1,1)*dt, acc(i-1,2)*dt,acc(i-1,3)*dt];
    pos(i,:)= pos(i-1,:)+[vel(i-1,1)*dt, vel(i-1,2)*dt,vel(i-1,3)*dt];
end
%% Anglar accelerations
for i=2:indextEnd
    dt = t(i) - t(i-1);
    if (dt ==0)
        error('dt val is zero at index %d', i)
    end
    angAccel(i,:)= [(rate_x(i)-rate_x(i-1))/dt, (rate_y(i)-rate_y(i-1))/dt,(rate_z(i)-rate_z(i-1))/dt,]; 
    

end
angAccel_x=lowpassFilter(angAccel(:,1),0.2);
angAccel_y=lowpassFilter(angAccel(:,2),0.2);
angAccel_z=lowpassFilter(angAccel(:,3),0.2);
%%
figure(1)
hold on
plot(t,rate_x)
plot(t,rate_y)
plot(t,rate_z)
hold off
title('Gyro Data')
xlabel('time(s)')
legend('x','y','z')

figure(2)
hold on
plot(t,acc(:,1))
plot(t,acc(:,2))
plot(t,acc(:,3))
hold off
title('Accel Data')    
xlabel('time(s)')
legend('x','y','z')

figure(3)
hold on
%plot(t,acc_x_cm)
%plot(t,acc_y_cm)
plot(t,acc_z_cm)
hold off
title('Acc')
xlabel('time(s)')
legend('x','y','z')
%%
figure(4)
hold on

plot(t,angAccel_x)
plot(t,angAccel_y)
plot(t,angAccel_z)
hold off
title('Angular accel')
xlabel('time(s)')
legend('x','y','z')
%%
figure(6)
hold on
plot(t,Q(:,1))
plot(t,Q(:,2))
plot(t,Q(:,3))
plot(t,Q(:,4))
hold off
title('Attirude ')
xlabel('time(s)')
legend('q0','q1','q2','q3')
%%
figure(7)
plot(t,pos(:,3))
xlabel('t(m)')
ylabel('Height (m)')
%axis([-500 500 -500 500 0 800])

  