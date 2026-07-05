#include "calculator.hpp"
num m_pi{boost::math::constants::pi<num>()};

std::vector<std::string> Calculator::shunting_yard(std::vector<std::string> splited_expr)
{
    std::vector<std::string> sortie;
    std::stack<std::string> operateur;
    std::string token;
    boost::regex pattern ("([-]?[0-9]+)(.[0-9]+)?");

    for (size_t i = 0; i<splited_expr.size();i++)
    {
        if (boost::regex_match(splited_expr[i],pattern) || splited_expr[i] == "A" || splited_expr[i] == "P")
        {
            sortie.push_back(splited_expr[i]);
        }
        else if (splited_expr[i] == ")")
        {
            while (true)
            {
                token = operateur.top();
                operateur.pop();
                if (token == "(") break;
                sortie.push_back(token);
            }
        }
        else
        {
            if (operateur.empty() || (splited_expr[i]=="("))
            {
                operateur.push(splited_expr[i]);
            }
            else
            {
                if ((operateur.top()=="(") || ((this->precedence(operateur.top())) < (this->precedence(splited_expr[i]))) )
                {
                    operateur.push(splited_expr[i]);
                }
                else
                {
                    do {
                        sortie.push_back(operateur.top());
                        operateur.pop();
                    } while ((!operateur.empty()) && (operateur.top() != "(") && ((this->precedence(operateur.top())) >= (this->precedence(splited_expr[i]))));
                    operateur.push(splited_expr[i]);
                }
            }
        }
    }
    while (!operateur.empty())
    {
        sortie.push_back(operateur.top());
        operateur.pop();
    }
    return sortie;
}

std::vector<std::string> Calculator::split_expr(std::string const& expr)
{
    //qDebug() << "avant :" << QString::fromStdString(expr);
    //std::cout <<"expr"<<expr;
    std::vector<std::string> result ;
    std::string cur (1,expr[0]);
    std::string token,n_token;
    boost::regex alpha ("[a-z]+");
    short type_a; 
    if (cur == "+" || cur == "-") result.push_back("0");
    type_a=this->type(cur);
    if ((type_a == 0) || (type_a >3))
    {
        n_token = expr[1];
        if ( expr.size()>1 && (this->type(n_token) == 1 || n_token == "(" || n_token == "P" || n_token == "A"))
        {
            if (cur != "") result.push_back(cur);
            type_a = 2;
            cur = "*";
        }
    }
    for (size_t i =1; i<expr.size(); i++)
    {
        token = expr[i];
        if (i != expr.size()-1) n_token = expr[i+1];
        if (((token == "-" ) || (token == "+")) && (expr[i-1] != ')') &&((type_a != 0 && type_a < 4 && expr[i-1] != '!') || (expr[i-1] == '(')) && (this->type(n_token) != 2))
        {
            result.push_back(cur);
            result.push_back(token+"1");
            cur = "*";
            type_a = 2;
            continue;
        }

        if (type_a != this->type(token) || (type_a == 2 && this->type(token) == 2))
        {
            result.push_back(cur);
            cur = token;
        }
        else
        {
            if (this->type(cur) >= 3||(boost::regex_match(cur,alpha) && this->isFunc(cur)))
            {
                result.push_back(cur);
                cur = "";
            }
            cur+=token;
        }

        type_a=this->type(token);

        if ((type_a == 0 || type_a >3) && (i<expr.size()-1) && (((this->type(n_token)) == 1) || n_token == "(" || n_token == "P" || n_token == "A"))
        {
            if (cur != "") result.push_back(cur);
            type_a = 2;
            cur = "*";
        }
        else if (type_a == 3 && token==")" && this->type(n_token) > 3 && (i<expr.size()-1))
        {
            if (cur != "") result.push_back(cur);
            type_a = 2;
            cur = "*";
        }
    }
    if (cur != "")
    {
        result.push_back(cur);
        cur = "";
    }
    /*qDebug() << "apres :" ;
    for (auto const& f: result)
    {
        qDebug() << QString::fromStdString(f);
    }*/
    return result;
}

short Calculator::precedence(std::string const& e)
{
    if ((e == "+") || (e == "-")) return 1;
    else if ((e == "*") || (e == "/") || (e == "%")) return 2;
    else if (e == "^") return 3;
    else if (e == "!") return 5;
    else return 4;
}

short Calculator::type(std::string c)
{
    std::vector<std::string> pattern ({"[0-9.]+","[a-z]+","[\\$\\-+/*^%!]","[)(]","P","A","!"}) ;
    boost::regex r;
    size_t i;
    for (i=0; i<pattern.size();i++){
        r = pattern[i];
        if (boost::regex_match(c,r)){
                break;
        }
    }
    return i;
}

