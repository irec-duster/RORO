#!/usr/bin/env python3
import csv
import sys

X = 0
Y = 1
Z = 2
TIME = 3

def read(filename):
    f = open(filename, 'r')
    csv_reader = csv.reader(f)
    data = list(csv_reader)

    if (tuple(data[0][:7]) != ('time','gyro_x','gyro_y','gyro_z','acc_x','acc_y','acc_z')):
        print("wrong data format")
        exit(-1)

    data = data[1:]

    # ignore first values, until sensors are stable
    t_zero = float(data[0][0]) + 0.3
    ignore_samples = next(i for i,x in enumerate(data) if float(x[0]) > t_zero)
    print('ignore first {} samples'.format(ignore_samples))
    ignore_samples = 0
    gyro = [[float(pt[1]), float(pt[2]), float(pt[3]), float(pt[0])] for pt in data[ignore_samples:]]
    acc = [[float(pt[4]), float(pt[5]), float(pt[6]), float(pt[0])] for pt in data[ignore_samples:]]
    start_flight = 38580
    stop_flight = 41222

    return {'gyro': gyro[start_flight:stop_flight], 'acc': acc[start_flight:stop_flight]}


def main():
    import rotation_integration
    import sys
    import time
    rot = rotation_integration.Attitude()
    imu = read(sys.argv[1])
    gyro = imu['gyro']
    prev_time = gyro[0][TIME]
    print(prev_time)
    for sample in gyro:
        delta_t = sample[TIME] - prev_time
        prev_time = sample[TIME]
        sys.stderr.write('s {} t {} dt {}\n'.format(sample, sample[TIME], delta_t))
        rot.integration_step(sample[:3], delta_t)

        print('{},{},{},{}'.format(*rot.get_quaternion()))
        #print(delta_t)
        sys.stdout.flush()
        time.sleep(delta_t)


if __name__ == '__main__':
    main()

