#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <iomanip>
#include <math.h>
#include <cmath>

std::vector<std::pair<double,double> > c1;
std::vector<std::pair<double,double> > c2;
std::vector<std::pair<double,double> > r0;
std::vector<std::pair<double,double> > r1;
std::vector<std::pair<double,double> > r2;
std::vector<std::pair<double,double> > ocv_discharge;
std::vector<std::pair<double,double> > power; //First column is delta t (s), second column is Power (Wh)
std::vector<std::pair<double,double> > ocv_charge;
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
power_charge = 7
*/

#define C1_FLAG 0
#define C2_FLAG 1
#define R0_FLAG 2
#define R1_FLAG 3
#define R2_FLAG 4
#define OCV_DISCHARGE_FLAG     5
#define POWER_FLAG   6
#define OCV_CHARGE_FLAG 7

double Calc_SOC(double prev_SOC,double Current, double delta_t)
{
    double new_SOC = prev_SOC + Current/Q_n * delta_t;
    return new_SOC;

}

double Calc_V1(double prev_V1, double R1, double I_L, double C_1, double delta_t)
{
    double new_V1 = prev_V1 + delta_t * (-1 * prev_V1/R1 + I_L)/C_1;
    return new_V1;
}

double Calc_V2(double prev_V2, double R2, double I_L, double C_2, double delta_t)
{
    double new_V2 = prev_V2 + delta_t * (-1 * prev_V2/R2 + I_L)/C_2;
    return new_V2;
}

double Calc_IL(double R0, double OCV, double V1, double V2, double P){
    double I_L1 = (-1*(OCV-V1-V2) + sqrt(abs(pow(OCV-V1-V2,2) - 4 * (R0 * P))))/-2*R0;
    double I_L2 = (-1*(OCV-V1-V2) - sqrt(abs(pow(OCV-V1-V2,2) - 4 * (R0 * P))))/-2*R0;

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
        return I_L1;
    }
    else{
        return I_L2;
    }
}
double return_OCV(double SOC, double power){

        //error checking
    if(SOC > 1){
        SOC = 1;
    }
    else if(SOC < 0){
        SOC = 0;
    }

    //search through the csv file by SOC and find one that equals the calculated SOC by ~0.00005
    //to do this efficiently, use a binary search

    double value = 0.0;
    int low;
    int high;
    int mid

    
    if(power > 0){
         low = 0;
         high = power_charge.size();
            while(low <= high){
                mid = low + (high-low)/2;
                if(std::abs(ocv_charge[mid].first - SOC) < 0.00005){
                    return ocv_charge[mid].second;
                }
                else if(ocv_charge[mid].first < SOC){
                    low = mid + 1;
                }
                else{
                    high = mid + 1;
                }
            }
            throw std::runtime_error("SOC not found")
    }  
    else{
        low = 0;
         high = ocv_discharge.size();
            while(low <= high){
                mid = low + (high-low)/2;
                if(std::abs(ocv_discharge[mid].first - SOC) < 0.00005){
                    return ocv_discharge[mid].second;
                }
                else if(ocv_discharge[mid].first < SOC){
                    low = mid + 1;
                }
                else{
                    high = mid + 1;
                }
            }
            throw std::runtime_error("SOC not found")
    }  
    return -1;
           
        
}
double return_Param(int parameter_flag, double SOC)
{
    //error checking
    if(SOC > 1){
        SOC = 1;
    }
    else if(SOC < 0){
        SOC = 0;
    }

    //search through the csv file by SOC and find one that equals the calculated SOC by ~0.00005
    //to do this efficiently, use a binary search

    double value = 0.0;
    int low;
    int high;
    int mid

    switch(parameter_flag){
        Case C1_FLAG:
            low = 0;
            high = c1.size();
            
            while(low <= high){
                mid = low + (high-low)/2;
                if(std::abs(c1[mid].first - SOC) < 0.00005){
                    return c1[mid].second;
                }
                else if(c1[mid].first < SOC){
                    low = mid + 1;
                }
                else{
                    high = mid + 1;
                }
            }
            throw std::runtime_error("SOC not found")
        break;
        Case C2_FLAG:
            low = 0;
            high = c2.size();
            
            while(low <= high){
                mid = low + (high-low)/2;
                if(std::abs(c2[mid].first - SOC) < 0.00005){
                    return c2[mid].second;
                }
                else if(c2[mid].first < SOC){
                    low = mid + 1;
                }
                else{
                    high = mid + 1;
                }
            }
            throw std::runtime_error("SOC not found")
        break;
        Case R0_FLAG:
            low = 0;
            high = r0.size();
            
            while(low <= high){
                mid = low + (high-low)/2;
                if(std::abs(r0[mid].first - SOC) < 0.00005){
                    return r0[mid].second;
                }
                else if(r0[mid].first < SOC){
                    low = mid + 1;
                }
                else{
                    high = mid + 1;
                }
            }
            throw std::runtime_error("SOC not found")
        break;
        Case R1_FLAG
            low = 0;
            high = r1.size();
            
            while(low <= high){
                mid = low + (high-low)/2;
                if(std::abs(r1[mid].first - SOC) < 0.00005){
                    return r1[mid].second;
                }
                else if(r1[mid].first < SOC){
                    low = mid + 1;
                }
                else{
                    high = mid + 1;
                }
            }
            throw std::runtime_error("SOC not found")
        break;
        Case R2_FLAG:
            low = 0;
            high = r2.size();
            
            while(low <= high){
                mid = low + (high-low)/2;
                if(std::abs(r2[mid].first - SOC) < 0.00005){
                    return r2[mid].second;
                }
                else if(r2[mid].first < SOC){
                    low = mid + 1;
                }
                else{
                    high = mid + 1;
                }
            }
            throw std::runtime_error("SOC not found");
        break;



    }
    return -1;
}

