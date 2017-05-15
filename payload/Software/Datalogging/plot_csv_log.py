# TIME - Time stamp
# ATT - Vehicle attitude
# ATSP - Vehicle attitude setpoint
# IMU - IMU sensors
# SENS - Other sensors
# LPOS - Local position estimate
# LPSP - Local position setpoint
# GPS - GPS position
# ATTC - Attitude controls (actuator_0 output)
# STAT - Vehicle state
# RC - RC input channels
# OUT0 - Actuator_0 output
# AIRS - Airspeed (TAS - True Airspeed; IAS - indicated AIRSPEED)
# ARSP - Attitude rate setpoint
# FLOW - Optical flow
# GPOS - Global position estimate
# GPSP - Global position setpoint
# ESC - ESC state
# GVSP - Global velocity setpoint

# pitch up - ch2+ ch3-
# pitch down opposite
# roll right Ch2+ ch3+

import sys
import matplotlib.pyplot as plt
import math
import csv


f = open(sys.argv[1], 'r')
csv_reader = csv.reader(f)
data = list(csv_reader)
time_index = data[0].index('TIME_StartTime')
vehicle_attitude_roll_index  = data[0].index('ATT_Roll')
vehicle_attitude_pitch_index  = data[0].index('ATT_Pitch')
vehicle_attitude_yaw_index  = data[0].index('ATT_Yaw')

RC_Throttle_index = data[0].index('RC_C0')
RC_Roll_index = data[0].index('RC_C1')
RC_Pitch_index = data[0].index('RC_C2')
RC_Yaw_index = data[0].index('RC_C3')

print(RC_Throttle_index)

data_vect = list(zip(*data[1:]))
time = data_vect[time_index]
vehicle_atittude_roll = data_vect[vehicle_attitude_roll_index]
vehicle_atittude_pitch = data_vect[vehicle_attitude_pitch_index]
vehicle_atittude_yaw = data_vect[vehicle_attitude_yaw_index]
RC_Throttle = data_vect[RC_Throttle_index] #
RC_Roll = data_vect[RC_Roll_index]
RC_Pitch = data_vect[RC_Pitch_index]
RC_Yaw = data_vect[RC_Yaw_index]


legend = data[0]

fig = plt.figure()
f, ax = plt.subplots(2, sharex=True)

ax[0].plot(time, vehicle_atittude_roll)
ax[1].plot(time, vehicle_atittude_pitch)
#ax.plot(time, vehicle_atittude_yaw)
#ax.plot(time, RC_Throttle)
#ax.plot(time, RC_Roll)
#ax.plot(time, RC_Pitch)
#ax.plot(time, RC_Yaw)



plt.show()
