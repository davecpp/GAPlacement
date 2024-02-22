#pragma once
#include <vector>
#include <type_traits>

using int_t = std::int32_t;

template<typename Ty>
using MatrixT = std::vector<std::vector<Ty>>;

template<typename T>
struct is_integral_matrix : std::false_type {};

template<typename T>
struct is_integral_matrix<std::vector<std::vector<T>>> : std::integral_constant<bool, std::is_integral<T>::value> {};



template<typename MatrixType, typename ElemTy = typename MatrixType::value_type::value_type>
//typename std::enable_if<is_integral_matrix<MatrixType>::value, MatrixType>::type
MatrixT<ElemTy> makeMatrix(size_t rows, size_t cols, typename MatrixType::value_type::value_type defaultValue) {
	return MatrixT<ElemTy>(rows, typename MatrixType::value_type(cols, defaultValue));
}



