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
      fin_angle
      fin_t
      Mass
      Ibody
      
      % Current State Vector with Initial values f
      X = [0 0 0];    % Position x, y, z   
      Q = [0 ,0, 0, 0]; % Angle in quarternions  
      P = [0 0 0];      % Linear Momentum  
      L = [0 0 0];      % Angular momentum
      Xdot = [0 0 0];    % Velocity xdot, ydot, zdot 
      Qdot = [0 ,0, 0, 0];% Angular rates
      Pdot = [0 0 0];   % Linear Momentum rates = applied force
      Ldot = [0 0 0];   % Angular Momentum rates= applied torques
      alpha = 0;      
          
     
   end
   methods
       %Constructor takes data from rocket property file
       function obj = rocket(val) 
          if nargin > 0
             %if isnumeric(val)
                obj.Length = val(1);
                obj.D = val(2);
                obj.Cone_L = val(3);
                obj.fin_n = val(4);
                obj.fin_h = val(5);
                obj.fin_base = val(6);
                obj.fin_top = val(7);
                obj.fin_angle = deg2rad(val(8));
                obj.fin_t = val(9);
                obj.Mass = val(10);
                obj.Ibody= [val(11), 0 ,0; 0 , val(12), 0; 0, 0, val(13)];
             %else
                %error('Value must be numeric')
             %end
          end
       end
       %Fuctions to calcualte various properties
%        function set.x(obj,value)
%          if (value > 0)
%             obj.x = value;
%          else
%             error('Property value must be positive')
%          end 
%        end
       function Cd = Cd(obj) % Drag in axial direction
           Cd = 0.1; % Cd_calc(Re, alpha, M)           
       end
       function CnXcp = CnXcp(obj) % Drag in axial direction
           Cn_alpha = 0.1;
           Xcp = 1.7;%Cd_cal(Re, alpha, M) 
           CnXcp = [Cn_alpha*obj.alpha Xcp];
       end
       function a = a(obj) % test
            obj.alpha = 2;
       end

   end
end