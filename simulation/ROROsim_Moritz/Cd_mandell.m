function [Ca, Cd] = Cd_mandell(roro) 
    %%Equations from Mandell, See Box S., 2009 Aerodynamic Coefficients from p.12f
    global env
    rho =  env.rho; % density 
    mu =  env.mu;  % dynamic viscosity 
    C = env.C; %speed of sound dry air 15C sea level
    V = norm(roro.Xdot,2);   %ms-1 Mag of  characteristic velocity at center of pressure location 
    M = V/C;
    Re  = roro.Re;

    % Rocket dimentions 
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

    %% Assigning to equations as discribed in mandell
    l_n = L_cone;

    d_b = D_cyl;
    d_f = D_cyl;
    d_d = D_cyl;
    l_c  = 0; % no btail
    l_b = L_cyl;
    Re_c = 5e5;
    T_f = fin.t;
    l_TR = L;
    l_TS = D_cyl+2*fin.h;
    n=fin.n;
    
    % mid chord sweep
    temp.x1 = fin.h*tan(fin.sweep);
    temp.x2 = temp.x1 + fin.topchord - fin.basechord;
    fin.sweepc = atan2((fin.basechord/2 + (temp.x2-fin.topchord/2)),fin.h);
    clear temp
    
    l_m = fin.h/acos(fin.sweepc);
    A_fe= (fin.topchord+fin.basechord)/2*fin.h;
    A_fp = A_fe + 0.5*d_f*fin.basechord;

    %% Viscous Friction (See Box S., 2009, p13)
    %Viscous friciton ROCKET FOREBODY Cf_fb
    B = Re_c*(0.074/Re^(0.2) - 1.328/sqrt(Re)); %Box Eq46
    if (Re < Re_c) % Box Eq45
        Cf_fb =  1.328/sqrt(Re); 
    else
        Cf_fb=0.074/Re^(0.2)-B/Re;
    end
    
    % Viscous friction FIN Cf_f
    % Re fins (Different characteristic length than for Re Rocket)
    Re_f  = env.rho*V*l_m/env.mu;
    B_f = Re_c*(0.074/Re_f^(0.2) - 1.328/sqrt(Re_f)); 
    if (Re_f < Re_c)
        Cf_f =  1.328/sqrt(Re_f);
    else
        Cf_f=0.074/Re_f^(0.2)-B_f/Re_f;
    end
 
    %% Drag  zero angle of attack
    % Body drag, Box Eq41
    Cd_fb = (1 + 60/(l_TR/d_b)^3+0.0025*l_b/d_b)*(2.7*l_n/d_b +4*l_b/d_b + 2*(1-d_d/d_b)*l_c/d_b)*Cf_fb;
    % Base drag, Box Eq 42
    Cd_b = 0.029*(d_d/d_b)^3/sqrt(Cd_fb);
    % Fin drag, Box Eq43
    Cd_f = 2*Cf_f*(1+2*T_f/l_m)*4*n*A_fp/(pi*d_f^2);
    % Interference drag, Box Eq 44
    Cd_i = 2*Cf_f*(1+2*T_f/l_m)*4*n*(A_fp-A_fe)/(pi*d_f^2);
    
    % Total zero angle of attack drag, Box Eq48
    Cd0 = Cd_fb + Cd_b + Cd_f + Cd_i;

    % Launch pin drag
    A_pin = roro.L_pinDia*roro.L_pinH;
    Cd_pin = 2*0.5*A_pin/A_ref; 
    Cd0 = Cd0 + Cd_pin;
    
    %% Additional Drag at angle of attack
    
    alpha = roro.alpha; %angle of attack
    
    
    % coefficients delta and eta from windtunnel experiments (See Box, 2009, p13)
    deltaktab=[4 6 8 10 12 14 16 18 20;0.78 0.86 0.92 0.94 0.96 0.97 0.975 0.98 0.982];
    etatab=[4 6 8 10 12 14 16 18 20 22 24;0.6 0.63 0.66 0.68 0.71 0.725 0.74 0.75 0.758 0.77 0.775]; % error in paper
    etak=interp1(etatab(1,:),etatab(2,:),F_ratio,'linear','extrap');
    deltak=interp1(deltaktab(1,:),deltaktab(2,:),F_ratio,'linear','extrap');
    if etak>1
        etak=1;
    end
    if deltak>1
        deltak=1;
    end
    
    % Body drag at angle, Box Eq49
    Cd_b_alpha = 2*deltak*alpha^2 + 3.6*etak*(1.36*l_TR - 0.55*l_n)*alpha^3/(pi*d_b);
    % Fin-body interference coefficients
    Rs = l_TS/R_cyl;
    Kbf=0.8065*Rs^2+1.1553*Rs; %Eq51
    Kfb=0.1935*Rs^2+0.8174*Rs+1; %Eq52
    % Fin drag at angle, Eq50
    Cd_f_alpha = (1.2*A_fp*4/(pi*d_f^2) +3.12*(Kfb +Kbf-1)*A_fe*4/(pi*d_f^2))*alpha^2;
    
    %% Total Drag Coefficient
    Cd = Cd0 + Cd_b_alpha + Cd_f_alpha; %Eq53
    CnXcp = roro.CnXcp;
    Cn= CnXcp(1);
    % Converting Cd in axial direction
    Ca = (Cd*cos(alpha) -0.5*Cn*sin(2*alpha))/(1-sin(alpha)^2);
    
    % compressibility correction, Eq55
    beta = sqrt(1-M^2);
    Cd = Cd/beta;
    Ca = Ca/beta;
end
