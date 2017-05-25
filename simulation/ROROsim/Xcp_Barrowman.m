function [Xcp_Barrowman, Xcp_Planform, Ssm_Barrowman] = Xcp_Barrowman( roro )
%Calculate Xcp according to Barrowman for documentation
    %% Rocket Dimensions
    
    L = roro.Length; 
    L_cone = roro.Cone_L;
    L_cyl = L - L_cone;
    D_cyl = roro.D;
    R_cyl = D_cyl/2;
    
    % Fin geometry

    fin.n=roro.fin_n;
    fin.sweep = roro.fin_sweep;
    fin.h = roro.fin_h;
    fin.topchord = roro.fin_top;
    fin.basechord = roro.fin_base;
    fin.l_forward = tan(fin.sweep)*fin.h; % Length of forward pointing triangle
    fin.l_backward = fin.basechord-fin.topchord-fin.l_forward; % length of backward 
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
    fin.l_m = fin.h/cos(fin.sweepc); %midchord length; 
    
    
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
    
    Cn_alpha_Barrow = cone.Cn_alpha + fin.Cn_alpha;
    Xcp_Barrowman = (fin.Cn_alpha*fin.Xcp + cone.Cn_alpha*cone.Xcp) / Cn_alpha_Barrow
    %Static stability amrgin
    Ssm_Barrowman = (Xcp_Barrowman - roro.Xcm)/roro.D
    
    %% CoP: Center of Planform Area
    % A_planform
    cone.A_plan = 2/3 * L_cone * roro.D;
    cyl.A_plan = D_cyl* L_cyl;
    fin.A_plan1 = 6*(0.5 * fin.l_forward * fin.h);
    fin.A_plan2 = 6*(fin.topchord^2);
    fin.A_plan3 = 6*(0.5 * fin.l_backward * fin.h);
    A_plan = cone.A_plan + cyl.A_plan + fin.A_plan1 + fin.A_plan2 + fin.A_plan3;
    % Distances center planform to nose cone tip
    cone.X_plan = 5/8 * L_cone;
    cyl.X_plan = L_cone + L_cyl/2;
    fin.X_plan1 = fin.X_b + 2/3 * fin.l_forward;
    fin.X_plan2 = fin.X_b + fin.l_forward + 0.5*fin.topchord;
    fin.X_plan3 = fin.X_b * fin.basechord - 2/3 * fin.l_backward;
    
    Xcp_Planform = (cone.A_plan * cone.X_plan + cyl.A_plan * cyl.X_plan...
        + fin.A_plan1 * fin.X_plan1 + fin.A_plan2 * fin.X_plan2 + fin.A_plan3 * fin.X_plan3) / A_plan
end

