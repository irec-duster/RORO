function  accent_calc( roro )
%Function calculates the assent phase of the rocket
    global env;
    Re
    Mach
    Cd
    Cn
    
    X
    Q
    P
    L
    tspan = [0 12];
    y0 = [0; 1; 1];

    
    % Solve flight using ODE45
    [t, state]= ode45(@flight,tspan,state_0);

    % --------------------------------------------------------------------------
    %% Equations of motion discribed to be sloved in ode45 
    function state_dot = flight(t,state)
        %Calculate angle of attack (alpha)*
        
        
        % Rotation matrix for transforming body coord to ground coord
        Rmatrix= quat2rotm(Q);
        
        % Axis wrt earth coord
        YA = Rmatrix*env.YA0'; 
        PA = Rmatrix*env.PA0'; 
        RA = Rmatrix*env.RA0'; 
    
   
        % Velocity
        Xdot=roro.P./roro.M;
        
        % Angular velocity in quarternians 
        invIbody = inv(roro.Ibody);
        omega = R*invIbody*R'*roro.L';
        s = Q(1);
        v =[Q(1) Q(2) Q(3)];
        sdot = 0.5*(dot(omega,v));
        vdot = 0.5*(s*omega + cross(omega,v));
        Qdot = [s, vdot]
        
        % Angle of attack: Angle between velocity vector of the CoP to the roll axis, given in the ground coord        
        % To Do : windmodel in env, Xcm in roro
        Vcm = Xdot  + W;
        Xstab = Xcm - Xcp;
        if(Xstab < 0)
            warning('Rocket unstable');
        end
        omega_norm = omega/norm(omega); %normalized
        Xprep =Xstab*sin(acos(dot(RA,omega_norm)));
        
        Vomega = Xprep *cross(RA,omega);
        
        V = Vcm + Vomega % approxamating the velocity of the cop        
        
        
        Vnorm = V/norm(V);
        alpha = acos(dot(Vnorm,RA));
        
        % Forces = rate of change of Momentums
        
        Fthrust = -roro.T*RA;
        
        Fg = [0 0 -roro.Mass*env.g]';
        
        % Axial Forces
        Famag = 0.5*env.rho*V^2*roro.A_ref*roro.Ca; 
        
        Fa = -Famag*RA
        
        % Normal Forces
        Fnmag = 0.5*env.rho*V^2*roro.A_ref*roro.Cn;
        
        RA_Vplane = cross(RA,Vnorm);
        Fn = Fnmag*(cross(RA,RA_Vplane));
        
        % Torque
        Trqn = Fnmag*Xstab*(RA_Vplane) 
        
        %Tqm=(Cda1*omega)*omegaax2; rotational torque by motor
%         r_f = %TODO
%         Trmag = 0.5*env.rho*V^2*roro.A_ref*roro.Cld*r_f;
%         Tr = Trmag*RA;
        Trq = Tn;
        
        
        
        
        
    end
end
 
