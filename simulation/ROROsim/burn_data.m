function burn_data( roro )
    % Burn data function is executed at the start of each iteration in the
    % solver. This updates the motor mass and inertias by assuming the
    % impulse generated is proportional to the mass consumed
    t= roro.time;
    T=roro.T; % Thrust 
    tt=roro.motordata(:,1);
    TT=roro.motordata(:,2);
    ind= find(tt<t); % Finds index less then curretn time
    ind=ind(end);
    tt =[tt(1:ind); t]; % Burn uptill 
    TT= [TT(1:ind); T];
    roro.impulseGen =  trapz(tt,TT); % impulse generated upto that point.
    
    propM_used = roro.propM_tot/roro.Motor_impulse*roro.impulseGen;
    roro.propM_current = roro.propM_tot  - propM_used; % Remaining prop mass
    if(roro.deltat == 0)
        roro.deltaMass =0 ;
    else
        roro.deltaMass = (roro.propM_current - roro.propM_prev)/ roro.deltat;
    end
    roro.propM_prev = roro.propM_current;
    
    % New Inertias at time step
    % IMP:The mass of the prop should already be updated in roro to get correct Xcm 
    
    d = roro.Xcm_prop - roro.Xcm; % distance from Cm of propellent and??? 
    prop_OD = roro.prop_OD;
    prop_h = roro.prop_h;
    Mass_prop = roro.propM_current;
    prop_density = roro.prop_density;
    prop_ID = sqrt(prop_OD^2 -4*Mass_prop/(pi*prop_h*prop_density ));
    
    propIx = 0.5*Mass_prop*(prop_OD^2+prop_ID^2)/4;
    propIy = Mass_prop/12*(3*(prop_ID^2+prop_ID^2)/4 + prop_h^2) + roro.propM_current*(d);
    roro.Iprop = [propIx, 0, 0; 0, propIy, 0; 0, 0, propIy]; % propIy = propIz 
    
end

