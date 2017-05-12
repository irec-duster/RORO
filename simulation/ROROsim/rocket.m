%% Rocket  

classdef rocket <handle
   properties
      Length
      D
      Cone_L
      fin_n
      fin_h
      fin_base
      fin_top
      fin_sweep
      fin_t
      Mass_dry
      Ibody
      Xcm
      motorname
      motordata
      A_ref %Clean this up?
      Mass_propellent
      Mass_motor
      Motor_impulse
      
      % Current State Vector with Initial values f
      time = 0;         %time
      X = [0; 0; 0];    % Position x, y, z   
      Q = [0; 0; 0; 0]; % Angle in quarternions  
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
                obj.Ibody= [prop(11), 0 ,0; 0 , prop(12), 0; 0, 0, prop(13)];
                obj.Xcm = prop(14);
                obj.motorname=motorname;
                obj.A_ref = (pi*obj.D^2/4);
             %else
                %error('Value must be numeric')
             %end
          end
       end
       %Fuctions to calcualte various properties

       function Cd = Cd(obj) % Drag in axial direction
           Cd = Cd_mandell(obj);
           if (isinf(Cd) || Cd > 10)
               Cd =10;
           end
       end
       function CnXcp = CnXcp(obj) % Normal force and Cop location
           [Cn_alpha, Xcp]=Cn_alphaXcp(obj);
           CnXcp = [Cn_alpha*obj.alpha Xcp];
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
               obj.Mass_propellent =0;
           end
           Mass= obj.Mass_dry + obj.Mass_motor + ...
               obj.Mass_propellent*(obj.Motor_impulse-burn_data(obj));
           
       end
   end
end