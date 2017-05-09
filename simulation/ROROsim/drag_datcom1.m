clc; clear all;

%%  Drag using Mandel 


w = length(0:0.5:5);
alphatab = [0:0.5:5];

v0tab= [10:10:250];
h= length(v0tab);
Cd = zeros(h,w);
for i=1:h
    
L = 2.646; %characteristic length
rho =  1.225; % density 
mu =  1.7894e-5;  % dynamic viscosity 
C = 340.3; %speed of sound dry air 15C sea level
v0 = v0tab(i); %150;   %ms-1 characteristic velocity
M = v0/C;
Re  = rho*v0*L/mu;

% Approx laminar flow over rocket

% Rocket dimentions 
L_cone = 0.61;
L_cyl = L - L_cone;
D_cyl = 0.158;
R_cyl = D_cyl/2;

Re  = rho*v0*D_cyl/mu;
%%
R_ogive = (L_cone^2+ D_cyl^2/4)/D_cyl;
%A_ref

fun = @(x) 2*pi*(sqrt(R_ogive^2 - power((L_cone - x),2))+R_cyl-R_ogive);

A_wet =  integral(fun, 0, L_cone);

A_wet = A_wet + 2*pi*R_cyl*L_cyl;

A_ref = pi*R_cyl^2;

F_ratio = L / D_cyl;

%Fin Geometry
fin.n=3;
fin.sweep = deg2rad(45);
fin.h = 11.5e-2;
fin.topchord = 7e-2;
fin.basechord = 21e-2;
fin.t = 1e-2;
fin.a_ref = fin.n*fin.h*fin.t;
fin.area = (fin.topchord+fin.basechord)/2*fin.h;
fin.a_wet = fin.n*2*fin.area;
fin.c = (fin.topchord+fin.basechord)/2;
fin.X_b =  L - fin.basechord; % fin location

l_n = L_cone;

d_n = D_cyl;
d_b = D_cyl;
d_f = D_cyl;
d_d = D_cyl;
l_c  = 0; % no btail
l_b = L_cyl;
Re_c = 5e5;
T_f = fin.t;
l_TR = L;
n=fin.n;
% mid chord sweep

temp.x1 = fin.h*tan(fin.sweep);

temp.x2 = temp.x1 + fin.topchord - fin.basechord;

fin.sweepc = atan2((fin.basechord/2 + (temp.x2-fin.topchord/2)),fin.h);

clear temp fun fun2
l_m = fin.h/acos(fin.sweepc);

A_fe= (fin.topchord+fin.basechord)/2*fin.h;

A_fp = A_fe + 0.5*d_f*fin.basechord;

%%
B = Re_c*(0.074/Re^(0.2) - 1.328/sqrt(Re));

if (Re < Re_c)
    Cf =  1.328/sqrt(Re);
else
    Cf=0.074/Re^(0.2)-B/Re;
end

% 
Re_f  = rho*v0*l_m/mu;

B_f = Re_c*(0.074/Re_f^(0.2) - 1.328/sqrt(Re_f));

if (Re_f < Re_c)
    Cf_f =  1.328/sqrt(Re_f);
else
    Cf_f=0.074/Re_f^(0.2)-B/Re_f;
end
%

Cd_fb = (1 + 60/(l_TR/d_b^3)+0.0025*l_b/d_b)*(2.7*l_n/d_b +4*l_b/d_b + 2*(1-d_d/d_b)*l_c/d_b)*Cf;

Cd_b = 0.029*(d_d/d_b)^3/sqrt(Cd_fb);

Cd_f = 2*Cf_f *(1+2*T_f/l_m)*4*n*A_fp/(pi*d_f^2);

Cd_i = 2*Cf_f*(1+2*T_f/l_m)*4*n*(A_fp-A_fe)/(pi*d_f^2);

Cd0 = Cd_fb + Cd_b + Cd_f + Cd_i;

% compressibility correction
for j=1:w
% Alpha
alpha =deg2rad(alphatab(j));
deltaktab=[4 6 8 10 12 14 16 18 20;0.78 0.86 0.92 0.94 0.96 0.97 0.975 0.98 0.982];
etatab=[4 6 8 10 12 14 16 18 20 22 24;0.6 0.63 0.66 0.68 0.71 0.725 0.74 0.75 0.758 0.77 0.775];
% error in paper
etak=interp1(etatab(1,:),etatab(2,:),F_ratio,'linear','extrap');
deltak=interp1(deltaktab(1,:),deltaktab(2,:),F_ratio,'linear','extrap');
if etak>1;
    etak=1;
end
if deltak>1;
    deltak=1;
end
Cd_b_alpha = 2*deltak*alpha^2 + 3.6*etak*(1.36*l_TR - 0.55*l_n)*alpha^3/(pi*d_b)

Rs = R_cyl/(R_cyl+fin.h);
Kbf=0.8065*Rs^2+1.1553*Rs;
Kfb=0.1935*Rs^2+0.8174*Rs+1;

Cd_f_alpha = (1.2*A_fp*4/(pi*d_f^2) +3.12*(Kfb +Kbf-1)*A_fe*4/(pi*d_f^2))*alpha^2;

Cd = Cd0 + Cd_b_alpha + Cd_f_alpha;


Cdsurf(i,j) = Cd/sqrt(1-M^2)
end
end

surf(alphatab',v0tab',Cdsurf)
xlabel('alpha (deg)')
ylabel('V0 (m/s)')
zlabel('Cd')