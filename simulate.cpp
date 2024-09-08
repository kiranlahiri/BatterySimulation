#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <iomanip>


std::vector<std::pair<double,double> > c1;
std::vector<std::pair<double,double> > c2;
std::vector<std::pair<double,double> > r0;
std::vector<std::pair<double,double> > r1;
std::vector<std::pair<double,double> > r2;
std::vector<std::pair<double,double> > ocv;
std::vector<std::pair<double,double> > power; //First column is delta t (s), second column is Power (Wh)
double Q_n = 3039.12; // for 18650-25R 9P36S, change for each case
double P_diss = 0.0;
double E_diss = 0.0;
/*
PARAMETER FLAGS FOR READING IN CSV
C1 = 0
C2 = 1
R0 = 2
R1 = 3
R2 = 4
OCV = 5
power = 6
*/

double Calc_SOC(double prev_SOC,double Current, double delta_t)
{
    double new_SOC = prev_SOC + Current/Q_n * delta_t;
    return new_SOC;

}

double Calc_V1(double Prev_v1, double R1, double I_L, double C_1, double delta_t)
{
    double new_V1 = Prev_v1 + delta_t * (-1 * prev_v1/R1 + I_L)/C_1;
    return new_v1;
}

double Calc_V2(double Prev_v2, double R2, double I_L, double C_2, double delta_t)
{
    double new_V2 = Prev_v2 + delta_t * (-1 * prev_v2/R2 + I_L)/C_2;
    return new_v2;
}

double Calc_IL(double R0, double OCV, double V1, double V2, double IL, double P){
    double I_L1 = (-1*(OCV-V1-V2) + math.sqrt(math.abs((OCV-V1-V2)^2 - 4 * (R0 * P))))/-2*R0;
    double I_L2 = (-1*(OCV-V1-V2) - math.sqrt(math.abs((OCV-V1-V2)^2 - 4 * (R0 * P))))/-2*R0;

    if((I_L1 < 0 && P < 0) && (I_L2 > 0)){
        return I_L1;
    }

    if((I_L2 < 0 && P < 0) && (I_L1 > 0)){
        return I_L2;
    }

    if((I_L1 > 0 && P > 0) && (I_L2 < 0)){
        return I_L1;
    }

    if((I_L2 > 0 && P > 0) && (I_L1 < 0)){
        return I_L2;
    }

    double V_T1 = P/I_L1;
    double V_T2 = P/I_L2;


    if(V_T1 == OCV-V1-V2-I_L1*R0){
        return IL_1;
    }
    else{
        return IL_2;
    }
}

void run_sim(){
    /*
    for first time step
    V1 = 0;
    v2 = 0;
    P_out = 0;
    t = 0
    SOC = 1;

    double I_L = Calc_I();
    
    while(until we reachc end of drive cycle){
        SOC = Calc SOC(SOC, I_t,delta t)
        Find R-2RC Values
        V1 = Calc_V1;
        V2= Calc_v2;
        I_L = Calc_I_L;
        
        P_diss += V1^2/R1 + V2^2/R2 + I_L^2*rR0
        E_diss += P_diss*delta_t
    }
    
    
    */

}

void read_parameters(std::string parameter_csv, int parameter_flag)
{
    std::ifstream parameter_file(parameter_csv);
    if(!parameter_file.is_open())
    {
        throw std::runtime_error("Could not open file");
    }
    
    std::vector<std::pair<double,double> > name_of_vector;
    std::string line;
    std::string first_val, second_val;
    std::string trash;

    if(parameter_file.good())
    {
        while(std::getline(parameter_file,line))
        {
            std::stringstream ss(line);
            std::getline(ss,first_val,',');
            std::getline(ss,second_val);

            double first = atof(first_val.c_str());
            double second = atof(second_val.c_str());

            std::pair<double, double> csv_pair;
            csv_pair  = std::make_pair(first,second);

            switch (parameter_flag)
            {
                case 0:
                    c1.push_back(csv_pair);
                    break;
                case 1:
                    c2.push_back(csv_pair);
                    break;
                case 2:
                    r0.push_back(csv_pair);
                    break;
                case 3:
                    r1.push_back(csv_pair);
                    break;
                case 4:
                    r2.push_back(csv_pair);
                    break;
                case 5:
                    ocv.push_back(csv_pair);
                    break;
                case 6
                    power.push_back(csv_pair)
            }
            
        }
    }

    parameter_file.close();
    if(parameter_file.is_open())
    {
        throw std::runtime_error("File not closed properly");
    }
}



int main(int argc, char** argv)
{

    //read_parameters("SOC_C1_INR_18650_25R.csv", 0);
    //read_parameters("SOC_C2_INR_18650_25R.csv", 1);
    //read_parameters("SOC_R0_INR_18650_25R.csv", 2);
    read_parameters("SOC_R1_INR_18650_25R.csv", 3);
    //read_parameters("SOC_R2_INR_18650_25R.csv", 4);
    //read_parameters("Samsung18650_25R DataDC_OCV_Curve.csv", 5);

   for(int i = 0; i < r1.size(); ++i)
    {
        std::cout << std::setprecision(15) << r1[i].first << ", " << r1[i].second << std::endl;
    }
    
    return 0;
}
