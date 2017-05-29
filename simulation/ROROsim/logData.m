function logData(alpha, Cd, Cda, Xcm, Mass, Vmag, Xcp, zeta, Ssm, t)

    global log

    %log = [log;[Xdot, Xstab, rad2deg(alpha),t]];
    log = vertcat(log,[alpha, Cd, Cda, Xcm, Mass, Vmag, Xcp, zeta, Ssm, t]);
%     if(t==0)
%         file1 = fopen('log.dat','w');
%     else
%         file1 = fopen('log.dat','a');
%     end
%     input = ('%f,%f,%f,%f,%f,%f \n');
%     fprintf(file1,input,[Xdot, Xstab, rad2deg(alpha),t]);
%     fclose(file1);

    % if(t==0)
    %     save('log',X, Xdot, accel, alpha,t);
    % else
    %     save('log',X, Xdot, accel, alpha,t,'-append');
    % end
    % 
    % 
    % end
end

