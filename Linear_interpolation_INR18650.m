T = readtable("Samsung 18650_25R Parameters - Sheet1");

SOC = T.SOC
R0 = T.R0
R1 = T.R1
R2 = T.R2
C1 = T.C1
C2 = T.C2

x_interp = linspace(0,1,10000);
R0_interp=interp1(SOC,R0,x_interp,'linear','extrap');

R1_interp=interp1(SOC,R1,x_interp,'linear','extrap');



R2_interp=interp1(SOC,R2,x_interp,'linear','extrap');


C1_interp=interp1(SOC,C1,x_interp,'linear','extrap');

C2_interp=interp1(SOC,C2,x_interp,'linear','extrap');


SOC_R0 = [x_interp; R0_interp];
SOC_R1 = [x_interp; R1_interp];
SOC_R2 = [x_interp; R2_interp];
SOC_C1 = [x_interp; C1_interp];
SOC_C2 = [x_interp; C2_interp];

writematrix(SOC_R0,"SOC_R0_INR_18650_25R.csv");
writematrix(SOC_R1,"SOC_R1_INR_18650_25R.csv")
writematrix(SOC_R2,"SOC_R2_INR_18650_25R.csv")
writematrix(SOC_C1,"SOC_C1_INR_18650_25R.csv")
writematrix(SOC_C2,"SOC_C2_INR_18650_25R.csv")