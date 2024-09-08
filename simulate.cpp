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

/*
PARAMETER FLAGS FOR READING IN CSV
C1 = 0
C2 = 1
R0 = 2
R1 = 3
R2 = 4
OCV = 5
*/


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