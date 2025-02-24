
#include "seq/vladimirova_j_m_monte_karlo/include/ops_seq.hpp"

#include <cmath>
#include <cstddef>
#include <iostream>
#include <random>
#include <vector>

namespace vladimirova_j_m_monte_karlo {

std::random_device dev;
std::mt19937 random(dev());
double CreateRandomVal(int minV, int maxV, size_t decimal_places) {
  // Init value for input and output
  int d = maxV - minV;
  double f = random() % (d * decimal_places);
  double ans = (f / (double)(decimal_places)) + minV;
  return ans;
}

}  // namespace vladimirova_j_m_monte_karlo

bool vladimirova_j_m_monte_karlo::TestTaskSequential::PreProcessingImpl() {
  // Init value for input and output
  func = reinterpret_cast<bool (*)(std::vector<double>, size_t)>(task_data->inputs[2]);
  auto* in_ptr = reinterpret_cast<double*>(task_data->inputs[1]);
  std::vector<double> var_vect = std::vector<double>(in_ptr, in_ptr + var_size_);
  var_size_ /= 2;
  var_integr_ = std::vector<vladimirova_j_m_monte_karlo::boundaries_integral>(var_size_);
  for (size_t i = 0; i < var_size_; i++) {
    var_integr_[i].min = var_vect[i * 2];
    var_integr_[i].max = var_vect[(i * 2) + 1];
  }
  accuracy = reinterpret_cast<size_t>(task_data->inputs[3]);
  return true;
}

bool vladimirova_j_m_monte_karlo::TestTaskSequential::ValidationImpl() {
  // Check equality of counts elements
  // ��� ����� ����������,     ������ �����������,   ������ �� �������,    ��������

  var_size_ = reinterpret_cast<size_t>(task_data->inputs[0]);
  if ((var_size_ == 0) || (var_size_ % 2 != 0) || (var_size_ < 3)) {
    return false;
  }  // has variables
  auto* in_ptr = reinterpret_cast<double*>(task_data->inputs[1]);
  std::vector<double> var_vect = std::vector<double>(in_ptr, in_ptr + var_size_);
  for (size_t i = 0; i < var_size_; i += 2) {
    if (var_vect[i] >= var_vect[i + 1]) {
      return false;
    }  // x_min<x_max
  }
  return (task_data->inputs[2] != nullptr) && (reinterpret_cast<size_t>(task_data->inputs[3]) > 0);  // has funtion
  return true;
}

bool vladimirova_j_m_monte_karlo::TestTaskSequential::RunImpl() {
  // Multiply matrices
  size_t successful_point = 0;
  std::vector<double> random_val = std::vector<double>(var_size_);
  double tmp = (var_integr_[0].max - var_integr_[0].min);
  size_t decimal_places = 1;
  while ((tmp * decimal_places * var_size_) < accuracy) {
    decimal_places *= 10;
  }
  // std::cout <<"   ----------------- " <<decimal_places <<"  " << std::endl;
  for (size_t i = 0; i < accuracy; i++) {
    for (size_t j = 0; j < var_size_; j++) {
      random_val[j] = CreateRandomVal(var_integr_[j].min, var_integr_[j].max, decimal_places);
      // std::cout << " random " << random_val[j];
    }
    successful_point += (int)(func(random_val, var_size_));
    // std::cout << " res " << (int)(func(random_val, var_size_)) << std::endl;
  }
  double S = 1;
  int flag = 1;
  for (size_t i = 0; i < var_size_; i++) {
    S *= (var_integr_[i].max - var_integr_[i].min);
    if ((flag > 0) && (S < 0)) {
      flag *= -1;
    }
  }
  if (S < 0) {
    S *= -1;
  }
  S *= ((double)(successful_point) / accuracy);
  // std::cout << " !!!!!!!!!!!!  S= " << S << " var_s="<< var_size_ <<" coef="<< successful_point <<std::endl;
  output_.push_back(S);

  return true;
}

bool vladimirova_j_m_monte_karlo::TestTaskSequential::PostProcessingImpl() {
  reinterpret_cast<double*>(task_data->outputs[0])[0] = output_[0];
  return true;
}
