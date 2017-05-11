function current_imp  = burn_data( roro )

    t=roro.time;
    T=roro.T;
    tt=roro.motordata(:,1);
    TT=roro.motordata(:,2);
    ind= find(tt<t); % Finds index less then curretn time
    ind=ind(end);
    tt =[tt(1:ind); t]; % Burn uptill 
    TT= [TT(1:ind); T];
    current_imp =  trapz(tt,TT);

end

