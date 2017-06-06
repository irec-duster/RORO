function [ yaw, pitch, roll] = quat2ang(w ,x, y, z  )
	ysqr = y*y;
	
	t0 = +2.0 * (w * x + y*z);
	t1 = +1.0 - 2.0 * (x*x + ysqr);
	roll = atan2(t0, t1);
	
	t2 = +2.0 * (w*y - z*x);
    
	if t2 > 1 
        t2 =  1; 
    end
    if t2 < -1
        t2 = -1;
    end
	pitch = asin(t2);
	
	t3 = 2.0 * (w * z + x*y);
	t4 = 1.0 - 2.0 * (ysqr + z*z);
	yaw = atan2(t3, t4);

end