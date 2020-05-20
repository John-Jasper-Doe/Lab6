/**
 ******************************************************************************
 * @file    main.cpp
 * @author  Maxim <aveter@bk.ru>
 * @date    19/05/2019
 * @brief   The main project file is "Matrix".
 ******************************************************************************
 */

#include "sparse_matrix.hpp"

#include <iostream>
#include <cassert>
#include <iomanip>


/**
 * @brief Main function / entry point.
 */
int main()
{
  const int num_elem = 10;
  sparse_matrix<int> matrix;

  /* Initialization of the main diagonal of the matrix */
  for (size_t i = 0; i < num_elem; ++i) {
    matrix[i][i] = static_cast<int>(i);
  }

  /* Initialization of the secondary diagonal of the matrix */
  for (size_t i = 0; i < num_elem; ++i) {
    matrix[i][(num_elem - 1) - i] = static_cast<int>((num_elem - 1) - i);
  }

  /* Output matrix values from range [1,1] to [8,8] */
  for (size_t i = 1; i <= 8; ++i) {
    for (size_t j = 1; j <= 8; ++j) {
      std::cout << std::setw(3) << matrix[i][j] << ' ';
    }
    std::cout << '\n';
  }
  std::cout << std::endl;

  /* Output the number of matrix cells occupied by the data */
  std::cout << matrix.size() << '\n' << std::endl;

  /* Output of all occupied cells in the matrix */
  for (const std::tuple<size_t, size_t, int> &elem : matrix) {
    const size_t &hindex = std::get<0>(elem);
    const size_t &vindex = std::get<1>(elem);
    const int &value = std::get<2>(elem);

    std::cout << "[" << hindex << ", " << vindex << "] = " << value << '\n';
  }

  std::cout << std::endl;

  /* Additional checks upon request */
  const sparse_matrix<int> m2 = matrix;

  /* Output of all occupied cells in the matrix */
  for (const auto cit : m2) {
    const size_t &hindex = std::get<0>(cit);
    const size_t &vindex = std::get<1>(cit);
    const int &value = std::get<2>(cit);

    std::cout << "[" << hindex << ", " << vindex << "] = " << value << '\n';
  }

  std::cout << std::endl;

  matrix.clear();


  //-----------------------------------------------------------------
  // Bonus test
  //
  const int m_size = 10;
  int count_step = 0;
  sparse_matrix<int> matrix2;

  for (size_t i = 0; i < num_elem; i += 2) {
    for (size_t j = 0; j < num_elem; j += 2) {
      matrix2[i + 1][j] = static_cast<int>(8);
      matrix2[i][j + 1] = static_cast<int>(8);
      ++count_step;
    }
  }


  for (size_t i = 0; i < m_size; ++i) {
    for (size_t j = 0; j < m_size; ++j) {
      std::cout << std::setw(3) << matrix2[i][j] << ' ';
    }
    std::cout << '\n';
  }
  std::cout << std::endl;
  std::cout << "Count step: " << count_step << std::endl;

  return 0;
}

