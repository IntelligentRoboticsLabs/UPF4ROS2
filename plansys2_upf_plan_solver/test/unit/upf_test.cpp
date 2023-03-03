// Copyright 2019 Intelligent Robotics Lab
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>


#include "ament_index_cpp/get_package_share_directory.hpp"

#include "gtest/gtest.h"
#include "plansys2_upf_plan_solver/upf_plan_solver.hpp"

#include "pluginlib/class_loader.hpp"
#include "pluginlib/class_list_macros.hpp"
#include "plansys2_core/PlanSolverBase.hpp"

void test_plan_generation(const std::string & argument = "")
{
  std::string pkgpath = ament_index_cpp::get_package_share_directory("plansys2_upf_plan_solver");
  std::ifstream domain_ifs(pkgpath + "/pddl/domain_simple.pddl");
  std::string domain_str((
      std::istreambuf_iterator<char>(domain_ifs)),
    std::istreambuf_iterator<char>());

  std::ifstream problem_ifs(pkgpath + "/pddl/problem_simple_1.pddl");
  std::string problem_str((
      std::istreambuf_iterator<char>(problem_ifs)),
    std::istreambuf_iterator<char>());

  auto node = rclcpp_lifecycle::LifecycleNode::make_shared("test_node");
  auto planner = std::make_shared<plansys2::UPFPlanSolver>();
  planner->configure(node, "UPF");
  node->set_parameter(rclcpp::Parameter("UPF.arguments", argument));

  auto plan = planner->getPlan(domain_str, problem_str, "generate_plan_good");

  ASSERT_TRUE(plan);
  ASSERT_EQ(plan.value().items.size(), 3);
  
  std::vector<std::string> actions;
  for (const auto & action : plan.value().items) {
    std::string current = action.action;
    std::transform(current.begin(),current.end(),current.begin(), 
      [](unsigned char c){ return std::tolower(c); });
    actions.push_back(current);
  }
  
  ASSERT_EQ(actions[0], "(move leia kitchen bedroom)");
  ASSERT_EQ(actions[1], "(approach leia bedroom jack)");
  ASSERT_EQ(actions[2], "(talk leia jack jack m1)");
}

TEST(upf_plan_solver, generate_plan_good)
{
  test_plan_generation();
}

TEST(upf_plan_solver, generate_plan_good_with_argument)
{
  test_plan_generation("-e");
}

TEST(upf_plan_solver, load_upf_plugin)
{
  try {
    pluginlib::ClassLoader<plansys2::PlanSolverBase> lp_loader(
      "plansys2_core", "plansys2::PlanSolverBase");
    plansys2::PlanSolverBase::Ptr plugin =
      lp_loader.createUniqueInstance("plansys2/UPFPlanSolver");
    ASSERT_TRUE(true);
  } catch (std::exception & e) {
    std::cerr << e.what() << std::endl;
    ASSERT_TRUE(false);
  }
}

TEST(upf_plan_solver, check_1_ok_domain)
{
  std::string pkgpath = ament_index_cpp::get_package_share_directory("plansys2_upf_plan_solver");
  std::ifstream domain_ifs(pkgpath + "/pddl/domain_1_ok.pddl");
  std::string domain_str((
      std::istreambuf_iterator<char>(domain_ifs)),
    std::istreambuf_iterator<char>());

  auto node = rclcpp_lifecycle::LifecycleNode::make_shared("test_node");
  auto planner = std::make_shared<plansys2::UPFPlanSolver>();
  planner->configure(node, "UPF");

  bool result = planner->is_valid_domain(domain_str, "check_1_ok_domain");

  ASSERT_TRUE(result);
}


TEST(upf_plan_solver, check_2_error_domain)
{
  std::string pkgpath = ament_index_cpp::get_package_share_directory("plansys2_upf_plan_solver");
  std::ifstream domain_ifs(pkgpath + "/pddl/domain_2_error.pddl");
  std::string domain_str((
      std::istreambuf_iterator<char>(domain_ifs)),
    std::istreambuf_iterator<char>());

  auto node = rclcpp_lifecycle::LifecycleNode::make_shared("test_node");
  auto planner = std::make_shared<plansys2::UPFPlanSolver>();
  planner->configure(node, "UPF");

  bool result = planner->is_valid_domain(domain_str, "check_2_error_domain");

  ASSERT_FALSE(result);
}


TEST(upf_plan_solver, generate_plan_unsolvable)
{
  std::string pkgpath = ament_index_cpp::get_package_share_directory("plansys2_upf_plan_solver");
  std::ifstream domain_ifs(pkgpath + "/pddl/domain_simple.pddl");
  std::string domain_str((
      std::istreambuf_iterator<char>(domain_ifs)),
    std::istreambuf_iterator<char>());

  std::ifstream problem_ifs(pkgpath + "/pddl/problem_simple_2.pddl");
  std::string problem_str((
      std::istreambuf_iterator<char>(problem_ifs)),
    std::istreambuf_iterator<char>());

  auto node = rclcpp_lifecycle::LifecycleNode::make_shared("test_node");
  auto planner = std::make_shared<plansys2::UPFPlanSolver>();
  planner->configure(node, "UPF");

  auto plan = planner->getPlan(domain_str, problem_str);

  ASSERT_FALSE(plan);
}

TEST(upf_plan_solver, generate_plan_error)
{
  std::string pkgpath = ament_index_cpp::get_package_share_directory("plansys2_upf_plan_solver");
  std::ifstream domain_ifs(pkgpath + "/pddl/domain_simple.pddl");
  std::string domain_str((
      std::istreambuf_iterator<char>(domain_ifs)),
    std::istreambuf_iterator<char>());

  std::ifstream problem_ifs(pkgpath + "/pddl/problem_simple_3.pddl");
  std::string problem_str((
      std::istreambuf_iterator<char>(problem_ifs)),
    std::istreambuf_iterator<char>());

  auto node = rclcpp_lifecycle::LifecycleNode::make_shared("test_node");
  auto planner = std::make_shared<plansys2::UPFPlanSolver>();
  planner->configure(node, "UPF");

  auto plan = planner->getPlan(domain_str, problem_str);

  ASSERT_FALSE(plan);
}

int main(int argc, char ** argv)
{
  testing::InitGoogleTest(&argc, argv);
  rclcpp::init(argc, argv);

  return RUN_ALL_TESTS();
}
