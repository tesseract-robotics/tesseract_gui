/**
 * @file joint_trajectory_set.h
 * @brief Tesseract Joint Trajectory Trajectory Set
 *
 * @author Levi Armstrong
 * @date December 5, 2021
 * @version TODO
 * @bug No known bugs
 *
 * @copyright Copyright (c) 2021, Southwest Research Institute
 *
 * @par License
 * Software License Agreement (Apache License)
 * @par
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * @par
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef TESSERACT_COMMON_JOINT_TRAJECTORY_SET_H
#define TESSERACT_COMMON_JOINT_TRAJECTORY_SET_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <Eigen/Dense>
#include <vector>
#include <string>
#include <boost/serialization/base_object.hpp>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_environment/environment.h>
#include <tesseract_command_language/composite_instruction.h>
#include <tesseract_common/joint_state.h>

namespace tesseract_common
{
struct JointTrajectoryInfo
{
  JointTrajectoryInfo() = default;

  /** @brief The initial state of the environment */
  JointState initial_state;

  /** @brief (Optional) Additional Commands to be applied to trajectory set environment prior to trajectory visualization */
  tesseract_environment::Commands commands;

  /** @brief The joint trajectory */
  JointTrajectory trajectory;

  /** @brief A description of the trajectory */
  std::string description;

private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version);  // NOLINT
};

class JointTrajectorySet
{
public:
  /** @brief This is only provided for serialization, DO NOT USE */
  JointTrajectorySet() = default;

  /**
   * @brief Create a trajectory set with initial state
   * @param initial_state The initial state of the environment
   * @param environment (Optional) The environment to use for planning
   */
  JointTrajectorySet(const std::unordered_map<std::string, double>& initial_state,
                     tesseract_environment::Environment::Ptr environment = nullptr);

  /**
   * @brief Create a trajectory set with initial state and environment commands
   * @param initial_state The initial state of the environment
   * @param commands Additional Commands to be applied to environment prior to planning
   */
  JointTrajectorySet(const std::unordered_map<std::string, double>& initial_state,
                     tesseract_environment::Commands commands);

  /**
   * @brief Append a Joint Trajectory
   * @brief This will adjust the time because each trajectory most likely starts from zero
   * @param key The key to store the trajectory under
   * @param joint_trajectory The joint trajectory to append
   * @param description A description of the trajectory
   */
  void appendJointTrajectory(const JointTrajectory& joint_trajectory, const std::string& description);

  /**
   * @brief Get the environment for the joint trajectory set
   * @details This can be a nullptr. If nullptr then check for commands which should be applied to the existing environment.
   * @return The environment
   */
  tesseract_environment::Environment::Ptr getEnvironment() const;

  /**
   * @brief Get the environment commands that were applied to existing environment for the joint trajectory set
   * @return The environment commands
   */
  const tesseract_environment::Commands& getEnvironmentCommands() const;

  /**
   * @brief Get the initial state
   * @return The initial state
   */
  const JointState& getInitialState() const;

  /**
   * @brief Get the scene trajectory
   * @return The trajectory
   */
  const std::vector<JointTrajectoryInfo>& getJointTrajectories() const;

  std::size_t size() const;

  /** @brief access specified element */
  std::vector<JointTrajectoryInfo>::reference operator[](std::size_t pos);
  std::vector<JointTrajectoryInfo>::const_reference operator[](std::size_t pos) const;

private:
  /** @brief The initial state of the environment */
  JointState initial_state_;

  /** @brief A vector of joint trajectory in the set */
  std::vector<JointTrajectoryInfo> joint_trajectory_;

  /** @brief (Optional) Override the existing Tesseract Environment */
  tesseract_environment::Environment::Ptr environment_{nullptr};

  /** @brief (Optional) Additional Commands to be applied to environment prior to trajectory visualization */
  tesseract_environment::Commands commands_;

  /**
   * @brief Append a joint state to the end
   * @details It assumes the state time is a delta time which gets add to the current trajectory time.
   * @param joint_state The joint state to append
   */
  void appendJointState(JointTrajectoryInfo& traj_info, const JointState& joint_state);

  JointState getNewTrajectoryInitialState() const;

  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version);  // NOLINT
};
}  // namespace tesseract_common
#endif  // TESSERACT_COMMON_JOINT_TRAJECTORY_SET_H
