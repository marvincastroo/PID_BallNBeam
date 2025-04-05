%% Primer controlador PI
clear all;
close all;
clc
% Respuesta del sistema en lazo cerrado
s = tf ('s') ;
P = -0.174445/(1 +0.013864*s);
C = -1.983*(1 + 1/(0.0138*s))
t = [0:0.1:5];
Myr = (C*P)/(1 + C*P);
r = 0;
r (t >= 1) = 1;
yr = lsim(Myr, r, t);

figure (1) ;
title ('Respuesta del sistema como Servocontrol') ;
xlabel ('Tiempo (s)') ;
ylabel ('Respuesta del sistema ') ;
hold on ;
plot (t , yr) %grafica la salida de servo control 

% Respuesta en sisotool
L_siso = -12.64/s
sisotool(L_siso)

% Discretización del controlador
Cz = c2d(C, 0.1, "tustin")



