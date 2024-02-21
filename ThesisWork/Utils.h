#pragma once
#include <vector>
#include <type_traits>

using int_t = int;

template<typename Ty>
using MatrixT = std::vector<std::vector<Ty>>;

struct Coord
{
	Coord(int_t i_x, int_t i_y) : x(i_x), y(i_y) {}
	Coord() = default;
	int_t x = -1;
	int_t y = -1;

	operator bool() {
		return x >= 0 && y >= 0;
	}
};


template<typename T>
struct is_integral_matrix : std::false_type {};

template<typename T>
struct is_integral_matrix<std::vector<std::vector<T>>> : std::integral_constant<bool, std::is_integral<T>::value> {};



template<typename MatrixType, typename ElemTy = typename MatrixType::value_type::value_type>
//typename std::enable_if<is_integral_matrix<MatrixType>::value, MatrixType>::type
MatrixT<ElemTy> makeMatrix(size_t rows, size_t cols, typename MatrixType::value_type::value_type defaultValue) {
	return MatrixT<ElemTy>(rows, typename MatrixType::value_type(cols, defaultValue));
}



