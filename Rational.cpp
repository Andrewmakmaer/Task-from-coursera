#include <iostream>
#include <algorithm>
#include <sstream>
#include <exception>
using namespace std;

void Is_Negativ(int &numer, int &denom)
{
    if (denom < 0)   //если знаментель отрицательный
    {                //приводим к виду -num/den
        numer *= -1; //или num/den
        denom *= -1;
    }
}

void Reduction(int &numer, int &denom)
{
    int NOD = __gcd(numer, denom); //находим наибольший общий делитель
    numer /= NOD;                  //сокращаем дробь
    denom /= NOD;
}

class Rational
{
public:
    Rational()
    {
        __numerator__ = 0;
        __denominator__ = 1;
    }

    Rational(int numerator, int denominator)
    {
        if (denominator == 0)
        {
            throw invalid_argument("Invalid argument");
        }
        Reduction(numerator, denominator);
        Is_Negativ(numerator, denominator);
        __numerator__ = numerator;
        __denominator__ = denominator;
    }

    ~Rational() {}

    int Numerator() const
    {
        return __numerator__;
    }

    int Denominator() const
    {
        return __denominator__;
    }

    void Writing_new_values(int &numerator_value, int &denominator_value)
    {
        if (denominator_value == 0)
        {
            throw invalid_argument("Invalid argument");
        }
        Reduction(numerator_value, denominator_value);
        Is_Negativ(numerator_value, denominator_value);
        __numerator__ = numerator_value;
        __denominator__ = denominator_value;
    }

private:
    int __numerator__;
    int __denominator__;
};

bool operator==(const Rational &lhs, const Rational &rhs)
{
    Rational lhs_copy(lhs.Numerator(), lhs.Denominator());
    Rational rhs_copy(rhs.Numerator(), rhs.Denominator());
    if (lhs_copy.Numerator() == rhs_copy.Numerator() && lhs_copy.Denominator() == rhs_copy.Denominator())
    {
        return true;
    }
    else
    {
        return false;
    }
}

Rational operator+(const Rational &lhs, const Rational &rhs)
{
    if (lhs.Denominator() == rhs.Denominator())
    {
        return Rational(lhs.Numerator() + rhs.Numerator(), lhs.Denominator());
    }
    else
    {
        return Rational(lhs.Numerator() * rhs.Denominator() + rhs.Numerator() * lhs.Denominator(),
                        rhs.Denominator() * lhs.Denominator());
    }
}

Rational operator-(const Rational &lhs, const Rational &rhs)
{
    if (lhs.Denominator() == rhs.Denominator())
    {
        return Rational(lhs.Numerator() - rhs.Numerator(), lhs.Denominator());
    }
    else
    {
        return Rational(lhs.Numerator() * rhs.Denominator() - rhs.Numerator() * lhs.Denominator(),
                        rhs.Denominator() * lhs.Denominator());
    }
}

Rational operator*(const Rational &lhs, const Rational &rhs)
{
    return Rational(lhs.Numerator() * rhs.Numerator(), lhs.Denominator() * rhs.Denominator());
}

Rational operator/(const Rational &lhs, const Rational &rhs)
{
    if (rhs.Numerator() == 0)
    {
        throw domain_error("Division by zero");
    }
    return Rational(lhs.Numerator() * rhs.Denominator(), rhs.Numerator() * lhs.Denominator());
}

ostream &operator<<(ostream &stream, const Rational &item)
{
    stream << item.Numerator() << "/" << item.Denominator();
    return stream;
}

istream &operator>>(istream &stream, Rational &item)
{
    int a = 0, b = 0;
    char l;
    stream >> a >> l >> b;
    if (l == '/')
    {
        item.Writing_new_values(a, b);
    }
    return stream;
}

bool operator<(const Rational &lhs, const Rational &rhs)
{
    int a = lhs.Numerator() * rhs.Denominator();
    int b = rhs.Numerator() * lhs.Denominator();
    return a < b;
}

Rational Equation_Solver(const Rational &term1, const Rational &term2, const char &oper)
{
    if (oper == '+')
    {
        return term1 + term2;
    }
    else if (oper == '-')
    {
        return term1 - term2;
    }
    else if (oper == '*')
    {
        return term1 * term2;
    }
    else if (oper == '/')
    {
        return term1 / term2;
    }
}

int main()
{
    Rational term1, term2;
    char oper;
    cout << "Enter fractional numbers and operation: ";
    try
    {
        cin >> term1 >> oper >> term2;
    }
    catch (invalid_argument &ex)
    {
        cout << ex.what() << endl;
        return 1;
    }

    try
    {
        cout << "= "<< Equation_Solver(term1, term2, oper) << endl;
    }
    catch (domain_error &ex)
    {
        cout << ex.what() << endl;
    }

    return 0;
}