function  accent_calc( )
%Function calculates the assent phase of the rocket
    global roro;
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
        
        
        Vnorm = Xdot/norm(Xdot,2);
        alpha = acos(dot(Vnorm,RA));
        
        
        
        
    end
end
 
