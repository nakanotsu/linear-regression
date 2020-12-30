#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <omp.h>
#include <algorithm>
#include <list>
#include <string>
#include <map>
#include <numeric>
#include <math.h>

std::map<double, double> smi;
std::map <std::string, double> dollars;
std::map<double, double> cash;
std::list<double> buff;

auto convertYear(std::map<double, double> y, std::map<std::string, double> d) {
    std::map<double, double> cash;
    auto years = y.begin();        
    while (years != y.end()) {
        buff = {};
        auto dollars = d.begin();
        while (dollars != d.end()) {            
            if (years->first == std::stod(dollars->first.substr(0, 4))) {                
                buff.push_back(dollars->second);
            }
            ++dollars;
        }
        double sum = std::accumulate(std::begin(buff), std::end(buff), 0.0);
        double result = sum / buff.size();
        cash.insert(std::pair<double, double>(years->first, years->second/result));
        ++years;
    }
    return cash;
}

auto linearRegression(std::map<double, double> m, int year) {
    double n = m.size();
    double sx = 0;
    double sxx = 0;
    double sy = 0;
    double syy = 0;
    double sxy = 0;
    auto iter = m.begin();
    while (iter != m.end()) {
        sx += iter->first;
        sxx += pow(iter->first, 2);
        sy += iter->second;
        syy += pow(iter->second, 2);
        sxy += iter->first * iter->second;
        ++iter;
    }
    double b = (n * sxy - sx * sy) / (n * sxx - sx * sx);
    double a = (1 / n) * sy - b * (1 / n) * sx;
    if (year != 0) {
        double c = b * year + a;
        double deviation = abs(cash[year] - c);
        std::cout << "year " << year << " average : $" << cash[year] << " USD." << std::endl;
        std::cout << "in linear regresion : $" << c << " USD." << std::endl;
        std::cout << "deviation : $" << deviation << " USD." << std::endl;
    }    
    std::cout << "y = " << b << "x " << a;
}

auto csv_smi(std::string path) {
    std::map<double, double> map_tmp;
    std::ifstream csv;
    csv.open(path);
    std::string line;
    getline(csv, line);
    while (csv.peek() != EOF) {
        getline(csv, line);
        line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
        size_t position = line.find(";") + 1;
        std::string key_k = line.substr(0, line.find(";"));
        std::string key_v = line.substr(position);
        key_k.erase(std::remove(key_k.begin(), key_k.end(), '\"'), key_k.end());
        key_v.erase(std::remove(key_v.begin(), key_v.end(), '\"'), key_v.end());
        double key_kd = std::stod(key_k);
        double key_vd = std::stod(key_v);
        map_tmp.insert(std::pair<double, double>(key_kd, key_vd));
    }
    csv.close();
    return map_tmp;
 }
    
auto csv_dollars(std::string path) {     
    std::map<std::string, double> map_tmp;
    std::ifstream csv;
    csv.open(path);
    std::string line;
    getline(csv, line);    
    while (csv.peek() != EOF) {
        getline(csv, line);
        line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
        size_t position = line.find(";") + 1;
        std::string key_k = line.substr(0, line.find(";"));
        std::string key_v = line.substr(position);
        key_k.erase(std::remove(key_k.begin(), key_k.end(), '\"'), key_k.end());
        key_v.erase(std::remove(key_v.begin(), key_v.end(), '\"'), key_v.end());
        double key_vd = std::stod(key_v);
        map_tmp.insert(std::pair<std::string, double>(key_k, key_vd));        
    }
    csv.close();
    return map_tmp;        
}

int main(int argc, char** argv) { 
    if (argc > 2) {
        std::string ruta_smi(argv[1]);
        std::string ruta_dollars(argv[2]);
        int year = 0;
        if (argc > 3) {
            std::string year_b(argv[3]);
            year = std::stoi(year_b);
        }
               
        smi = csv_smi(ruta_smi);
        dollars = csv_dollars(ruta_dollars);       
        cash = convertYear(smi, dollars);

        // Call for Linear regression
        linearRegression(cash, year);
    }
    else {
        std::cout << "Insuficcient arguments.";
    }
    //END
    std::cout << std::endl;
    return 0;
}