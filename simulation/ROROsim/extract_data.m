function a = extract_data ( state,t)
% Function run the states through the model again to extract the needed
% internal parameters. This has to be done becasue ode45 does not allow for
% the extraction of internal parameters
    global env
    roro_data =rocket(init_rocket());
    motor_init( roro_data );
    env = environement(1400, 25, 86000, roro_data );
    %%
    roro_data.X = state(1,1:3)';
    roro_data.Q = state(1,4:7)'; 
    roro_data.P = state(1,8:10)';
    roro_data.L = state(1,11:13)'; 
    roro_data.time= t(1);
    %%
    ilast = length(t);
    a = 0; %zeros(ilast,3);
    %%
    for i = 2:ilast
        
         roro_data.time= t(i);
         roro_data.deltat = t(i)-t(i-1);
         roro_data.X = state(i,1:3)';
         roro_data.Q = state(i,4:7)'; 
         roro_data.P = state(i,8:10)';
         roro_data.L = state(i,11:13)';
         burn_data(roro_data);
         roro_data.Xdot = (state(i,1:3)' - state(i-1,1:3)')/roro_data.deltat;
         roro_data.Qdot = (state(i,4:7)' - state(i-1,4:7)')/roro_data.deltat ; 
         roro_data.Pdot = (state(i,8:10)' - state(i-1,8:10)')/roro_data.deltat;
         roro_data.Ldot = (state(i,11:13)' -state(i-1,11:13)')/roro_data.deltat;
         
        Rmatrix= quat2rotm(roro_data.Q');
        RA = Rmatrix*env.RA0';
        X = roro_data.X;
        Xdot = roro_data.Xdot;
        L = roro_data.L;
        if(norm(X) < roro_data.Rail)
            W = [0, 0, 0]';
        else
            W = env.W;
        end
        CnXcp = roro_data.CnXcp;
        Cn= CnXcp(1);
        Xcp= CnXcp(2);
        Cda = CnXcp(3); % Damping coefficient
        zeta = CnXcp(4); % Damping ratio
        Ssm = CnXcp(5); % Static stability margin
        invIbody = roro_data.Ibody\eye(3); %inv(roro.Ibody); inverting matrix
        omega = Rmatrix*invIbody*Rmatrix'*L;
        Vcm = Xdot  + W;
        Xstab = Xcp- roro_data.Xcm;

        omega_norm = normalize(omega); %normalized
        Xprep =Xstab*sin(acos(dot(RA,omega_norm))); % Prependicular distance between omaga and RA
        
        Vomega = Xprep *cross(RA,omega);
        
        V = Vcm + Vomega; % approxamating the velocity of the cop        
        
        Vmag = norm(V);
        Vnorm = normalize(V);
        alpha = acos(dot(Vnorm,RA));
        roro_data.alpha = alpha;
        
        %% Log Data
       
        logData(roro_data.alpha, roro_data.Cd, Cda, roro_data.Xcm, roro_data.Mass, Vmag, Xcp, zeta, Ssm, t(i));

         %logData(roro_data.X(3),roro_data.Cd,t(i)); % Eg roro.Cd for drag norm(Xdot)/env.C
%          log(i,1) = norm(roro_data.Xdot);
%          log(i,2) = roro_data.Re;
%          log(i,3) = t(i);
    end
        
end

