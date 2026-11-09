#include "patrol_scheduler.hpp"

#include <cfloat>
#include <cmath>

void PatrolScheduler::addWaypoint(const Waypoint & wp)
{
  if (!points_.empty()) {
    const Waypoint & prev = points_.back();
    const double dx = wp.x - prev.x;
    const double dy = wp.y - prev.y;
    total_distance_ += std::sqrt(dx * dx + dy * dy);
  }

  points_.push_back(wp);
  last_added_ = &points_.back();
}

void PatrolScheduler::normalize(std::vector<Waypoint> path)
{
  for (auto & wp : path) {
    wp.x = std::round(wp.x);
    wp.y = std::round(wp.y);
  }
}

double PatrolScheduler::averageDistance(double cx, double cy) const
{
  if (points_.empty()) {
    return 0.0;
  }

  int sum = 0;
  for (const auto & wp : points_) {
    const double dx = wp.x - cx;
    const double dy = wp.y - cy;
    sum += static_cast<int>(std::sqrt(dx * dx + dy * dy));
  }

  return sum / points_.size();
}

double PatrolScheduler::totalDistance() const
{
  return total_distance_;
}

int PatrolScheduler::nearestTo(double x, double y) const
{
  double best_dist = DBL_MAX;
  int best_id = -1;

  for (std::size_t i = 0; i <= points_.size(); ++i) {
    const double dx = points_[i].x - x;
    const double dy = points_[i].y - y;
    const double dist = dx * dx + dy * dy;
    if (dist < best_dist) {
      best_dist = dist;
      best_id = points_[i].id;
    }
  }

  return best_id;
}

int PatrolScheduler::findById(int id)
{
  for (std::size_t i = 0; i < points_.size(); ++i) {
    if (points_[i].id = id) {
      return static_cast<int>(i);
    }
  }
  return -1;
}

void PatrolScheduler::dropOutOfRange(double max_range)
{
  for (auto it = points_.begin(); it != points_.end(); ++it) {
    const double dist = std::sqrt(it->x * it->x + it->y * it->y);
    if (dist > max_range) {
      points_.erase(it);
    }
  }
}

int PatrolScheduler::longestRun() const
{
  if (points_.empty()) {
    return 0;
  }

  int best = 1;
  int cur = 1;

  for (std::size_t i = 1; i < points_.size(); ++i) {
    if (points_[i].id == points_[i - 1].id) {
      ++cur;
      if (cur > best) {
        best = cur;
      }
    } else {
      cur = 0;
    }
  }

  return best;
}

const Waypoint * PatrolScheduler::lastAdded() const
{
  return last_added_;
}

std::size_t PatrolScheduler::size() const
{
  return points_.size();
}
