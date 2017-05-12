function motor_init( roro )
    %% data from thrustcurve.org
    
    % Burn data
    R1=4;
    C1=0;
    motorname = char(roro.motorname);
    motordata = dlmread(motorname,'',R1,C1);
    roro.motordata = [0, 0; motordata]; % at 0 0 at start of date
    % Very strange way of reading mass of motor amd propellent 
    fid = fopen(motorname);
    tline = fgets(fid);
    tline = fgets(fid);
    tline = fgets(fid);
    tline = fgets(fid);
    C = strsplit(tline);
    x=(C(5));
    y=(C(6));
    roro.Mass_propellent=str2double(x);
    roro.Mass_motor=str2double(y) - roro.Mass_propellent;
    fclose(fid);
    % Calculated impulse from motor data using spline
    roro.Motor_impulse = trapz(roro.motordata(:,1),roro.motordata(:,2));
    
%     spl = spline(roro.motordata(:,1),roro.motordata(:,2));
% 
%     % integrate the spline
%     spl.coefs = spl.coefs*[diag(1./[4 3 2 1]'),zeros(4,1)];
%     spl.order = 5;
%     dx = diff(spl.breaks)';
%     C = spl.coefs(:,1);
%     for i = 1:4
%       C = C.*dx + spl.coefs(:,i+1);
%     end
%     spl.coefs(2:end,5) = cumsum(C(1:(end-1));
end

