function [Cd]=Cd_mandell(roro) 
    % Equations from Mandell
    global env
    rho =  env.rho; % density 
    mu =  env.mu;  % dynamic viscosity 
    C = env.C; %speed of sound dry air 15C sea level
    V = norm(roro.Xdot,2);   %ms-1 Mag of  characteristic velocity at center of pressure location 
    M = V/C;
    Re  = roro.Re;

    % Approx laminar flow over rocket

    %% Rocket dimentions 
    L = roro.Length; 
    L_cone = roro.Cone_L;
    L_cyl = L - L_cone;
    D_cyl = roro.D;
    R_cyl = D_cyl/2;

    %Fin Geometry
    fin.n=roro.fin_n;
    fin.sweep = roro.fin_sweep;
    fin.h = roro.fin_h;
    fin.topchord = roro.fin_top;
    fin.basechord = roro.fin_base;
    fin.t = roro.fin_t;
    fin.a_ref = fin.n*fin.h*fin.t;
    fin.area = (fin.topchord+fin.basechord)/2*fin.h;
    fin.a_wet = fin.n*2*fin.area;
    fin.c = (fin.topchord+fin.basechord)/2;
    fin.X_b =  L - fin.basechord; % fin location

    %A_ref
    A_ref = pi*R_cyl^2;

    F_ratio = L / D_cyl;


    %%
    R_ogive = (L_cone^2+ D_cyl^2/4)/D_cyl;
    %A_wet

    fun = @(x) 2*pi*(sqrt(R_ogive^2 - power((L_cone - x),2))+R_cyl-R_ogive);

    A_wet =  integral(fun, 0, L_cone);

    A_wet = A_wet + 2*pi*R_cyl*L_cyl; 



    %% Assigning to equations as discribes in mandell
    l_n = L_cone;

    d_n = D_cyl;
    d_b = D_cyl;
    d_f = D_cyl;
    d_d = D_cyl;
    l_c  = 0; % no btail
    l_b = L_cyl;
    Re_c = 5e5;
    T_f = fin.t;
    l_TR = L;
    n=fin.n;
    % mid chord sweep

    temp.x1 = fin.h*tan(fin.sweep);

    temp.x2 = temp.x1 + fin.topchord - fin.basechord;

    fin.sweepc = atan2((fin.basechord/2 + (temp.x2-fin.topchord/2)),fin.h);

    clear temp fun fun2
    l_m = fin.h/acos(fin.sweepc); % length midchord

    A_fe= (fin.topchord+fin.basechord)/2*fin.h;

    A_fp = A_fe + 0.5*d_f*fin.basechord;

    %% ------Viscous Friction------
    % Viscous friction ROCKET FORBODY Cf
    B = Re_c*(0.074/Re^(0.2) - 1.328/sqrt(Re));

    if (Re < Re_c)
        Cf =  1.328/sqrt(Re);
    else
        Cf=0.074/Re^(0.2)-B/Re;
    end

    %  Viscous friction ROCKET FINS Cf_f
    Re_f  = env.rho*V*l_m/env.mu;  %Note the V is at the cop not the finneed to recalculate for better results 

    B_f = Re_c*(0.074/Re_f^(0.2) - 1.328/sqrt(Re_f));

    if (Re_f < Re_c)
        Cf_f =  1.328/sqrt(Re_f);
    else
        Cf_f=0.074/Re_f^(0.2)-B_f/Re_f;
    end
    
    %% -------Drag at zero AoA-------
    % Body drag, Box Eq41
    Cd_fb = (1 + 60/(l_TR/d_b)^3+0.0025*l_b/d_b)*(2.7*l_n/d_b +4*l_b/d_b + 2*(1-d_d/d_b)*l_c/d_b)*Cf;
    % Base drag, Box Eq42
    Cd_b = 0.029*(d_d/d_b)^3/sqrt(Cd_fb);
    % Fin drag, Box Eq44
    Cd_f = 2*Cf_f *(1+2*T_f/l_m)*4*n*A_fp/(pi*d_f^2);
    % Interference drag, Box Eq44
    Cd_i = 2*Cf_f*(1+2*T_f/l_m)*4*n*(A_fp-A_fe)/(pi*d_f^2);
    % Total drag coefficient at zero angle of attack
    Cd0 = Cd_fb + Cd_b + Cd_f + Cd_i;

    % Launch pin drag
    A_pin = roro.L_pinDia*roro.L_pinH;
    Cd_pin = 2*0.5*A_pin/A_ref; 
    Cd0 = Cd0 + Cd_pin;
    % compressibility correction
    %% -------Additional drag at AoA-------
    % Alpha
    alpha = roro.alpha;
    % Coefficients delta dn eta from windtunnel experiments, See Box p13
    deltaktab=[4 6 8 10 12 14 16 18 20;0.78 0.86 0.92 0.94 0.96 0.97 0.975 0.98 0.982];
    etatab=[4 6 8 10 12 14 16 18 20 22 24;0.6 0.63 0.66 0.68 0.71 0.725 0.74 0.75 0.758 0.77 0.775];
    % error in paper
    etak=interp1(etatab(1,:),etatab(2,:),F_ratio,'linear','extrap');
    deltak=interp1(deltaktab(1,:),deltaktab(2,:),F_ratio,'linear','extrap');
    if etak>1;
        etak=1;
    end
    if deltak>1;
        deltak=1;
    end
    % Body drag at angle alpha
    Cd_b_alpha = 2*deltak*alpha^2 + 3.6*etak*(1.36*l_TR - 0.55*l_n)*alpha^3/(pi*d_b);
    % Fin body interference coefficients
    Rs = R_cyl/(R_cyl+fin.h);
    Kbf=0.8065*Rs^2+1.1553*Rs;
    Kfb=0.1935*Rs^2+0.8174*Rs+1;
    % Fin drag at angle alpha
    Cd_f_alpha = (1.2*A_fp*4/(pi*d_f^2) +3.12*(Kfb +Kbf-1)*A_fe*4/(pi*d_f^2))*alpha^2;

    %% -------Totol Drag Coefficient-------
    Cd = Cd0 + Cd_b_alpha + Cd_f_alpha;
    Cd = Cd/sqrt(1-M^2);
    CnXcp = roro.CnXcp;
    Cn= CnXcp(1);
    Cd = (Cd*cos(alpha) -0.5*Cn*sin(2*alpha))/(1-sin(alpha)^2);

end