double calc_P_diss(double V1, double V2, double IL, double R0, double R1, double R2){
    return (V1^2)/R1 +(V2^2)/R2 + I_L^2*R0;
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
                case C1_FLAG:
                    c1.push_back(csv_pair);
                    break;
                case C2_FLAG:
                    c2.push_back(csv_pair);
                    break;
                case R0_FLAG:
                    r0.push_back(csv_pair);
                    break;
                case R1_FLAG:
                    r1.push_back(csv_pair);
                    break;
                case R2_FLAG:
                    r2.push_back(csv_pair);
                    break;
                case OCV_DISCHARGE_FLAG:
                    ocv_discharge.push_back(csv_pair);
                    break;
                case POWER_FLAG:
                    power.push_back(csv_pair);
                case OCV_CHARGE_FLAG
                    ocv_charge.push_back(csv_pair);
            }
            
        }
    }

    parameter_file.close();
    if(parameter_file.is_open())
    {
        throw std::runtime_error("File not closed properly");
    }
}

void run_sim(){

    //First time step
    //initial conditions
    double V1 = 0;
    double V2 = 0;
    double p_out = 0;
    double delta_t = 0;
    double SOC = 1;
    double R0 = return_Param(R0_FLAG,SOC);
    double R1 = return_Param(R1_FLAG,SOC);
    double R2 = return_Param(R2_FLAG,SOC);
    double C1 = return_Param(C1_FLAG,SOC);
    double C2 = return_Param(C2_FLAG,SOC);
    double OCV = return_OCV(SOC,p_out);
    //Calculate load current
    double I_L = Calc_IL(R0,OCV,V1,V2,p_out);
    double p_diss = 0.0;
    double e_diss 0.0;

    for(int i = 0; i < power.size(); i++){
        //Calculate SOC based on previous I_L
        SOC = Calc_SOC(SOC, I_L, power[i].first);
        //Use new SOC to find new paramters
        R0 = return_Param(R0_FLAG,SOC);
        R1 = return_Param(R1_FLAG,SOC);
        R2 = return_Param(R2_FLAG,SOC);
        C1 = return_Param(C1_FLAG,SOC);
        C2 = return_Param(C2_FLAG,SOC);
        OCV = return_OCV(SOC,power[i].second);\

        //Calculate V1, V2, and load current
        V1 = Calc_V1(V1,R1,I_L,C1,power[i].first);
        V2 = Calc_V2(V2,R2,I_L,C2,power[i].first);
        I_L = Calc_IL(R0,OCV,V1,V2,power[i].second);
        //calculate total load current
        p_diss += calc_P_diss(V1,V2,I_L,R0,R1,R2);
        e_diss += p_diss*power[i].first;
        
    }

    std::cout<<"Total Power Dissipated: " << p_diss << endl;
    std::cout<<"Total Energy Dissipated: " << e_diss << endl;
}



int main(int argc, char** argv)
{

    read_parameters("SOC_C1_INR_18650_25R.csv", 0);
    read_parameters("SOC_C2_INR_18650_25R.csv", 1);
    read_parameters("SOC_R0_INR_18650_25R.csv", 2);
    read_parameters("SOC_R1_INR_18650_25R.csv", 3);
    read_parameters("SOC_R2_INR_18650_25R.csv", 4);
    read_parameters("Samsung18650_25R DataDC_OCV_Curve.csv", 5);
    read_parameters("output_2014.csv",6)
    read_parameters("INR 18650-25R Open Circuit Voltage Charge.csv",7)

    run_sim();
    
    return 0;
}
