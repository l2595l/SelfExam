//
// test_main.cpp —— 巡逻点调度器的验收测试
//
// 每个 case 独立构造数据，互不影响。
// 通过会打印 [PASS]，失败会打印 [FAIL] 并给出期望值与实际值。
//
// 注意：功能测试通过 ≠ 代码正确。某些缺陷只在内存层面暴露，
//       需要配合 run_tests.sh 的 AddressSanitizer 第二轮才能发现。
//
#include <cmath>
#include <cstdio>
#include <vector>

#include "patrol_scheduler.hpp"

static int g_pass = 0;
static int g_fail = 0;

static void check(bool ok, const char * name, const char * expect, const char * got)
{
  if (ok) {
    ++g_pass;
    std::printf("[PASS] %s\n", name);
  } else {
    ++g_fail;
    std::printf("[FAIL] %s\n       expected: %s\n       got     : %s\n", name, expect, got);
  }
}

static bool nearly(double a, double b)
{
  return std::fabs(a - b) < 1e-6;
}

int main()
{
  // ---- case01：averageDistance 精度 ----
  {
    PatrolScheduler s;
    s.addWaypoint({1, 3.0, 0.0});
    s.addWaypoint({2, 0.0, 4.0});

    const double avg = s.averageDistance(0.0, 0.0);
    char got[32];
    std::snprintf(got, sizeof(got), "%.6f", avg);
    check(nearly(avg, 3.5), "case01 averageDistance 应为 3.5", "3.500000", got);
  }

  // ---- case02：normalize 应作用于调用方数据 ----
  {
    std::vector<Waypoint> path;
    path.push_back({1, 1.4, 2.6});

    PatrolScheduler s;
    s.normalize(path);

    char got[64];
    std::snprintf(got, sizeof(got), "(%.1f, %.1f)", path[0].x, path[0].y);
    check(nearly(path[0].x, 1.0) && nearly(path[0].y, 3.0),
          "case02 normalize 后调用方数据应变为 (1.0, 3.0)", "(1.0, 3.0)", got);
  }

  // ---- case03：findById 返回值 ----
  {
    PatrolScheduler s;
    s.addWaypoint({10, 0.0, 0.0});
    s.addWaypoint({20, 1.0, 0.0});
    s.addWaypoint({30, 2.0, 0.0});

    const int idx = s.findById(20);
    char got[32];
    std::snprintf(got, sizeof(got), "%d", idx);
    check(idx == 1, "case03 findById(20) 应返回下标 1", "1", got);
  }

  // ---- case04：dropOutOfRange 删除数量 ----
  {
    PatrolScheduler s;
    s.addWaypoint({1, 0.0, 0.0});
    s.addWaypoint({2, 10.0, 0.0});
    s.addWaypoint({3, 11.0, 0.0});
    s.addWaypoint({4, 1.0, 1.0});

    s.dropOutOfRange(5.0);

    char got[32];
    std::snprintf(got, sizeof(got), "%zu", s.size());
    check(s.size() == 2, "case04 超出 5.0 的两点应被删除，剩 2 个", "2", got);
  }

  // ---- case05：totalDistance 累计值 ----
  {
    PatrolScheduler s;
    s.addWaypoint({1, 0.0, 0.0});
    s.addWaypoint({2, 3.0, 4.0});

    const double d = s.totalDistance();
    char got[32];
    std::snprintf(got, sizeof(got), "%.6f", d);
    check(nearly(d, 5.0), "case05 totalDistance 应为 5.0", "5.000000", got);
  }

  // ---- case06：lastAdded 指向最后加入的点 ----
  {
    PatrolScheduler s;
    for (int i = 1; i <= 200; ++i) {
      s.addWaypoint({i, static_cast<double>(i), 0.0});
    }

    const Waypoint * p = s.lastAdded();
    const int id = (p != nullptr) ? p->id : -1;
    char got[32];
    std::snprintf(got, sizeof(got), "%d", id);
    check(id == 200, "case06 lastAdded 应指向最后加入的点（id=200）", "200", got);
  }

  // ---- case07：longestRun 最长连续段 ----
  {
    PatrolScheduler s;
    s.addWaypoint({1, 0.0, 0.0});
    s.addWaypoint({2, 1.0, 0.0});
    s.addWaypoint({2, 2.0, 0.0});
    s.addWaypoint({3, 3.0, 0.0});

    const int run = s.longestRun();
    char got[32];
    std::snprintf(got, sizeof(got), "%d", run);
    check(run == 2, "case07 longestRun 应为 2（id 序列 1,2,2,3）", "2", got);
  }

  // ---- case08：nearestTo 基本正确性 ----
  {
    PatrolScheduler s;
    s.addWaypoint({7, 5.0, 5.0});

    const int id = s.nearestTo(0.0, 0.0);
    char got[32];
    std::snprintf(got, sizeof(got), "%d", id);
    check(id == 7, "case08 nearestTo 应返回唯一那个点的 id", "7", got);
  }

  // ---- 汇总 ----
  std::printf("\n----------------------------------------\n");
  std::printf("PASS: %d    FAIL: %d\n", g_pass, g_fail);
  std::printf("----------------------------------------\n");

  return (g_fail == 0) ? 0 : 1;
}
