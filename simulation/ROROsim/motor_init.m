function motor_init( roro )
    %% data from thrustcurve.org
    R1=4;
    C1=0;
    motorname = char(roro.motorname);
    roro.motordata = dlmread(motorname,'',R1,C1);
end

