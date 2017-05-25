function motor_init( roro )
    %% data from thrustcurve.org
    % Reads the .eng thrust file of the motors and calculates the reletive 
    % the reletive properties of the motor. 
    
    % Extracting thrust curve
    R1=4;
    C1=0;
    motorname = char(roro.motorname);
    motordata = dlmread(motorname,'',R1,C1);
    roro.motordata = [0, 0; motordata]; % at 0 0 at start of date
    
    % Very strange way of reading mass of motor amd propellent from the
    % file
    fid = fopen(motorname);
    tline = fgets(fid);
    tline = fgets(fid);
    tline = fgets(fid);
    %tline = fgets(fid);
    C = strsplit(tline);
    Motor_diameter = str2double((C(2)))*1e-3; %[m]
    Motor_lenght = str2double((C(3)))*1e-3; %[m]
    Mass_prop = str2double((C(5)));
    Mass_motor = str2double((C(6)));
    roro.propM_tot = Mass_prop;
    roro.Mass_motor = Mass_motor - Mass_prop;
    fclose(fid);
    
    %Initally current and total prop mass are equal
    roro.propM_current = Mass_prop;
    % Calculated impulse from motor data using spline
    roro.Motor_impulse = trapz(roro.motordata(:,1),roro.motordata(:,2));
    
    %Initializing propellent dimentions 
    prop_density = 1.5079e+03; % calciualed by volume and mass of prop from drawing   
    prop_OD =  Motor_diameter - 12.4000e-3; %[m] %Offsets estimates from drawing
    prop_h = Motor_lenght - 121.10003e-3;   %[m]
    prop_ID = sqrt(prop_OD^2 -4*Mass_prop/(pi*prop_h*prop_density ));
    roro.Xcm_prop = roro.Length - prop_h/2; % this is not correct
    
    roro.prop_density= prop_density;
    roro.prop_OD = prop_OD;
    roro.prop_h = prop_h;
    
    %Initializing propellent inertias w.r.t. cm
    d = roro.Xcm_prop - roro.Xcm; % Note: The mass of the prop should already be updated in roro to get correct Xcm 
    propIx = 0.5*Mass_prop*(prop_OD^2+prop_ID^2)/4;
    propIy = Mass_prop/12*(3*(prop_ID^2+prop_ID^2)/4 + prop_h^2) + Mass_prop*(d);
    roro.Iprop = [propIx, 0, 0; 0, propIy, 0; 0, 0, propIy]; % propIy = propIz 
    
    
    
%     TODO: implement same thing using splines
%     spl = spline(roro.motordata(:,1),roro.motordata(:,2));
% 
%     % integrate the spline
%     spl.coefs = spl.coefs*[diag(1./[4 3 2 1]'),zeros(4,1)];
%     spl.order = 5;
%     dx = diff(spl.breaks)';
%     C = spl.coefs(:,1);
%     for i = 1:4
%       C = C.*dx + spl.coefs(:,i+1);
%     endzeros(3,3); 
%     spl.coefs(2:end,5) = cumsum(C(1:(end-1));
end

