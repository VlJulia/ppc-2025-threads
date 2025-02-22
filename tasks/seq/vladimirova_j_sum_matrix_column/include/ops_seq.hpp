#pragma once

#include <utility>
#include <vector>

#include "core/task/include/task.hpp"
namespace vladimirova_j_sum_matrix_column {

class TestTaskSequential : public ppc::core::Task {
 public:
  explicit TestTaskSequential(ppc::core::TaskDataPtr task_data) : Task(std::move(task_data)) {}
  bool PreProcessingImpl() override;
  bool ValidationImpl() override;
  bool RunImpl() override;
  bool PostProcessingImpl() override;

 private:
  std::vector<int> input_, output_;
  size_t m_col_{}, m_row_{};
};

}  // namespace vladimirova_j_sum_matrix_column
   /*
    */