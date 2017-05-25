   
    t= roro.motordata(:,1);
    F=roro.motordata(:,2);
    xev = 3*linspace(0,1,300);
    spl = spline(t,F);
    plot(t,ppval(spl,t));
    % integrate the spline
    spl.coefs = spl.coefs*[diag(1./[4 3 2 1]'),zeros(4,1)];
    spl.order = 5;
    %%
    dx = diff(spl.breaks)';
    C = spl.coefs(:,1);
    for i = 1:4
      C = C.*dx + spl.coefs(:,i+1);
    end
    spl.coefs(2:end,5) = cumsum(C(1:(end-1)));
    xev = t;
    splint = ppval(spl,xev);
    plot(xev,splint)
