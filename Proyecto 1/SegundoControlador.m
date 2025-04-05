% Identificación del Sistema
data = load("Curva.txt")

t = data(:,1);
y = data(:,2);
u = data(:,3);

figure(1)
plot(t,u,'b',t,y,'k');
legend('salida','entrada');

yn = y - mean(y(1:10));
un = u - mean(u(1:10));

figure(2)
plot(t,un,'r',t,yn,'g')
systemIdentification


% Definir Planta y Controlador 
s = tf('s');
Ps = (-0.17445)/(0.013864*s+1);
Cs = (-1.1983*(0.203*s+1))/(0.203*s);
sisotool
