#include "ComponentMust.h"
ComponentMust::ComponentMust(){}

Frequency ComponentMust::eff_convert_frequency(char freq_char) const

{
	//cette fonction fait la conversion 
	//de la frequence de type string lue de XML de MUST
	Frequency frequence;

	switch (freq_char)
	{
	case 'A':
		frequence = Annual;
		break;
	case 'S':
		frequence = Semiannual;
		break;
	case 'Q':
		frequence = Quarterly;
		break;
	case 'M':
		frequence = Monthly;
		break;
	case 'W':
		frequence = Weekly;
		break;
	default:
		frequence = NoFrequency;
	}

	return frequence; // retourne frequency de type QuantLib
}



DayCounter ComponentMust::eff_convert_basis(std::string basis) const
{
	//cette fonction fait la conversion 
	//de la convention de calcul (basis) de type string lue de XML de MUST

	// daycounter;
	if (basis == "A360")
		return Actual360();

	if (basis == "A365")
		return Actual365Fixed();

	if (basis == "Actual")
		return ActualActual();

	if (basis == "Daily Price")
		return OneDayCounter();

	if (basis == "Business 252")
		return Business252();

	if (basis == "JGB")
		return Actual365NoLeap();

	if (basis == "30/360")
		return Thirty360(Thirty360::EurobondBasis);


	return Thirty360();

}


Date ComponentMust::eff_convert_date(std::string dateString) const
{
	// convert string type to date type
	int my_year;
	int my_month;
	int my_day;

	if (dateString.length() == 10)
	{
		my_year = boost::lexical_cast<int>(dateString.substr(1, 4));
		my_month = boost::lexical_cast<int>(dateString.substr(5, 2));
		my_day = boost::lexical_cast<int>(dateString.substr(7, 2));
	}

	if (dateString.length() == 8)
	{
		my_year = boost::lexical_cast<int>(dateString.substr(0, 4));
		my_month = boost::lexical_cast<int>(dateString.substr(4, 2));
		my_day = boost::lexical_cast<int>(dateString.substr(6, 2));
	}



	Month m = Month(my_month);
	Date date(my_day, m, my_year);
	return date; // retourne la date du format QuantLib
}


Rate ComponentMust::eff_convert_rate(std::string rateString) const
{
	if (rateString[rateString.length() - 1] == '%')
	{
		rateString.erase(rateString.length() - 1, 1);
	}

	Rate rateReturn = boost::lexical_cast<Rate>(rateString);
	return rateReturn/100.0;
}



int ComponentMust::eff_freq_int(Frequency freq) const
{
	if (freq == Annual) return 1;
	if (freq == Semiannual) return 2;
	if (freq == Quarterly) return 4;
	if (freq == Monthly) return 12;
	if (freq == Weekly) return 52;
	return 1;

}


std::vector<std::tuple<Date, Date, Rate>> ComponentMust::eff_matice_rate(vector<vector<string>> matriceString)
{
	vector<std::tuple<Date, Date, Rate>> matrixRate;

	// conversion des �l�ments de la matrice
	for (size_t i = 0; i < matriceString.size(); i++)
	{

		Date startDate = this->eff_convert_date(matriceString[i][0]);
		Date endDate = this->eff_convert_date(matriceString[i][1]);
		Rate rate = this->eff_convert_rate(matriceString[i][2]);

		matrixRate.push_back(std::make_tuple(startDate, endDate, rate));
	}

	return matrixRate;
}




vector<std::tuple<Date, Date, Real>> ComponentMust::eff_matice_spread(vector<vector<string>> matriceString)
{
	vector<std::tuple<Date, Date, Spread>> matrixReal;

	// conversion des �l�ments de la matrice
	for (size_t i = 0; i < matriceString.size(); i++)
	{

		Date startDate = this->eff_convert_date(matriceString[i][0]);
		Date endDate = this->eff_convert_date(matriceString[i][1]);
		Real spread = boost::lexical_cast<Spread>(matriceString[i][2]);
		matrixReal.push_back(std::make_tuple(startDate, endDate, spread));
	}

	return matrixReal;
}