bool Calculator::isFunc(std::string e)
{
    for (auto const&x: this->fonc_list){
        if (x == e)
        {
            return true;
        }
    }
    return false;
}

num Calculator::convertToRadian(num const& x)
{
    if (this->angle == 'D')
    {
        return x/180*m_pi;
    }
    else if (this->angle == 'G')
    {
        return x/100*m_pi;
    }
    else
    {
        return x;
    }
}

num Calculator::convertFromRadian(num const& x)
{
    if (this->angle == 'D')
    {
        return x*180/m_pi;
    }
    else if (this->angle == 'G')
    {
        return x*100/m_pi;
    }
    else
    {
        return x;
    }
}

std::string Calculator::calculate(std::string& expr)
{
    expr.erase(std::remove_if(expr.begin(), expr.end(), ::isspace), expr.end());
    num rep;
    std::string message {"Math error:"};
    std::vector<std::string> postfix_expr;
    try 
    {
        if (!(this->test_syntax(expr))) throw std::runtime_error("Erreur de syntaxe");
        postfix_expr= this->shunting_yard(this -> split_expr(expr));
        rep = this -> eval_expr(postfix_expr);
    }

    catch (std::domain_error const& e)
    {
        this -> state = 2;
        return message + e.what();
    }
    catch (std::out_of_range const& e)
    {
        this -> state = 3;
        return "Math Error: Nombre trop grand";
    }
    catch (std::invalid_argument const& e)
    {
        this -> state = 1;
        return "Math Error";
    }
    catch (std::overflow_error const& e)
    {
        this -> state = 3;
        return "Math Error: Nombre trop grand";
    }
    catch (std::underflow_error const& e)
    {
        this -> state = 3;
        return "Math Error: Nombre trop grand";
    }
    catch (std::runtime_error const& e)
    {
        this -> state = 1;
        return e.what();
    }
    catch (std::exception const& e)
    {
        this -> state = 1;
        return "Erreur de syntaxe";
    }
    catch (...)
    {
        this -> state = 1;
        return "Erreur de syntaxe";
    }
    this -> state = 0;
    this -> resultat = rep;

    return this->formatOutput(rep);
}

void Calculator::setAngle(char a)
{
    this -> angle = a;
}

char Calculator::getAngle()
{
    return this -> angle;
}

char Calculator::getState()
{
    return this->state;
}

char Calculator::getMode()
{
    return this->mode;
}

void Calculator::setMode(char m)
{
    this -> mode = m;
}

std::string Calculator::MS(std::string & s)
{
    std::string m = this -> calculate (s);
    if (this -> state == 0) 
    {
        this -> memory = this -> resultat;
        return this -> memory.str();
    }
    return m ;
}

std::string Calculator::MR()
{
    if (boost::math::isnan(this->memory)) return "Erreur: Memoire vide";
    return this-> memory.str();
}

std::string Calculator::MP(std::string & s)
{
    std::string m = this -> calculate (s);
    if (this -> state == 0) 
    {
        num l = this->memory ? !(boost::math::isnan(this->memory)) : 0;
        l += this -> resultat;
        this -> memory = l;
        return this -> memory.str();
    }
    return m ;
}

std::string Calculator::MM(std::string & s)
{
    std::string m = this -> calculate (s);
    if (this -> state == 0) 
    {
        num l = this->memory ? !(boost::math::isnan(this->memory)) : 0;
        l -= this -> resultat;
        this -> memory = l;
        return this -> memory.str();
    }
    return m ;
}

void Calculator::MC()
{
    this->memory=nan("");
}

