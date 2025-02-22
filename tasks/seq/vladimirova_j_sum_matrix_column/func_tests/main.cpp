#include <gtest/gtest.h>

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <memory>
#include <random>
#include <string>
#include <vector>

#include "core/task/include/task.hpp"
#include "core/util/include/util.hpp"
#include "seq/vladimirova_j_sum_matrix_column/include/ops_seq.hpp"

namespace vladimirova_j_sum_matrix_column {
std::vector<int> CreateRandomVector(size_t size, size_t spread_of_val) {
  // Init value for input and output
  std::random_device dev;
  std::mt19937 random(dev());
  std::vector<int> v(size);
  for (size_t i = 0; i < size; i++) {
    v[i] = (random() % (2 * spread_of_val + 1)) - spread_of_val;
  }
  return v;
}
}  // namespace vladimirova_j_sum_matrix_column

TEST(vladimirova_j_sum_matrix_column, test_sum_column_zero_1) {
  constexpr size_t kCount_r = 0;
  constexpr size_t kCount_c = 500;
  // Create data
  std::vector<int> in = vladimirova_j_sum_matrix_column::CreateRandomVector(kCount_c * kCount_r, 500);
  std::vector<int> out(kCount_c, 0);
  // for (size_t i = 0; i < kCount * kCount; i++) std::cout << in[i] << " ";
  // std::cout << std::endl;

  // Create task_data
  auto task_data_seq = std::make_shared<ppc::core::TaskData>();
  task_data_seq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  task_data_seq->inputs_count.emplace_back(kCount_c);
  task_data_seq->inputs_count.emplace_back(kCount_r);  // how many columns, how many rows
  task_data_seq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  task_data_seq->outputs_count.emplace_back(out.size());

  // Create Task
  vladimirova_j_sum_matrix_column::TestTaskSequential test_task_sequential(task_data_seq);
  ASSERT_EQ(test_task_sequential.Validation(), false);
}
TEST(vladimirova_j_sum_matrix_column, test_sum_column_zero_2) {
  constexpr size_t kCount_r = 500;
  constexpr size_t kCount_c = 0;
  // Create data
  std::vector<int> in = vladimirova_j_sum_matrix_column::CreateRandomVector(kCount_c * kCount_r, 500);
  std::vector<int> out(kCount_c, 0);
  // for (size_t i = 0; i < kCount * kCount; i++) std::cout << in[i] << " ";
  // std::cout << std::endl;

  // Create task_data
  auto task_data_seq = std::make_shared<ppc::core::TaskData>();
  task_data_seq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  task_data_seq->inputs_count.emplace_back(kCount_c);
  task_data_seq->inputs_count.emplace_back(kCount_r);  // how many columns, how many rows
  task_data_seq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  task_data_seq->outputs_count.emplace_back(out.size());

  // Create Task
  vladimirova_j_sum_matrix_column::TestTaskSequential test_task_sequential(task_data_seq);
  ASSERT_EQ(test_task_sequential.Validation(), false);
}
TEST(vladimirova_j_sum_matrix_column, test_sum_column_3) {
  constexpr size_t kCount = 3;

  // Create data
  std::vector<int> in = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::vector<int> out(kCount, 0);
  std::vector<int> ans = {12, 15, 18};

  // Create task_data
  auto task_data_seq = std::make_shared<ppc::core::TaskData>();
  task_data_seq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  task_data_seq->inputs_count.emplace_back(kCount);
  task_data_seq->inputs_count.emplace_back(kCount);  // how many columns, how many rows
  task_data_seq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  task_data_seq->outputs_count.emplace_back(out.size());

  // Create Task
  vladimirova_j_sum_matrix_column::TestTaskSequential test_task_sequential(task_data_seq);
  ASSERT_EQ(test_task_sequential.Validation(), true);
  test_task_sequential.PreProcessing();
  test_task_sequential.Run();
  test_task_sequential.PostProcessing();
  EXPECT_EQ(out, ans);
}
TEST(vladimirova_j_sum_matrix_column, test_sum_column_1x50x50) {
  constexpr size_t kCount = 50;

  // Create data
  std::vector<int> in = std::vector<int>(kCount * kCount, 1);
  std::vector<int> out(kCount, 0);
  // for (size_t i = 0; i < kCount * kCount; i++) std::cout << in[i] << " ";
  // std::cout << std::endl;
  std::vector<int> ans = std::vector<int>(kCount, kCount);

  // Create task_data
  auto task_data_seq = std::make_shared<ppc::core::TaskData>();
  task_data_seq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  task_data_seq->inputs_count.emplace_back(kCount);
  task_data_seq->inputs_count.emplace_back(kCount);
  task_data_seq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  task_data_seq->outputs_count.emplace_back(out.size());

  // Create Task
  vladimirova_j_sum_matrix_column::TestTaskSequential test_task_sequential(task_data_seq);
  ASSERT_EQ(test_task_sequential.Validation(), true);
  test_task_sequential.PreProcessing();
  test_task_sequential.Run();
  test_task_sequential.PostProcessing();
  EXPECT_EQ(out, ans);
}
TEST(vladimirova_j_sum_matrix_column, test_sum_column_1x100x50) {
  constexpr size_t kCount_r = 50;
  constexpr size_t kCount_c = 100;
  // Create data
  std::vector<int> in = std::vector<int>(kCount_c * kCount_r, 1);
  std::vector<int> out(kCount_c, 0);
  // for (size_t i = 0; i < kCount * kCount; i++) std::cout << in[i] << " ";
  // std::cout << std::endl;
  std::vector<int> ans = std::vector<int>(kCount_c, kCount_r);

  // Create task_data
  auto task_data_seq = std::make_shared<ppc::core::TaskData>();
  task_data_seq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  task_data_seq->inputs_count.emplace_back(kCount_c);
  task_data_seq->inputs_count.emplace_back(kCount_r);  // how many columns, how many rows
  task_data_seq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  task_data_seq->outputs_count.emplace_back(out.size());

  // Create Task
  vladimirova_j_sum_matrix_column::TestTaskSequential test_task_sequential(task_data_seq);
  ASSERT_EQ(test_task_sequential.Validation(), true);
  test_task_sequential.PreProcessing();
  test_task_sequential.Run();
  test_task_sequential.PostProcessing();
  EXPECT_EQ(out, ans);
}
TEST(vladimirova_j_sum_matrix_column, test_sum_column_1x500) {
  constexpr size_t kCount_r = 1;
  constexpr size_t kCount_c = 500;
  // Create data
  std::vector<int> in = vladimirova_j_sum_matrix_column::CreateRandomVector(kCount_c * kCount_r, 500);
  std::vector<int> out(kCount_c, 0);
  // for (size_t i = 0; i < kCount * kCount; i++) std::cout << in[i] << " ";
  // std::cout << std::endl;

  // Create task_data
  auto task_data_seq = std::make_shared<ppc::core::TaskData>();
  task_data_seq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  task_data_seq->inputs_count.emplace_back(kCount_c);
  task_data_seq->inputs_count.emplace_back(kCount_r);  // how many columns, how many rows
  task_data_seq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  task_data_seq->outputs_count.emplace_back(out.size());

  // Create Task
  vladimirova_j_sum_matrix_column::TestTaskSequential test_task_sequential(task_data_seq);
  ASSERT_EQ(test_task_sequential.Validation(), true);
  test_task_sequential.PreProcessing();
  test_task_sequential.Run();
  test_task_sequential.PostProcessing();
  EXPECT_EQ(out, in);
}
TEST(vladimirova_j_sum_matrix_column, test_sum_column_1_m1_50x50) {
  constexpr size_t kCount_r = 50;
  constexpr size_t kCount_c = 50;
  // Create data
  std::vector<int> in = std::vector<int>(kCount_r * kCount_c, 1);
  for (size_t i = kCount_r * kCount_c / 2; i < kCount_r * kCount_c; i++) in[i] *= -1;
  // 1  1  1  1
  // 1  1  1  1   =  0 0 0 0
  //-1 -1 -1 -1
  //-1 -1 -1 -1
  std::vector<int> out(kCount_c, 0);
  std::vector<int> ans(kCount_c, 0);
  // for (size_t i = 0; i < kCount * kCount; i++) std::cout << in[i] << " ";
  // std::cout << std::endl;

  // Create task_data
  auto task_data_seq = std::make_shared<ppc::core::TaskData>();
  task_data_seq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  task_data_seq->inputs_count.emplace_back(kCount_c);
  task_data_seq->inputs_count.emplace_back(kCount_r);  // how many columns, how many rows
  task_data_seq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  task_data_seq->outputs_count.emplace_back(out.size());

  // Create Task
  vladimirova_j_sum_matrix_column::TestTaskSequential test_task_sequential(task_data_seq);
  ASSERT_EQ(test_task_sequential.Validation(), true);
  test_task_sequential.PreProcessing();
  test_task_sequential.Run();
  test_task_sequential.PostProcessing();
  EXPECT_EQ(out, ans);
}
TEST(vladimirova_j_sum_matrix_column, test_sum_column_500x1) {
  constexpr size_t kCount_r = 500;
  constexpr size_t kCount_c = 1;
  // Create data
  std::vector<int> in = vladimirova_j_sum_matrix_column::CreateRandomVector(kCount_c * kCount_r, 500);
  std::vector<int> out(kCount_c, 0);
  std::vector<int> ans(kCount_c, 0);
  for (size_t i = 0; i < kCount_r; i++) ans[0] += in[i];
  // for (size_t i = 0; i < kCount * kCount; i++) std::cout << in[i] << " ";
  // std::cout << std::endl;

  // Create task_data
  auto task_data_seq = std::make_shared<ppc::core::TaskData>();
  task_data_seq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  task_data_seq->inputs_count.emplace_back(kCount_c);
  task_data_seq->inputs_count.emplace_back(kCount_r);  // how many columns, how many rows
  task_data_seq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  task_data_seq->outputs_count.emplace_back(out.size());

  // Create Task
  vladimirova_j_sum_matrix_column::TestTaskSequential test_task_sequential(task_data_seq);
  ASSERT_EQ(test_task_sequential.Validation(), true);
  test_task_sequential.PreProcessing();
  test_task_sequential.Run();

  test_task_sequential.PostProcessing();
  EXPECT_EQ(out, ans);
}
TEST(vladimirova_j_sum_matrix_column, test_sum_column_arif_pr_100x100_1) {
  constexpr size_t kCount = 100;

  // Create data
  std::vector<int> in = std::vector<int>(kCount * kCount);
  for (size_t i = 0; i < kCount * kCount; i++) in[i] = i;
  std::vector<int> out(kCount);
  // for (size_t i = 0; i < kCount * kCount; i++) std::cout << in[i] << " ";
  // std::cout << std::endl;
  std::vector<int> ans = std::vector<int>(kCount, 0);
  size_t tmp = kCount * (kCount - 1);
  for (size_t i = 0; i < kCount; i++) ans[i] = ((i * 2 + tmp) * kCount) / 2;

  // Create task_data
  auto task_data_seq = std::make_shared<ppc::core::TaskData>();
  task_data_seq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  task_data_seq->inputs_count.emplace_back(kCount);
  task_data_seq->inputs_count.emplace_back(kCount);
  task_data_seq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  task_data_seq->outputs_count.emplace_back(out.size());

  // Create Task
  vladimirova_j_sum_matrix_column::TestTaskSequential test_task_sequential(task_data_seq);
  ASSERT_EQ(test_task_sequential.Validation(), true);
  test_task_sequential.PreProcessing();
  test_task_sequential.Run();
  test_task_sequential.PostProcessing();
  EXPECT_EQ(out, ans);
}
TEST(vladimirova_j_sum_matrix_column, test_sum_column_arif_pr_100x50_1) {
  constexpr size_t kCount_r = 100;
  constexpr size_t kCount_c = 50;

  // Create data
  std::vector<int> in = std::vector<int>(kCount_r * kCount_c);
  for (size_t i = 0; i < kCount_r * kCount_c; i++) in[i] = i;
  std::vector<int> out(kCount_c);
  // for (size_t i = 0; i < kCount * kCount; i++) std::cout << in[i] << " ";
  // std::cout << std::endl;
  std::vector<int> ans = std::vector<int>(kCount_c, 0);
  size_t tmp = kCount_c * (kCount_r - 1);
  for (size_t i = 0; i < kCount_c; i++) ans[i] = ((i * 2 + tmp) * kCount_r) / 2;  // sum_pr = ((a1 + an)/2)*n

  // Create task_data
  auto task_data_seq = std::make_shared<ppc::core::TaskData>();
  task_data_seq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  task_data_seq->inputs_count.emplace_back(kCount_c);
  task_data_seq->inputs_count.emplace_back(kCount_r);  // how many columns, how many rows
  task_data_seq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  task_data_seq->outputs_count.emplace_back(out.size());

  // Create Task
  vladimirova_j_sum_matrix_column::TestTaskSequential test_task_sequential(task_data_seq);
  ASSERT_EQ(test_task_sequential.Validation(), true);
  test_task_sequential.PreProcessing();
  test_task_sequential.Run();
  test_task_sequential.PostProcessing();
  EXPECT_EQ(out, ans);
}
TEST(vladimirova_j_sum_matrix_column, test_sum_column_arif_pr_50x100_1) {
  constexpr size_t kCount_r = 50;
  constexpr size_t kCount_c = 100;

  // Create data
  std::vector<int> in = std::vector<int>(kCount_r * kCount_c);
  for (size_t i = 0; i < kCount_r * kCount_c; i++) in[i] = i;
  std::vector<int> out(kCount_c);
  // for (size_t i = 0; i < kCount * kCount; i++) std::cout << in[i] << " ";
  // std::cout << std::endl;
  std::vector<int> ans = std::vector<int>(kCount_c, 0);
  size_t tmp = kCount_c * (kCount_r - 1);
  for (size_t i = 0; i < kCount_c; i++) ans[i] = ((i * 2 + tmp) * kCount_r) / 2;  // sum_pr = ((a1 + an)/2)*n

  // Create task_data
  auto task_data_seq = std::make_shared<ppc::core::TaskData>();
  task_data_seq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  task_data_seq->inputs_count.emplace_back(kCount_c);
  task_data_seq->inputs_count.emplace_back(kCount_r);  // how many columns, how many rows
  task_data_seq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  task_data_seq->outputs_count.emplace_back(out.size());

  // Create Task
  vladimirova_j_sum_matrix_column::TestTaskSequential test_task_sequential(task_data_seq);
  ASSERT_EQ(test_task_sequential.Validation(), true);
  test_task_sequential.PreProcessing();
  test_task_sequential.Run();
  test_task_sequential.PostProcessing();
  EXPECT_EQ(out, ans);
}
TEST(vladimirova_j_sum_matrix_column, test_sum_column_geom_pr2_4x5_1) {
  constexpr size_t kCount_r = 4;
  constexpr size_t kCount_c = 5;
  size_t q = 2;
  // Create data
  std::vector<int> in = std::vector<int>(kCount_r * kCount_c);
  in[0] = 1;
  for (size_t i = 1; i < kCount_r * kCount_c; i++) in[i] = in[i - 1] * q;
  std::vector<int> out(kCount_c);
  // for (size_t i = 0; i < kCount_c * kCount_r; i++) std::cout << in[i] << " ";
  // std::cout << std::endl;
  std::vector<int> ans = std::vector<int>(kCount_c, 0);
  size_t tmp = kCount_c * (kCount_r - 1);
  q = in[kCount_c] / in[0];
  for (size_t i = 0; i < kCount_c; i++)
    ans[i] = ((in[i + tmp] * q) - in[i]) / (q - 1);  // sum_pr = (b1q^n - b1)/q-1 => (bn - b1)/ q-1;
  // Create task_data
  auto task_data_seq = std::make_shared<ppc::core::TaskData>();
  task_data_seq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  task_data_seq->inputs_count.emplace_back(kCount_c);
  task_data_seq->inputs_count.emplace_back(kCount_r);  // how many columns, how many rows
  task_data_seq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  task_data_seq->outputs_count.emplace_back(out.size());

  // Create Task
  vladimirova_j_sum_matrix_column::TestTaskSequential test_task_sequential(task_data_seq);
  ASSERT_EQ(test_task_sequential.Validation(), true);
  test_task_sequential.PreProcessing();
  test_task_sequential.Run();
  test_task_sequential.PostProcessing();
  EXPECT_EQ(out, ans);
}
TEST(vladimirova_j_sum_matrix_column, test_sum_column_geom_pr5_3x3_1) {
  constexpr size_t kCount_r = 3;
  constexpr size_t kCount_c = 3;
  size_t q = 3;
  // Create data
  std::vector<int> in = std::vector<int>(kCount_r * kCount_c);
  in[0] = 1;
  for (size_t i = 1; i < kCount_r * kCount_c; i++) in[i] = in[i - 1] * q;
  std::vector<int> out(kCount_c);
  // for (size_t i = 0; i < kCount_c * kCount_r; i++) std::cout << in[i] << " ";
  // std::cout << std::endl;
  std::vector<int> ans = std::vector<int>(kCount_c, 0);
  size_t tmp = kCount_c * (kCount_r - 1);
  q = in[kCount_c] / in[0];
  for (size_t i = 0; i < kCount_c; i++)
    ans[i] = ((in[i + tmp] * q) - in[i]) / (q - 1);  // sum_pr = (b1q^n - b1)/q-1 => (bn - b1)/ q-1;
  // Create task_data
  auto task_data_seq = std::make_shared<ppc::core::TaskData>();
  task_data_seq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  task_data_seq->inputs_count.emplace_back(kCount_c);
  task_data_seq->inputs_count.emplace_back(kCount_r);  // how many columns, how many rows
  task_data_seq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  task_data_seq->outputs_count.emplace_back(out.size());

  // Create Task
  vladimirova_j_sum_matrix_column::TestTaskSequential test_task_sequential(task_data_seq);
  ASSERT_EQ(test_task_sequential.Validation(), true);
  test_task_sequential.PreProcessing();
  test_task_sequential.Run();
  test_task_sequential.PostProcessing();
  EXPECT_EQ(out, ans);
}
