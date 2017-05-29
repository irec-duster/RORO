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


def return_param(data, param_name):
    param_index = data[0].index(param_name)
    data_vec = list(zip(*data[40000:]))
    param_value = data_vec[param_index]
    time_index = data[0].index('TIME_StartTime')
    time = data_vec[time_index]
    return [float(t) for t in time], [float(p) for p in param_value]



f = open(sys.argv[1], 'r')
csv_reader = csv.reader(f)
data = list(csv_reader)


def csv_writer(data, path):
    with open(path, "wb") as csv_file:
        writer = csv.writer(csv_file, delimiter=',')
        for line in data:
            writer.writerow(line)



# vehicle_attitude_roll_index  = data[0].index('ATT_Roll')
# vehicle_attitude_pitch_index  = data[0].index('ATT_Pitch')
# vehicle_attitude_yaw_index  = data[0].index('ATT_Yaw')

# RC_Throttle_index = data[0].index('RC_C0')
# RC_Roll_index = data[0].index('RC_C1')
# RC_Pitch_index = data[0].index('RC_C2')
# RC_Yaw_index = data[0].index('RC_C3')
# AIRS_True_Value_index= data[0].index('AIRS_TAS')
# Barometer_altitude_index = data[0].index('SENS_BaroAlt')
# Aircraft_Roll_rate_index = data[0].index('IMU_GyroX')
# Aircraft_Pitch_rate_index = data[0].index('IMU_GyroY')
# Aircraft_Yaw_rate_index = data[0].index('IMU_GyroZ')



# RC_Yaw = data_vect[RC_Yaw_index]

# Aircraft_Roll_rate = data_vect[Aircraft_Roll_rate_index]
# Aircraft_Pitch_rate = data_vect[Aircraft_Pitch_rate_index]
# Aircraft_Yaw_rate = data_vect[Aircraft_Yaw_rate_index]


# Barometer_altitude = data_vect[Barometer_altitude_index]
# Airspeed = data_vect[AIRS_True_Value_index]


# legend = data[0]

f, ax = plt.subplots(4,  sharex = 'all')
axi = 0
ax[axi].plot(*return_param(data, 'SENS_BaroAlt'))
ax[axi].set_ylabel('SENS_BaroAlt')
axi += 1
# ax[1].plot(time, Airspeed)
# ax[1].set_ylabel(data[0][AIRS_True_Value_index])

RC_time, RC_2 = return_param(data, 'RC_C1')
RC_time, RC_3 = return_param(data, 'RC_C2')
RC_Roll = [ch2 + ch3 for ch2, ch3 in zip(RC_2, RC_3)]
RC_Pitch = [ch2 - ch3 for ch2, ch3 in zip(RC_2, RC_3)]
ax[axi].plot(RC_time, RC_Roll)
ax[axi].plot(RC_time, RC_Pitch)
ax[axi].set_ylabel("RC Roll and Pitch")
axi += 1


ax[axi].plot(*return_param(data, 'IMU_GyroX'))
ax[axi].plot(*return_param(data, 'IMU_GyroY'))
ax[axi].plot(*return_param(data, 'IMU_GyroZ'))
axi += 1


ax[axi].plot(*return_param(data, 'IMU_AccX'))
ax[axi].plot(*return_param(data, 'IMU_AccY'))
ax[axi].plot(*return_param(data, 'IMU_AccZ'))
axi += 1





plt.show()
