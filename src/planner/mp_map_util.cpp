#include <planner/mp_map_util.h>

using namespace MPL;

MPMapUtil::MPMapUtil(bool verbose) {
  planner_verbose_ = verbose;
  if(planner_verbose_)
    printf(ANSI_COLOR_CYAN "[MPPlanner] PLANNER VERBOSE ON\n" ANSI_COLOR_RESET);
}

void MPMapUtil::setMapUtil(std::shared_ptr<VoxelMapUtil> map_util) {
  ENV_.reset(new MPL::env_map(map_util));
}

void MPMapUtil::setMapUtil(std::shared_ptr<SubVoxelMapUtil> map_util) {
  ENV_.reset(new MPL::env_map(map_util));
}

bool MPMapUtil::plan(const Waypoint &start, const Waypoint &goal) {
  //path_.clear();
  //primitives_.clear();

  if(planner_verbose_) {
    printf("start pos: [%f, %f, %f], vel: [%f, %f, %f], acc: [%f, %f, %f]\n",
        start.pos(0), start.pos(1), start.pos(2),
        start.vel(0), start.vel(1), start.vel(2),
        start.acc(0), start.acc(1), start.acc(2));
    printf("goal pos: [%f, %f, %f], vel: [%f, %f, %f], acc: [%f, %f, %f]\n",
        goal.pos(0), goal.pos(1), goal.pos(2),
        goal.vel(0), goal.vel(1), goal.vel(2),
        goal.acc(0), goal.acc(1), goal.acc(2));
  }

  if(!ENV_->is_free(start.pos)) {
    printf(ANSI_COLOR_RED "[MPPlanner] start is not free!" ANSI_COLOR_RESET "\n");
    return false;
  }
 
  MPL::ARAStar<Waypoint> AA;
  std::vector<double> action_dts;
  Trajectory traj;

  ENV_->set_goal(goal);

  AA.Astar(start, ENV_->state_to_idx(start), *ENV_, traj, action_dts, epsilon_, max_num_);

  traj_ = traj;
  if (traj.segs.empty()) {
    if(planner_verbose_)
      printf(ANSI_COLOR_RED "[MPPlanner] Cannot find a traj!" ANSI_COLOR_RESET "\n");
    return false;
  }

  /*
  path_.push_back(start);
  for (auto &it_node : path) {
    it_node.use_pos = start.use_pos;
    it_node.use_vel = start.use_vel;
    it_node.use_acc = start.use_acc;
    path_.push_back(it_node);
  }
  */

  //dts_ = action_dts;
  //std::reverse(dts_.begin(), dts_.end());
  return true;
}


