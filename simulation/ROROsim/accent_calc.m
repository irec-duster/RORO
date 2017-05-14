function  [t, state] = accent_calc( roro,tend )
%Function calculates the assent phase of the rocket
    global env;

   
    state_0 = [roro.X; roro.Q; roro.P; roro.L];
    tspan = [0,tend];
    
    % Event function to stop at max height
    options = odeset('Events',@event_function);
    
    % Solve flight using ODE45
    [t, state]= ode45(@flight,tspan,state_0,options);

    % --------------------------------------------------------------------------
    %% Equations of motion discribed to be sloved in ode45 
    function state_dot = flight(t,state)
        %TODO: put condition on burn data so it does not excecute after
        %bunout
        
        if (t>0)
            roro.deltat = t - roro.time;
            roro.time = t;
            burn_data(roro); % runs each cycle to update motor stats 
            
        end
        X= state(1:3);
        Q= state(4:7);
        P= state(8:10);
        L= state(11:13);

        roro.X= state(1:3);
        roro.Q= state(4:7);
        roro.P= state(8:10);
        roro.L= state(11:13);
        % Rotation matrix for transforming body coord to ground coord
        Rmatrix= quat2rotm(roro.Q');
        
        % Axis wrt earth coord
        YA = Rmatrix*env.YA0'; 
        PA = Rmatrix*env.PA0'; 
        RA = Rmatrix*env.RA0'; 
        CnXcp = roro.CnXcp;
        Cn= CnXcp(1);
        Xcp= CnXcp(2);
        Cda = CnXcp(3); % Damping coefficient
        %% -------Velocity-------
        Xdot=P./roro.Mass;
        
        %% -------Angular velocity--------- in quarternians 
        invIbody = roro.Ibody\eye(3); %inv(roro.Ibody); inverting matrix
        omega = Rmatrix*invIbody*Rmatrix'*L;
        s = Q(1);
        v =[Q(1); Q(2); Q(3)];
        sdot = -0.5*(dot(omega,v));
        vdot = 0.5*(s*omega + cross(omega,v));
        Qdot = [sdot; vdot];
        
        %% -------Angle of attack------- 
        % Angle between velocity vector of the CoP to the roll axis, given in the ground coord        
        % To Do : windmodel in env, Model gives errors 
        if(norm(X) < roro.Rail)
            W = [0, 0, 0]';
        else
            W = env.W;
        end
        
        Vcm = Xdot  + W;
        Xstab = Xcp- roro.Xcm;
%         if(Xstab < 0)
%             warning('Rocket unstable');
%         end
        omega_norm = normalize(omega); %normalized
        Xprep =Xstab*sin(acos(dot(RA,omega_norm))); % Prependicular distance between omaga and RA
        
        Vomega = Xprep *cross(RA,omega);
        
        V = Vcm + Vomega; % approxamating the velocity of the cop        
        
        Vmag = norm(V);
        Vnorm = normalize(V);
        alpha = acos(dot(Vnorm,RA));
        roro.alpha = alpha;
        %% Forces = rate of change of Momentums

        Fthrust = roro.T*RA;
        
        mg = roro.Mass*env.g;
        Fg = [0, 0, -mg]';
        
        % Axial Forces
        Famag = 0.5*env.rho*Vmag^2*roro.A_ref*roro.Cd; % To DO: make axial 
        
        Fa = -Famag*RA;
        
        % Normal Forces
        Fnmag = 0.5*env.rho*Vmag^2*roro.A_ref*Cn;
        
        RA_Vplane = cross(RA,Vnorm);
        Fn = Fnmag*(cross(RA,RA_Vplane));
        
        if (roro.T< mg && X(3)< 0.1)
            Ftot = [0, 0, 0]';
        else
            Ftot = Fthrust + Fg + Fa + Fn;
        end
        %% Torque
        Trqn = Fnmag*Xstab*(RA_Vplane); 
        
        m=diag([1, 1, 0]);
        invR = Rmatrix';
        Trq_da = -Cda*Rmatrix*m*invR*omega;
        %Tqm=(Cda1*omega)*omegaax2; rotational torque by motor
%        r_f = %TODO roll damping 
%        Trmag = 0.5*env.rho*V^2*roro.A_ref*roro.Cld*r_f;
%        Tr = Trmag*RA;
        if(norm(X) < roro.Rail)
            Trq = [0, 0, 0]';
        else
            Trq = Trqn+Trq_da;
        end
        
        %update rocket state derivatives 
        roro.Xdot= Xdot;
        roro.Qdot= Qdot;
        roro.Pdot= Ftot;
        roro.Ldot= Trq;
            
        logData(roro.T,roro.Cd,t); % Eg roro.Cd for drag norm(Xdot)/env.C
        state_dot =[Xdot; Qdot; Ftot;Trq];
        
        
    end
    
    function [value,isterminal,direction] = event_function(t,state)
    %% stops ode integration when the max height is reached 
        if (t > 1 && state(10) <= 0) % Linear momentum in z direction is zero
            value = 0; % when value = 0, an event is triggered
        else
            value =1;
        end
        isterminal = 1; % terminate after the first event
        direction = 0;  % get all the zeros
    end
end
 
