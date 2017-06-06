function plotData( clog, roro )

 
    tend = 15;
    t_Burnout = roro.t_Burnout;
    t_RailExit = roro.departureState(2);
    % vel_RailExit = roro.departureState(1);
    
    % Calculate Xcp Barrowman for documentation
    [Xcp_Barrowman, Xcp_Planform, Ssm_Barrowman] = Xcp_Barrowman_f(roro);
    Xcp_B = ones(length(clog),1) * Xcp_Barrowman;
    Xcp_P= ones(length(clog),1) * Xcp_Planform;
    
    %% --- Figure Flight Information ---
    
    figure('Name','Flight information','Position', [800 0 400 1000]);
    subplot(4,1,1)
    plot(clog(:,12),clog(:,6))
    xlabel('Time')
    ylabel('Xdot [m/s]')
    title('Velocity')
    axis([0 tend 0 300])
    line([t_RailExit t_RailExit],[0 300],'Color',[0 1 0])
    line([t_Burnout t_Burnout],[0 300],'Color',[1 0 0])

    subplot(4,1,2)
    plot(clog(:,12),clog(:,5))
    xlabel('Time')
    ylabel('Mass [kg]')
    title('Mass')
    axis([0 tend 20 30])
    line([t_RailExit t_RailExit],[0 300],'Color',[0 1 0])
    line([t_Burnout t_Burnout],[0 300],'Color',[1 0 0])

    subplot(4,1,3)
    plot(clog(:,12),clog(:,2))
    xlabel('Time(s)')
    ylabel('Cd []')
    title('Drag Coefficient')
    axis([0 tend 0 1])
    line([t_RailExit t_RailExit],[0 300],'Color',[0 1 0])
    line([t_Burnout t_Burnout],[0 300],'Color',[1 0 0])

    subplot(4,1,4)
    plot(clog(:,12),clog(:,1))
    xlabel('Time')
    ylabel('alpha [rad]')
    title('Angle of Attack')
    axis([0 tend 0 0.4])
    line([t_RailExit t_RailExit],[0 300],'Color',[0 1 0])
    line([t_Burnout t_Burnout],[0 300],'Color',[1 0 0])

    %% --- Figures Stability Analysis ---
    % COP
    figure('Name','Stability Analysis', 'Position', [0 0 800 1000])
    subplot(3,2,1)
    hold on
    xcp1 = plot(clog(:,12),clog(:,7));
    xcp2 = plot(clog(:,12),Xcp_B);
    xcp3 = plot(clog(:,12),Xcp_P);
    hold off
    xlabel('Time')
    ylabel('Xcp [m]')
    title('Center of Pressure')
    axis([0 tend 1 2])
    line([t_RailExit t_RailExit],[0 300],'Color',[0 1 0])
    line([t_Burnout t_Burnout],[0 300],'Color',[1 0 0])
    legend([xcp1 xcp2 xcp3],'Barrowman+LiftCorrection','Barrowman','Planform','Location','southwest','Orientation','horizontal')
    % COG
    subplot(3,2,2)
    plot(clog(:,12),clog(:,4))
    xlabel('Time')
    ylabel('Xcm [m]')
    title('Center of Mass')
    axis([0 tend 1 1.7])
    line([t_RailExit t_RailExit],[0 300],'Color',[0 1 0])
    line([t_Burnout t_Burnout],[0 300],'Color',[1 0 0])
    % STABILITY MARGIN
    subplot(3,2,3)
    hold on
    ssm1 = plot(clog(:,12),clog(:,9));
    ssm2 = plot(clog(:,12),clog(:,10));
    hold off
    xlabel('Time')
    ylabel('Margin []')
    title('Static Stability Margin')
    axis([0 tend 0 3])
    line([t_RailExit t_RailExit],[0 300],'Color',[0 1 0])
    line([t_Burnout t_Burnout],[0 300],'Color',[1 0 0])
    legend([ssm1 ssm2], 'Barrowman+LiftCorrection','Barrowman','Location','southwest','Orientation','horizontal')
    % DAMPING RATIO
    subplot(3,2,4)
    plot(clog(:,12),clog(:,8))
    xlabel('Time')
    ylabel('Zeta []')
    title('Damping Ratio')
    axis([0 tend 0.0 0.2])
    line([t_RailExit t_RailExit],[0 300],'Color',[0 1 0])
    line([t_Burnout t_Burnout],[0 300],'Color',[1 0 0])
    % C_CORRECTIVE
    subplot(3,2,5)
    plot(clog(:,12),clog(:,11))
    xlabel('Time')
    ylabel('Corrective MomentCoeff [N]')
    title('Corrective Moment Coefficient')
    axis([0 tend 0.0 3500])
    line([t_RailExit t_RailExit],[0 5000],'Color',[0 1 0])
    line([t_Burnout t_Burnout],[0 5000],'Color',[1 0 0])
    % C_DAMPING
    subplot(3,2,6)
    plot(clog(:,12),clog(:,3))
    xlabel('Time')
    ylabel('Damping MomentCoeff [N]')
    title('Damping Moment Coefficient')
    axis([0 tend 0.0 100])
    line([t_RailExit t_RailExit],[0 5000],'Color',[0 1 0])
    line([t_Burnout t_Burnout],[0 5000],'Color',[1 0 0])
    


end

