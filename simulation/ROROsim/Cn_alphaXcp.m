function [Cn_alpha, Xcp, Xcp_Barrow, Xcp_Planform, Cda] = Cn_alphaXcp(roro) 
    % Takes rocket handle and environment  to calculate Cn, location of cop
    
    global env

    rho =  env.rho; % density 
    C = env.C; %speed of sound dry air 15C sea level
    V = norm(roro.Xdot,2);   %ms-1 Mag of characteristic velocity at center of pressure location 
    M = V/C;

    %% Rocket Geometry
    % Rocket dimentions
    L = roro.Length;
    L_cone = roro.Cone_L;
    L_cyl = L - L_cone;
    D_cyl = roro.D;
    R_cyl = D_cyl/2;

    %A_ref
    A_ref = pi*R_cyl^2;
    
    %Fin Geometry
    fin.n=roro.fin_n;
    fin.sweep = roro.fin_sweep;
    fin.h = roro.fin_h;
    fin.topchord = roro.fin_top;
    fin.basechord = roro.fin_base;
    fin.l_forward = tan(fin.sweep)*fin.h; % Length of forward pointing triangle
    fin.l_backward = (fin.basechord-fin.topchord-fin.l_forward); % length of backward pointing triangle
    fin.t = roro.fin_t;
    fin.a_ref = fin.n*fin.h*fin.t;
    fin.area = (fin.topchord+fin.basechord)/2*fin.h;
    fin.a_wet = fin.n*2*fin.area;
    fin.c = (fin.topchord+fin.basechord)/2;
    fin.X_b =  L-fin.basechord; % fin location
    % mid chord sweep
    x1 = fin.h*tan(fin.sweep);
    x2 = x1 + fin.topchord - fin.basechord;
    fin.sweepc = atan2((fin.basechord/2 + (x2-fin.topchord/2)),fin.h);
    fin.l_m = fin.h/cos(fin.sweepc);
    clear x1 x2;
    
    %A_planform
    cone.A_plan = 2/3 * L_cone * roro.D;
    cyl.A_plan = D_cyl* L_cyl;
    fin.A_plan1 = 6*(0.5 * fin.l_forward * fin.h);
    fin.A_plan2 = 6*(fin.topchord^2);
    fin.A_plan3 = 6*(0.5 * fin.l_backward * fin.h);

    %F_ratio = L / D_cyl; <-clean 
    
    %% Cn_alpha and CoP Momentum of Planform Area
    %Todo
    A_plan = cone.A_plan + cyl.A_plan + fin.A_plan1 + fin.A_plan2 + fin.A_plan3;
    cone.X_plan = 5/8 * L_cone;
    cyl.X_plan = L_cone + L_cyl/2;
    fin.X_plan1 = fin.X_b + 2/3 * fin.l_forward;
    fin.X_plan2 = fin.X_b + fin.l_forward + 0.5*fin.topchord;
    fin.X_plan3 = fin.X_b * fin.basechord - 2/3 * fin.l_backward;
    Xcp_Planform = (cone.A_plan * cone.X_plan + cyl.A_plan * cyl.X_plan...
        + fin.A_plan1 * fin.X_plan1 + fin.A_plan2 * fin.X_plan2 + fin.A_plan3 * fin.X_plan3) / A_plan;
    
    %% Cn_alpha and CoP: Classic Barrowman (According to Box S. et al, 2009)
    % Cone (See Box S., 2009, p9-10)
    cone.Cn_alpha = 2; 
    cone.Xcp = 0.466 * L_cone;
    %Fins (See Box S., 2009, p10)
    K_bf = 1+(roro.D/2)/(fin.h+roro.D/2);
    fin.Cn_alpha = K_bf * 4 * fin.n *(fin.h/roro.D)^2 / ...
        (1+sqrt(1+(2*fin.l_m / (fin.topchord+fin.basechord))^2));
    fin.Xcp = fin.X_b + fin.l_m * (fin.basechord + 2*fin.topchord) / (3*(fin.basechord + fin.topchord))...
        + (fin.basechord + fin.topchord - (fin.topchord*fin.basechord)/(fin.basechord + fin.topchord))/6;
    
    % Cn_alpha according to classic Barrowman (See Box S., 2009, p10)
    Cn_alpha_Barrow = cone.Cn_alpha + fin.Cn_alpha;
    Xcp_Barrow = (fin.Cn_alpha*fin.Xcp + cone.Cn_alpha*cone.Xcp) / Cn_alpha_Barrow;
    
    %% Cn_alpha and CoP: Extended Barrowman Body with  Lift correction (According to Galejs R., p.1-4)
   
    alpha = roro.alpha; % Angle of attack
    K=1.1;
    if (alpha == 0)
        alpha = 0.00001;
    end
    
    % correction for compressible flow 
    beta = sqrt( 1 - M^2); % M <1
    
    % Cone (See Galejs R., p.1-2)
    cone.Cn_correction = K * cone.A_plan/A_ref * alpha; 
    cone.Xcp_correction = 5/8 * L_cone; 
    % Cylinder (See Galejs R., p.2)
    cyl.Cn_correction = K * cyl.A_plan/A_ref * alpha;
    cyl.Xcp_correction = L_cone + L_cyl/2;
    % Fins
    fin.Cn_correction = 0;
    fin.Cn_alpha = K_bf * 4 * fin.n *(fin.h/roro.D)^2 / ...
        (1+sqrt(1+(2*beta*fin.l_m / (fin.topchord+fin.basechord))^2));
    
    % Cn_alpha according to extended Barrowman (See Galejs R., 2)
    Cn_alpha = cone.Cn_alpha + cone.Cn_correction + cyl.Cn_correction + fin.Cn_alpha + fin.Cn_correction;
    % Cn_alpha = Cn_alpha/beta; % Correction for compressible fluid
    Xcp = (fin.Cn_alpha*fin.Xcp + cone.Cn_alpha*cone.Xcp + cone.Cn_correction*cone.Xcp_correction + cyl.Cn_correction*cyl.Xcp_correction) / Cn_alpha;

    %% Roll damping 
    % % omega = deg2rad(140);
    % % Cn_alpha0 = 2*pi/beta; % from potential flow over a thin foil. 
    % % 
    % % temp = (fin.basechord+fin.topchord)*R_cyl^2*fin.h/2 + (fin.basechord+2*fin.topchord)*R_cyl*fin.h^2/3  + (fin.basechord+3*fin.topchord)*fin.h^3/12; 
    % % Cld = fin.n*Cn_alpha0/(A_ref*v0*D_cyl) * omega * temp;
    
    %% Pitch Damping  [Nm/s]

    % Corrective moment coefficient c1
    % (ApogeeRockets newsletter 193, p3)
    c1 = (rho/2 * V^2 * roro.A_ref * Cn_alpha) * (Xcp-roro.Xcm); %Corrective Moment coeff

    % Damping moment coefficient c2
    % Aerodynamic damping moment coeff (See ApogeeRockets newsletter 195, p.2-4)
    c2_a = (rho/2 * V * roro.A_ref) * ((cone.Cn_alpha*(cone.Xcp - roro.Xcm)^2)+(fin.Cn_alpha*(fin.Xcp - roro.Xcm)^2));
    % Jet damping moment coeff (See Box S., 2009, p.5)
    l_cn = roro.Length - Xcp;
    l_cc = roro.Xcm_prop - Xcp;
    c2_r = roro.deltaMass * (l_cn^2 - l_cc^2);
    
    c2 = c2_a + c2_r; % Damping Moment coeff

    % Damping ratio 
    % (See ApogeeRockets newsletter 197, p2)
    I_longitudinal = roro.Ibody_dry(2,2);
    Cda = c2/(2*sqrt(c1*I_longitudinal));
    
end

