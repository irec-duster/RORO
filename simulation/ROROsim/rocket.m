%% Rocket  
% Defines all the properties of the rocket. The dynamics are continuously
% updated, any varibales need for functions should come from this class
classdef rocket <handle
   properties
      % Rocket Characteristics, These do not change during flight
      Length
      D
      Cone_L
      fin_n
      fin_h
      fin_base
      fin_top
      fin_sweep
      fin_t
      Mass_dry          %Rocket and motor housing no prop
      Ibody_dry         %Rocket and motor housing no prop
      Xcm_dry           %Rocket and motor housing no prop
      Rail = 5.5;
      L_pinDia
      L_pinH
      A_ref %Clean this up?
      
      %Motor Characteristics: Updated in each itteration fh the ascent_calc     
      motorname
      motordata
      Mass_motor
      Motor_impulse         % Total Ipulse of motor
      propM_tot             % Total mass of prop
      Iprop                 % Inertia matrix of prop wrt Cg
      Xcm_prop              % Center of mass of the prop
      prop_OD
      prop_ID
      prop_h
      prop_density
      
      deltat                    % Size of time step calcualted in ascent_calc
      deltaMass = 0;            % initally zero
      impulseGen                % Impulse generated upto that point 
      propM_current             % Remaning prop mass
      propM_prev = 0;           % Mass for previous time step to calcualte deltaMass
           
      % Current State Vector with Initial values, Updated in ascent_calc
      time = 0;         %time
      X = [0; 0; 0];    % Position x, y, z   
      Q = [1; 0; 0; 0]; % Angle in quarternions  
      P = [0; 0; 0];      % Linear Momentum  
      L = [0; 0; 0];      % Angular momentum
      Xdot = [0; 0; 0];    % Velocity xdot, ydot, zdot 
      Qdot = [0; 0; 0; 0];% Angular rates
      Pdot = [0; 0; 0];   % Linear Momentum rates = applied force
      Ldot = [0; 0; 0];   % Angular Momentum rates= applied torques
      alpha = 0;      
          
     
   end
   methods
       %Constructor takes data from rocket property file
       function obj = rocket(val) %Val is the table of properties val2 motor data
          if nargin > 0
              prop=table2array(val(1:end-1,2));
              motorname=table2array(val(end,1));
             %if isnumeric(val)
                obj.Length = prop(1);
                obj.Cone_L = prop(2);
                obj.D = prop(3);
                obj.fin_n = prop(4);
                obj.fin_h = prop(5);
                obj.fin_base = prop(6);
                obj.fin_top = prop(7);
                obj.fin_sweep = deg2rad(prop(8));
                obj.fin_t = prop(9);
                obj.Mass_dry = prop(10);
                obj.Ibody_dry = [prop(11), 0 ,0; 0 , prop(12), 0; 0, 0, prop(13)];
                obj.Xcm_dry = prop(14);
                obj.L_pinDia = prop(15);
                obj.L_pinH = prop(16);
                obj.motorname=motorname;
                obj.A_ref = (pi*obj.D^2/4);
                % test heading 84deg
                Ra = -0.1047;                
                Rax = [0.1684;    0.985;         0];
                obj.Q=[cos(Ra/2) sin(Ra/2)*Rax(1) sin(Ra/2)*Rax(2) sin(Ra/2)*Rax(3)]';
             %else
                %error('Value must be numeric')
             %end
          end
       end
       %Fuctions to calcualte various properties

       function Cd = Cd(obj) % Drag in axial direction
           Cd = Cd_mandell(obj);
           if (isinf(Cd) || Cd > 1)
               Cd =1;
           end
       end
       
       function CnXcp = CnXcp(obj) % Normal force and Cop location
           [Cn_alpha, Xcp, Cda]=Cn_alphaXcp(obj);
           CnXcp = [Cn_alpha*obj.alpha, Xcp, Cda];
       end
       
       function T = T(obj) % Thrust curve
           M = obj.motordata;
           T = interp1(M(:,1),M(:,2),obj.time,'spline');
           if ( obj.time > M(end,1))
               T = 0;
           end
       end
       
       function Re = Re(obj) % Re of rocket
           global env
           Re = env.rho*norm(obj.Xdot)*obj.Length/env.mu;
           
       end
       
       function Mass = Mass(obj) % Current mass of rocket
           M = obj.motordata;
           if ( obj.time > M(end,1)); % To assure it goes to zero incase of integartion error
               obj.propM_current =0;
           end
           Mass= obj.Mass_dry + obj.propM_current;               
       end
       
       function Xcm = Xcm(obj) % Current Center of mass of rocket
           M = obj.motordata;
           if ( obj.time > M(end,1)); % To assure it goes to zero incase of integartion error
               Xcm = obj.Xcm_dry;
           end
           Xcm= obj.Xcm_dry*obj.Mass_dry + obj.Xcm_prop*obj.propM_current; 
           Xcm = Xcm/obj.Mass;
       end
       
       function Ibody = Ibody(obj) % Current Inertia of the rocket
           M = obj.motordata;
           if ( obj.time > M(end,1)); % To assure it goes to zero incase of integartion error
               Ibody = obj.Ibody_dry;
           end
           Ibody = obj.Ibody_dry + obj.Iprop;
       end
   end
end