#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <cmath>
#include <boost/regex.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <cassert>
#include <iomanip>
#include <QDebug>

using namespace boost::multiprecision;
typedef number<cpp_dec_float<200>,et_on> num;

class Calculator
{
    private:
        char angle {'D'};
        int state {0};
        char mode {'N'};
        num resultat = 0;
        num memory = nan("");
        std::vector<std::string> fonc_list {"log","ln","sin","cos","tan","asin","acos","atan","sh","ch","th","ath","ach","ash","exp","\\$","ceil","abs","floor","round"};
        std::vector<std::string> shunting_yard(std::vector<std::string>);
        std::vector<std::string> split_expr(std::string const&);
        num eval_expr(std::vector<std::string> const&);
        short precedence(std::string const& );
        bool test_syntax(std::string&);
        std::string replaceConsecutiveDashes(std::string const&);
        short type(std::string);
        bool isFunc(std::string);
        num convertToRadian(num const&);
        num convertFromRadian(num const&);
        std::string formatOutput(num const&);

    public:
        std::string calculate(std::string &);
        void setAngle(char);
        char getAngle();
        char getState ();
        char getMode();
        void setMode(char);
        
        std::string MS(std::string & );
        std::string MP(std::string &);
        std::string MM(std::string &);
        std::string MR ();
        void MC();
};

#endif 
