function [Cn_alpha, Xcp, Xcp_Barrow, Cda] = Cn_alphaXcp(roro) 
    % Takes rocket handle and environment  to calculate Cn, location of cop
    
    % using barrowman implemented in OpenRocket
    global env

    rho =  env.rho; % density 
    mu =  env.mu;  % dynamic viscosity 
    C = env.C; %speed of sound dry air 15C sea level
    V = norm(roro.Xdot,2);   %ms-1 Mag of characteristic velocity at center of pressure location 
    M = V/C;
    Re  = roro.Re;

    % correction for compressible flow 
    beta = sqrt( 1 - M^2); % M <1

    % Rocket dimentions 
    L = roro.Length; 
    L_cone = roro.Cone_L;
    L_cyl = L - L_cone;
    D_cyl = roro.D;
    R_cyl = D_cyl/2;


    %A_ref
    A_ref = pi*R_cyl^2;

    %F_ratio = L / D_cyl; <-clean


    %%
    R_ogive = (L_cone^2+ D_cyl^2/4)/D_cyl;
    %A_wet <-clean
    % 
    % fun = @(x) 2*pi*(sqrt(R_ogive^2 - power((L_cone - x),2))+R_cyl-R_ogive);
    % 
    % A_wet =  integral(fun, 0, L_cone);
    % 
    % A_wet = A_wet + 2*pi*R_cyl*L_cyl; 

    % Cone Vol

    fun2 = @(x) pi*(power((sqrt(R_ogive^2 - power((L_cone - x),2))+R_cyl-R_ogive),2));
    cone.vol =  integral(fun2, 0, L_cone);

    %A_plan
    temp_theta = atan(L_cone/(R_ogive-R_cyl));

    cone.A_plan = 2/3 * L_cone * roro.D;

    cyl.A_plan = D_cyl* L_cyl;
    
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

    % mid chord sweep
    x1 = fin.h*tan(fin.sweep);
    x2 = x1 + fin.topchord - fin.basechord;
    fin.sweepc = atan2((fin.basechord/2 + (x2-fin.topchord/2)),fin.h);
    fin.l_m = fin.h/cos(fin.sweepc);
    
    %% Cn_alpha and CoP Momentum of Planform Area
    %Todo
    
    %% Cn_alpha and CoP Barrowman (According to Box S. et al, 2009)
    % Cone
    cone.Cn_alpha = 2;
    cone.Xcp = 0.466 * L_cone;
    %Fins
    K_bf = 1+(roro.D/2)/(fin.h+roro.D/2);
    fin.Cn_alpha = K_bf * 4 * fin.n *(fin.h/roro.D)^2 / ...
        (1+sqrt(1+(2*fin.l_m / (fin.topchord+fin.basechord))^2));
    fin.Xcp = fin.X_b + fin.l_m * (fin.basechord + 2*fin.topchord) / (3*(fin.basechord + fin.topchord))...
        + (fin.basechord + fin.topchord - (fin.topchord*fin.basechord)/(fin.basechord + fin.topchord))/6;
    
    Cn_alpha_Barrow = cone.Cn_alpha + fin.Cn_alpha;
    Xcp_Barrow = (fin.Cn_alpha*fin.Xcp + cone.Cn_alpha*cone.Xcp) / Cn_alpha_Barrow;
    
    %% Cn_alpha Extended Barrowman Body Lift correction (According to Galejs R.)
    % Cn_alpha Extended Barrowman
    alpha = roro.alpha;
    K=1.1;
    if (alpha == 0)
        alpha = 0.00001;
    end
    
    % Cone
    cone.Cn_correction = K * cone.A_plan/A_ref * alpha;
    cone.Xcp_correction = 5/8 * L_cone;
    % Cylinder
    cyl.Cn_correction = K * cyl.A_plan/A_ref * alpha;
    cyl.Xcp_correction = L_cone + L_cyl/2;
    
    Cn_alpha = cone.Cn_alpha + cone.Cn_correction + cyl.Cn_correction + fin.Cn_alpha;
    Cn_alpha = Cn_alpha; % Correction for compressible fluid
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
    c1 = (rho/2 * V^2 * roro.A_ref * Cn_alpha) * (Xcp-roro.Xcm);

    % Damping moment coefficient c2
    % (AR newsletter 195, p2-4)
    % Aerodynamic damping moment coeff
    c2_a = (rho/2 * V * roro.A_ref) * ((cone.Cn_alpha*(cone.Xcp - roro.Xcm)^2)+(fin.Cn_alpha*(fin.Xcp - roro.Xcm)^2));
    % Jet damping moment coeff
    c2_r = roro.deltaMass * (roro.Length - roro.Xcm)^2;
    
    c2 = c2_a + c2_r;

    % Damping ratio 
    % (AR newsletter 197, p2)
    Cda = c2/(2*sqrt(c1*roro.Iyy));
    
end

