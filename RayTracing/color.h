#ifndef COLOR_H
#define COLOR_H

#include "basic.h"

namespace tl {

	struct Color {
	private:
		double len() {
			return sqrt(sqr(r) + sqr(g) + sqr(b));
		}
	public:
		double r, g, b;//range from 0-1
		Color(double r = 0, double g = 0, double b = 0) :r(r), g(g), b(b) {}

		static Color Red, Green, Blue, Black, White, Cyano, Purple, Yellow;

		void Color::normalize() {
			if (r > 1) r = 1;
			if (g > 1) g = 1;
			if (b > 1) b = 1;
		}
	};
	Color Color::Red = Color(1, 0, 0);
	Color Color::Green = Color(0, 1, 0);
	Color Color::Blue = Color(0, 0, 1);
	Color Color::Black = Color(0, 0, 0);
	Color Color::White = Color(1, 1, 1);
	Color Color::Cyano = Color(1, 1, 0);
	Color Color::Purple = Color(1, 0, 1);
	Color Color::Yellow = Color(0, 1, 1);

	Color operator + ( const Color& A , const Color& B ) {
		return Color( A.r + B.r , A.g + B.g , A.b + B.b );
	}
	
	Color operator - ( const Color& A , const Color& B ) {
		return Color( A.r - B.r , A.g - B.g , A.b - B.b );
	}
	
	Color operator * ( const Color& A , const Color& B ) {
		return Color( A.r * B.r , A.g * B.g , A.b * B.b );
	}
	
	Color operator * ( const Color& A , const double& k ) {
		return Color( A.r * k , A.g * k , A.b * k );
	}
	
	Color operator / ( const Color& A , const double& k ) {
		return Color( A.r / k , A.g / k , A.b / k );
	}
	
	Color& operator += ( Color& A , const Color& B ) {
		A = A + B;
		return A;
	}
	
	Color& operator -= ( Color& A , const Color& B ) {
		A = A - B;
		return A;
	}
	
	Color& operator *= ( Color& A , const double& k ) {
		A = A * k;
		return A;
	}
	
	Color& operator /= ( Color& A , const double& k ) {
		A = A / k;
		return A;
	}
	
	



}


#endif