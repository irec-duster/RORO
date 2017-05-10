%% Environment  

classdef environement<handle
   properties
        gamma = 1.4;
        R = 287;
        Ref_Temp = 291.15;      % Ref temp for mu
        Ref_Dyn_viscosity = 1.827e-5;
        Sutherlands_c = 120;
        Earth_M = 5.97237e24;
        Earth_R = 6378000;      % Radius at equator (m)
        G = 6.67408e-11;        % Gravatational constant
        P_sea = 1.01325e5;      % Pressure at sealevel
        rho_sea = 1.225;        % Air density at sealevel
        Temp_grad = 0.0065;     % Temperature gradient troposphere(K/m)
        YA0 = [1, 0, 0];
        PA0 = [0, 1, 0];
        RA0 = [0, 0, 1];
        h_g = 0;                % Ground height above sealevel (m)
        Pressure_g = 1.01325e5  % Ground Pressure (Default:sealevel)
        Temp_g = 288.16;        % Ground temp (Default:sealevel (15C)) (K)
        rho_g = 1.225;
        
   end
   methods
       function obj = environement(val1, val2, val3)
          if nargin > 0
             if isnumeric(val1 && val2 && val3)
                obj.h_g = val1;
                obj.Temp_g = val2+273.15;
                obj.Pressure_g = val3;
                %obj.rho_g =  build to update rho at h_g 
             else
                error('Enter numeric elevation(m) Temperature(C)and Pressure(Pa)')
             end
          end
       end

      function g = g(obj)% Calculates g at current altitude
          global roro
          g = obj.G*obj.Earth_M/(obj.Earth_R+ roro.X(3) + obj.h_g)^2;
      end

      function Temp = Temp(obj) % Calculates temperature at current altitude
          global roro
          Temp = -obj.Temp_grad*(roro.X(3))+obj.Temp_g();
      end
      function mu = mu(obj) % Calculates mu at current altitude
          mu = obj.Ref_Dyn_viscosity*(obj.Sutherlands_c + obj.Ref_Temp)/...
               (obj.Sutherlands_c + obj.Temp)*...
               (obj.Temp/obj.Ref_Temp)^(3/2);
      end
      function Pressure = Pressure(obj) % Calculates air density at current altitude
          n = (obj.g/(obj.Temp_grad*obj.R));
          Pressure = obj.Pressure_g*(obj.Temp/obj.Temp_g)^n;  %  alternate eq https://www.mide.com/pages/air-pressure-at-altitude-calculator
              
      end
      function rho = rho(obj) % Calculates air density at current altitude
          n = (obj.g/(obj.Temp_grad*obj.R))-1;
          rho = obj.rho_g*(obj.Temp/obj.Temp_g)^n;
      end
      function C = C(obj) % Calculates speed of sound at current altitude
          C = sqrt(obj.gamma*obj.R*obj.Temp);
      end

   end
end