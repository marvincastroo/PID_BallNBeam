t = out.Sistema.Time;
y = out.Sistema.Data(:,1);
u = out.Sistema.Data(:,2);
r = out.Sistema.Data(:,3);

figure(1)
plot( t, y, t, r, t, u)
legend('y', 'r')
xlabel('tiempo (s)')
ylabel('magnitud')

IAEr =trapz(t,abs(r-y))%IAE
TVur = sum(abs(diff(u)))%TVur
Umax = max(abs(max(u)),abs(min(u)))