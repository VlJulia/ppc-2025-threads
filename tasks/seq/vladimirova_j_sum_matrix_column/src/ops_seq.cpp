#include "seq/vladimirova_j_sum_matrix_column/include/ops_seq.hpp"

#include <cmath>
#include <cstddef>
#include <iostream>
#include <vector>

bool vladimirova_j_sum_matrix_column::TestTaskSequential::PreProcessingImpl() {
  auto *tmp_p = reinterpret_cast<int *>(task_data->inputs[0]);
  input_ = std::vector<int>(tmp_p, tmp_p + m_col_ * m_row_);
  size_t output_size = m_col_;
  output_ = std::vector<int>(output_size, 0);
  return true;
}

bool vladimirova_j_sum_matrix_column::TestTaskSequential::ValidationImpl() {
  m_row_ = (size_t)(task_data->inputs_count[1]);
  m_col_ = (size_t)(task_data->inputs_count[0]);
  return (m_col_ == task_data->outputs_count[0]) && (m_col_ > 0) && (m_row_ > 0);
}

bool vladimirova_j_sum_matrix_column::TestTaskSequential::RunImpl() {
  if (m_row_ == 1) {
    output_ = input_;
    return true;
  }
  if (m_col_ == 1) {
    for (size_t i = 0; i < m_row_; i++) {
      output_[0] += input_[i];
    }
    return true;
  }

  for (size_t i = 0; i < m_col_; i++) {
    for (size_t j = 0; j < m_row_; j++) {
      output_[i] += input_[i + j * m_col_];
    }
  }

  // std::cout <<"!!!!!!!!!!!!" <<std::endl;
  // for (size_t i = 0; i < m_col_; i++) std::cout << output_[i] << " ";
  // std::cout << std::endl;
  return true;
}

bool vladimirova_j_sum_matrix_column::TestTaskSequential::PostProcessingImpl() {
  for (size_t i = 0; i < output_.size(); i++) {
    reinterpret_cast<int *>(task_data->outputs[0])[i] = output_[i];
  }
  return true;
}
