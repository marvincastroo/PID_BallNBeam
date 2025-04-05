clc
% Define the end time
end_time = 50;

% Define the number of elements
num_elements = 500;

% Calculate the step size
step_size = end_time / (num_elements - 1);

% Create the time vector
t = [0:0.1:19.8]

data = readtable("andnew.xlsx");
ukm = data(:, 2);
y = data(:, 3);
r = data(:, 4);

r1 = r{293:491, 1}
u1 = ukm{293:491, 1}
y1 = y{293:491, 1}
size(t)
size(y1)

%t = readtable("resultados2.xlsx")
figure(1)
%plot(t, u1, t, y1, t, r1)
%legend('u','y', 'r')
plot( t, y1, 'k' , t, r1, 'r')
legend('y', 'r')
xlabel('tiempo (s)')
ylabel('magnitud')

IAEr =trapz(t,abs(r1-y1))%IAE
TVur = sum(abs(diff(u1)))%TVur
Umax = max(abs(max(u1)),abs(min(u1)))