bool Calculator::test_syntax(std::string & expr)
{
    int p{0};
    for (auto e: expr)
    {
        if (e == '(') p++;
        else if (e == ')') p--;
        if (p<0) return false; 
    }
    if (p!=0) return false; 

    boost::regex pat {"\\([\\-+/*^%!$]*\\)"};
    if (boost::regex_search(expr,pat)) return false; 
    boost::regex pat1 {"[+]{2,}"}, pat2 {"[\\-]{2,}"}, pat3 {"\\+\\-"}, pat4 {"\\-\\+"};

    while (true)
    {
        if (boost::regex_search(expr, pat1)) {
            expr = boost::regex_replace(expr, pat1, "+");
            continue;
        }

        if (boost::regex_search(expr, pat2)) {
           expr = this->replaceConsecutiveDashes(expr);
            continue;
        }

        if (boost::regex_search(expr, pat3)) {
            expr = boost::regex_replace(expr, pat3, "-");
            continue;
        }

        if (boost::regex_search(expr, pat4)) {
            expr = boost::regex_replace(expr, pat4, "-");
            continue;
        }

        break;
    }

    std::string car {expr[0]};
    if ((this->type(car) == 2) && (expr[0] != '+') && (expr[0] != '-'))return false; 

    car = expr[expr.size()-1];
    pat = "[0-9A-Z)!]";
    if (!(boost::regex_match(car,pat))) return false; 

    pat = "([a-z]+)([\\-+/*^%!$]{1})";
    if (boost::regex_search(expr,pat)) return false; 

    std::vector<std::string> pattern {"\\.[^\\d]+","[^\\d]+\\.","\\d+\\.\\d+(\\.\\d+)+"}; 
    for(auto x: pattern ){
        pat=x;
        if (boost::regex_search(expr,pat)) return false; 
    }

    for (auto x: this->fonc_list)
    {
        pat = x+"([^(])";
        if (boost::regex_search(expr,pat)) return false; 
    }

    pat = "[/*^%$]{2,}" ;
    if (boost::regex_search(expr,pat)) return false; 

    pat = "[\\-+/*^%!$][!]+";
    if (boost::regex_search(expr,pat)) return false; 

    pat = "[\\-\\+][/*^%$]";
    if (boost::regex_search(expr,pat)) return false; 

    pat = "[\\!]{1}[a-zA-Z]+";
    if (boost::regex_search(expr,pat)) return false; 

    pat = "[\\!]{1}[\\d]+";
    if (boost::regex_search(expr,pat)) return false; 

    pat = "\\-\\(";
    if (boost::regex_search(expr,pat)) boost::regex_replace(expr,pat,"-1*(");

    return true;
}

std::string Calculator::replaceConsecutiveDashes(std::string const& input) {
    std::string result;
    boost::regex dashRegex("-+");
    boost::sregex_iterator currentMatch(input.begin(), input.end(), dashRegex);
    boost::sregex_iterator lastMatch;

    size_t lastPos = 0;

    while (currentMatch != lastMatch) {
        boost::smatch match = *currentMatch;
        size_t matchPos = match.position();
        size_t matchLength = match.length();
        result += input.substr(lastPos, matchPos - lastPos);
        
        result += (matchLength % 2 == 0) ? "+" : "-";

        lastPos = matchPos + matchLength;
        currentMatch++;
    }

    result += input.substr(lastPos);

    return result;
}


std::string Calculator::formatOutput(num const& res)
{
    std::ostringstream s;
    std::string rep;
    if (this -> mode == 'F')
    {
        s << std::scientific<<std::showpos<< res;
    }
    else if (this -> mode == 'D')
    {
        long degres = static_cast<int> (res); 
        num decimal_minutes = (res - degres)*60;
        long minutes = static_cast<int> (decimal_minutes);
        num secondes = (decimal_minutes - minutes)*60;
        s <<degres<<"° "<<std::abs(minutes)+ "' "<<abs(secondes)<<"''";
    }
    else
    {
        s<<res;
    }
    //qDebug()<<"ETO"<<QString::fromStdString(s.str());
    return s.str();
}

