function [Cn_alpha, Xcp, Cda]=Cn_alphaXcp(roro) 
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

    cone.A_plan = 2*(R_ogive^2*temp_theta/2-((R_ogive-R_cyl)*L_cone)/2);

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

    %clear temp fun fun2

    %% Center of presure
    alpha = roro.alpha;

    % Cone 

    if (alpha ==0)
        alpha = 0.00001;
    end
    K=1.1;
    cone.Cn_correction = K * cone.A_plan/A_ref*sin(alpha)^2;
    cone.Cn_alpha = 2* (A_ref/A_ref)*sin(alpha)/ alpha + cone.Cn_correction/alpha;

    % cylinder 
    % Cn_alpha

    cyl.Cn_correction = K * cyl.A_plan/A_ref*sin(alpha)^2;

    cyl.Cn_alpha =  cyl.Cn_correction/alpha;
    % Fins 


    fin.Cn1_alpha = ((2*pi*fin.h^2)/ A_ref)/...
        (1 + sqrt(1 + (beta*fin.h^2/(fin.area*cos(fin.sweepc)))^2));

    % N fins corrected for body interference n >= 3
    fin.Cn_alpha = (1 + (R_cyl)/(R_cyl+ fin.h))*(fin.Cn1_alpha * fin.n/2*1);

    %% CoP location

    % Cone

    cone.Xcp = (L_cone*(A_ref)-cone.vol)/A_ref;

    % cyl

    cyl.Xcp = L_cone + L_cyl/2;

    %  Fins at 25% mac
    Xt = fin.h/tan(fin.sweep);
    fin.Xcp = fin.X_b + (Xt/3*(fin.basechord +  2*fin.topchord) + 1/6*(fin.basechord+fin.topchord)^2)/(fin.basechord+fin.topchord);


    Xcp = (fin.Cn_alpha*fin.Xcp + cone.Cn_alpha*cone.Xcp +  cyl.Xcp*cyl.Cn_alpha)/(fin.Cn_alpha+cone.Cn_alpha+cyl.Cn_alpha);
    %% Roll damping 
    % % omega = deg2rad(140);
    % % Cn_alpha0 = 2*pi/beta; % from potential flow over a thin foil. 
    % % 
    % % temp = (fin.basechord+fin.topchord)*R_cyl^2*fin.h/2 + (fin.basechord+2*fin.topchord)*R_cyl*fin.h^2/3  + (fin.basechord+3*fin.topchord)*fin.h^3/12; 
    % % Cld = fin.n*Cn_alpha0/(A_ref*v0*D_cyl) * omega * temp;

    %% Pitch Damping  [Nm/s]
    % Jet damping
    lcn = roro.Length - roro.Xcm;
    lcc = roro.Xcm_prop -roro.Xcm;
    Cda_jet = roro.deltaMass *(lcn^2 - lcc^2);
    
    Cda_l = 0.5*rho* V * A_ref *( fin.Cn_alpha*(fin.Xcp-roro.Xcm)^2+...
        cone.Cn_alpha*(cone.Xcp - roro.Xcm)^2 + cyl.Cn_alpha*(cyl.Xcp - roro.Xcm)^2);
    
    Cda  =  Cda_l+ Cda_jet;
    
    %% Cn_alpha
    Cn_alpha = fin.Cn_alpha + cyl.Cn_alpha + cone.Cn_alpha;


end

