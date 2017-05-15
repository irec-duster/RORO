
function zeta = damping(roro)
    
    global env

    %Air density
    rho =  env.rho; 
    %Velocity
    V =  260;
    %Run Script cop to determin Cn_alpha and Xcp for all components
    cop;
    %Length from noXcpXcple to tip of nosecone
    L_ne = roro.Length;
    %Massflow
    m_dot = 0;
    %Distance CG to tip of nosecoe
    W = roro.Xcm;
    %Reference area of attack (nc base area)
    A_ref = roro.A_ref;
    %Longitudinal (Pitch) Moment of Inertia around COG in kg/m^2
    %Value taken from Patricks Post on Slack 11th of May
    I_l = 9.756;

    %Corrective moment coefficient c1
    %(AR newsletter 193, p3)
    c1 = (rho/2 * V^2 * A_ref * Cn_alpha) * (Xcp-W)

    %Damping moment coefficient c2
    %(AR newsletter 195, p2-4)
    %aerodynamic damping moment coeff
    c2_a = (rho/2 * V * A_ref) * ((cone.Cn_alpha*(cone.Xcp - W)^2)+(cyl.Cn_alpha*(cyl.Xcp - W)^2)+(fin.Cn_alpha*(fin.Xcp - W)^2));
    %trhust damping moment coeff
    c2_r = m_dot * (L_ne - W)^2;
    
    c2 = c2_a + c2_r

    %Damping ratio 
    %(AR newsletter 197, p2)
    zeta = c2/(2*sqrt(c1*I_l));
end