num Calculator::eval_expr(std::vector<std::string> const& conteneur)
{
    qDebug()<<"IRETO: ";
    for (auto const& q: conteneur)
    {
        qDebug()<<QString::fromStdString(q)<<"|";
    }
    boost::regex number("[0-9]+");
    std::stack<num> pile;
    num x,y,z;

    for(size_t i =0 ; i < conteneur.size() ; i++)
    {
        if(boost::regex_search(conteneur[i],number))
        {
            pile.push(static_cast<num>(conteneur[i]));
        }
        else if (conteneur[i] == "A")
        {
            pile.push(this->resultat);
        }
        else if (conteneur[i] == "P")
        {
            pile.push(m_pi);
        }
        else if(conteneur[i]=="+")
        {
            x = pile.top();
            pile.pop();
            y = pile.top();
            pile.pop();
            z = y + x;
            pile.push(z);
        }
        else if(conteneur[i]=="-")
        {
            x = pile.top();
            pile.pop();
            y = pile.top();
            pile.pop();
            z = y - x;
            pile.push(z);
        }
        else if(conteneur[i]=="/")
        {
            x = pile.top();
            pile.pop();
            y = pile.top();
            pile.pop();
            if(x==0)
            {
                throw std::domain_error("Division par zéro");
            }
            z = y / x;
            pile.push(z);
        }
        else if(conteneur[i]=="*")
        {
            x = pile.top();
            pile.pop();
            y = pile.top();
            pile.pop();
            z = y * x;
            pile.push(z);
        }
        else if(conteneur[i]=="^")
        {
            x = pile.top();
            pile.pop();
            y = pile.top();
            pile.pop();
            z = pow(y,x);
            pile.push(z);
        }
        else if(conteneur[i]=="$")
        {
            x = pile.top();
            pile.pop();
            y = pile.top();
            pile.pop();
            if (x<0)
            {
                throw std::domain_error("Racine d'un nombre negatif");
            }
            else if (y == 0)
            {
                throw std::domain_error("La racine par zero d'un nombre");
            }
            if (y == 2.0L){
                z = sqrt(x);
            }
            else
            {
                z = pow(x , (1/y));
            }
            pile.push(z);
        }
        else if(conteneur[i]=="%")
        {
            x = pile.top();
            pile.pop();
            y = pile.top();
            pile.pop();
            if (x == 0L)
            {
                throw std::domain_error("Division par zéro");
            }
            z = fmod(y,x);
            pile.push(z);
        }
        else if(conteneur[i]=="!")
        {
            x = pile.top();
            if(x<0)
            {
                throw std::domain_error("Factorielle d'un nombre negatif");
            }
            z = tgamma(x+1);
            pile.pop();
            pile.push(z);
        }
        else if(conteneur[i]=="exp")
        {
            x = pile.top();
            z = exp(x);
            pile.pop();
            pile.push(z);
        }
        else if(conteneur[i]=="ln")
        {
            x = pile.top();
            if(x<=0)
            {
                throw std::domain_error("Logarithme d'un nombre nul ou negatif");
            }
            z = log(x);
            pile.pop();
            pile.push(z);
        }
        else if(conteneur[i]=="log")
        {
            x = pile.top();
            if(x<=0)
            {
                throw std::domain_error("Logarithme d'un nombre nul ou negatif");
            }
            z = log10(x);
            pile.pop();
            pile.push(z);
        }
        else if(conteneur[i]=="sin")
        {
            x = pile.top();
            z = sin(this->convertToRadian(x));;
            pile.pop();
            pile.push(z);
        }
        else if(conteneur[i]=="asin")
        {
            x = pile.top();
            if (x<-1 || x > 1) throw std::domain_error("Fonction asin: Valeur en dehors de l'ensemble de definition");
            z = this->convertFromRadian(asin(x));
            pile.pop();
            pile.push(z);
        }
        else if(conteneur[i]=="sh")
        {
            x = pile.top();
            z = sinh(x);
            pile.pop();
            pile.push(z);
        }
        else if(conteneur[i]=="ash")
        {
            x = pile.top();
            z = asinh(x);
            pile.pop();
            pile.push(z);
        }
        else if(conteneur[i]=="cos")
        {
            x = pile.top();
            z = cos(this->convertToRadian(x));
            pile.pop();
            pile.push(z);
        }
        else if(conteneur[i]=="acos")
        {
            x = pile.top();
            if (x<-1 || x > 1) throw std::domain_error("Fonction acos: Valeur en dehors de l'ensemble de definition");
            z = this->convertFromRadian(acos(x));
            pile.pop();
            pile.push(z);
        }
        else if(conteneur[i]=="ch")
        {
            x = pile.top();
            z = cosh(x);
            pile.pop();
            pile.push(z);
        }
        else if(conteneur[i]=="ach")
        {
            x = pile.top();
            z = acosh(x);
            pile.pop();
            pile.push(z);
        }
        else if(conteneur[i]=="tan")
        {
            x = pile.top();
            z = this->convertToRadian(x);
            if (z==(m_pi/2))
            {
                throw std::domain_error("Fonction tan: Valeur en dehors de l'ensemble de definition");
            }
            z = tan(z);
            pile.pop();
            pile.push(z);
        }
        else if(conteneur[i]=="atan")
        {
            x = pile.top();
            z = this->convertFromRadian(atan(x));
            pile.pop();
            pile.push(z);
        }
        else if(conteneur[i]=="th")
        {
            x = pile.top();
            z = tanh(x);
            pile.pop();
            pile.push(z);
        }
        else if(conteneur[i]=="ath")
        {
            x = pile.top();
            z = atanh(x);
            pile.pop();
            pile.push(z);
        }
        else if(conteneur[i]=="abs")
        {
            x = pile.top();
            z = abs(x);
            pile.pop();
            pile.push(z);
        }
        else if(conteneur[i]=="ceil")
        {
            x = pile.top();
            z = ceil(x);
            pile.pop();
            pile.push(z);
        }
        else if(conteneur[i]=="floor")
        {
            x = pile.top();
            z = floor(x);
            pile.pop();
            pile.push(z);
        }
        else if(conteneur[i]=="round")
        {
            x = pile.top();
            z = round(x);
            pile.pop();
            pile.push(z);
        }
        else
        {
            throw std::invalid_argument("Erreur de syntaxe");
        }
    }
    if (pile.size()>1)
    {
        throw std::runtime_error("Erreur de syntaxe ");
    }
    if (boost::math::isinf(pile.top()) || boost::math::isnan(pile.top()) )
    {
        throw std::overflow_error("Nombre trop grand");
    }
    return pile.top();
}