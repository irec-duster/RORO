function [qw ,qx, qy, qz ] = ang2quat(  yaw, pitch, roll )
     t0 = cos(yaw * 0.5);
	 t1 = sin(yaw * 0.5);
	 t2 = cos(roll * 0.5);
	 t3 = sin(roll * 0.5);
	 t4 = cos(pitch * 0.5);
	 t5 = sin(pitch * 0.5);

	qw = t0 * t2 * t4 + t1 * t3 * t5;
	qx = t0 * t3 * t4 - t1 * t2 * t5;
	qy = t0 * t2 * t5 + t1 * t3 * t4;
	qz = t1 * t2 * t4 - t0 * t3 * t5;

end

