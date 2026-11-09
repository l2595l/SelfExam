#ifndef PATROL_SCHEDULER_HPP
#define PATROL_SCHEDULER_HPP

#include <cstddef>
#include <vector>

struct Waypoint
{
  int id;
  double x;
  double y;
};

/// 巡逻点调度器：维护一串巡逻点，并提供查询与筛选能力。
class PatrolScheduler
{
public:
  /// 追加一个巡逻点
  void addWaypoint(const Waypoint & wp);

  /// 把一串巡逻点的坐标取整到最近整数
  void normalize(std::vector<Waypoint> path);

  /// 所有巡逻点到 (cx, cy) 的平均距离
  double averageDistance(double cx, double cy) const;

  /// 按加入顺序累计走过的总路程
  double totalDistance() const;

  /// 找到离 (x, y) 最近的巡逻点，返回其 id；无点时返回 -1
  int nearestTo(double x, double y) const;

  /// 按 id 查找，返回下标；找不到返回 -1
  int findById(int id);

  /// 删除所有到原点距离超过 max_range 的点
  void dropOutOfRange(double max_range);

  /// 最长的一段「id 连续相同」的长度
  int longestRun() const;

  /// 最后加入的那个点
  const Waypoint * lastAdded() const;

  std::size_t size() const;

private:
  std::vector<Waypoint> points_;
  double total_distance_;
  const Waypoint * last_added_ = nullptr;
};

#endif  // PATROL_SCHEDULER_HPP
