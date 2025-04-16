
#include "stl/vladimirova_j_m_monte_karlo_stl/include/ops_stl.hpp"

#include <cmath>
#include <cstddef>
#include <execution>
#include <iostream>
#include <numeric>
#include <random>
#include <utility>
#include <vector>

namespace {

thread_local std::mt19937 gen(std::random_device{}());

double CreateRandomVal(double min_v, double max_v) {
  std::uniform_real_distribution<double> dis(min_v, max_v);
  return dis(gen);
}

}  // namespace

bool vladimirova_j_m_monte_karlo_stl::TestTaskStl::PreProcessingImpl() {
  // Init value for input and output
  func_ = reinterpret_cast<bool (*)(std::vector<double>, size_t)>(task_data->inputs[1]);
  auto* in_ptr = reinterpret_cast<double*>(task_data->inputs[0]);
  std::vector<double> var_vect = std::vector<double>(in_ptr, in_ptr + var_size_);
  var_size_ /= 2;
  var_integr_ = std::vector<vladimirova_j_m_monte_karlo_stl::BoundariesIntegral>(var_size_);
  for (size_t i = 0; i < var_size_; i++) {
    var_integr_[i].min = var_vect[i * 2];
    var_integr_[i].max = var_vect[(i * 2) + 1];
  }
  accuracy_ = reinterpret_cast<size_t>(task_data->inputs[2]);
  return true;
}

bool vladimirova_j_m_monte_karlo_stl::TestTaskStl::ValidationImpl() {
  // Check equality of counts elements
  var_size_ = task_data->inputs_count[0];

  if ((var_size_ % 2 != 0) || (var_size_ < 3)) {
    return false;
  }  // has variables
  auto* in_ptr = reinterpret_cast<double*>(task_data->inputs[0]);
  std::vector<double> var_vect = std::vector<double>(in_ptr, in_ptr + var_size_);
  for (size_t i = 0; i < var_size_; i += 2) {
    if (var_vect[i] >= var_vect[i + 1]) {
      return false;
    }  // x_min<x_max
  }
  return (task_data->inputs[1] != nullptr) && (reinterpret_cast<size_t>(task_data->inputs[2]) > 0);  // has funtion

  return true;
}

bool vladimirova_j_m_monte_karlo_stl::TestTaskStl::RunImpl() {
  size_t successful_point = 0;
  std::vector<size_t> successful_points(accuracy_, 0);
  std::for_each(std::execution::par, successful_points.begin(), successful_points.end(),
                [&](size_t& local_successful_point) {
                  std::vector<double> local_random_val(var_size_);
                  for (size_t j = 0; j < var_size_; j++) {
                    local_random_val[j] = CreateRandomVal(var_integr_[j].min, var_integr_[j].max);
                  }
                  local_successful_point = static_cast<size_t>(func_(local_random_val, var_size_));
                });
  successful_point = std::accumulate(successful_points.begin(), successful_points.end(), 0);

  double s = 1;
  for (size_t i = 0; i < var_size_; i++) {
    s *= (var_integr_[i].max - var_integr_[i].min);
  }
  s *= static_cast<double>(successful_point) / static_cast<double>(accuracy_);
  output_.push_back(s);
  return true;
}

bool vladimirova_j_m_monte_karlo_stl::TestTaskStl::PostProcessingImpl() {
  reinterpret_cast<double*>(task_data->outputs[0])[0] = output_[0];
  return true;
}
