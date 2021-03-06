#include <cmath>
#include <array>



#pragma once



template<std::size_t length>
class Spectrum {
	private:
		class Intensity {
			private:
				double& r1;
				double& i1;
				double& r2;
				double& i2;
				double intensity;
				Intensity() {};
			public:
				Intensity(double& _r1, double& _i1, double& _r2, double& _i2);
				operator double();
				double operator= (double);
				double operator= (const Intensity&);
		};
		std::array<double, length> resultOfCdft;

		// cdft用
		static int ip[length];
		static double w[length / 2];

		Spectrum() {};
	public:
		template<typename FowardAccessIterator>
		Spectrum(FowardAccessIterator begin, FowardAccessIterator end);
		Intensity operator[] (std::size_t);
		std::size_t size();
		
		template<typename FowardAccessIterator>
		void reverse(FowardAccessIterator begin, FowardAccessIterator end);
};

template<std::size_t length>
Spectrum<length>::Intensity(double& _r1, double& _i1, double& _r2, double& _i2) : r1(_r1), i1(_i1), r2(_r2), i2(_i2) {
	
}