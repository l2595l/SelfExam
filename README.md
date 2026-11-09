# VANGUARD 算法组 · 模拟综合考核（Mock Exam）

> 本仓库为考核仓库。**请勿修改 `main` 分支**，所有改动提交到自己的分支并发起 PR。

---

## 一、考核信息

| 项目 | 说明 |
|---|---|
| 总时长 | **90 分钟** |
| Part A 客观题 | 20 题，闭卷，**限时 20 分钟**，满分 100（每题 5 分） |
| Part B Debug_1 | ROS2 环境与配置调试，**建议 20 分钟**，满分 100 |
| Part C Debug_2 | C++ 逻辑 Bug 修复，**建议 50 分钟**，满分 100 |

**时间铁律：开考 20 分钟后客观题停止作答；结束前 3 分钟无论完成度如何必须 commit + push。**

---

## 二、仓库结构

```
VANGUARD_Mock_Exam/
├── README.md                      ← 你正在看的文件
├── exam/
│   └── objective.md               ← Part A 客观题（20 题）
├── debug_1_ros2/                  ← Part B：ROS2 工作空间
│   └── src/
│       ├── patrol_interfaces/     ← 接口包（没有错误，作为参照）
│       └── patrol_controller/     ← 节点包（错误都在这里）
├── debug_2_cpp/                   ← Part C：C++ 项目
│   ├── include/patrol_scheduler.hpp
│   ├── src/patrol_scheduler.cpp
│   ├── tests/test_main.cpp
│   └── run_tests.sh
└── .github/workflows/ci.yml       ← 自动检查
```

---

## 三、Part A · 客观题

题目在 `exam/objective.md`。**闭卷**，20 分钟内完成。

答题方式：新建 `exam/answers.md`，格式如下（多选题把选项连写）：

```markdown
1. B
2. A
...
20. AB
```

---

## 四、Part B · Debug_1（ROS2）

`patrol_controller` 包**当前无法正常编译与运行**，包内埋了 **6 处**与环境、配置相关的问题（**不含逻辑错误**）。

你的任务：

1. 让 `colcon build` 通过
2. 让 `ros2 run patrol_controller patrol_node` 能启动
3. 让 `ros2 launch patrol_controller patrol.launch.py` 能启动
4. 让节点**能收到自己发布的消息**（终端应打印 `[patrol] target=... battery=...`）

环境准备：

```bash
cd debug_1_ros2
source /opt/ros/humble/setup.bash
colcon build
source install/setup.bash
```

**评分看过程**：能否复现、能否依据终端输出与 `ros2` 信息命令缩小范围、能否正确定位、是否只改配置相关文件。
**不允许**通过删除节点、注释 `find_package`、硬编码、删除测试等方式绕过。

---

## 五、Part C · Debug_2（C++）

`debug_2_cpp` 是一个「巡逻点调度器」。代码**能编译、能运行**，但存在 **8 个逻辑 Bug**，表现为单元测试失败或未定义行为。

```bash
cd debug_2_cpp
./run_tests.sh
```

脚本先跑功能测试（列出 PASS/FAIL），再用 AddressSanitizer 跑第二轮（捕获越界、悬垂指针等内存错误）。

**按 Bug 点计分，分值由难度决定，总分 100。** 修不完也要用注释写清「现象 + 定位依据」——按过程给分。

---

## 六、提交方式（务必按此流程）

### 1. Fork 并克隆

网页上点 Fork，然后：

```bash
git clone git@github.com:<你的用户名>/VANGUARD_Mock_Exam.git
cd VANGUARD_Mock_Exam
```

### 2. 配置身份（已配置可跳过）

```bash
git config --global user.name  "你的姓名"
git config --global user.email "你的邮箱"
git config --global --list
```

### 3. 建立自己的分支（姓名全拼）

```bash
git checkout -b ZhangSan
git branch            # 星号必须在自己的分支上
```

### 4. 修改、提交、推送

```bash
git status
git diff              # 提交前必看，确认没有误改无关文件
git add <你改的文件>
git commit -m "fix: ..."
git push -u origin ZhangSan
```

### 5. 创建 PR

- **base**：原仓库 `main`
- **head**：你的 `ZhangSan` 分支
- **标题**：你的姓名
- **PR 只需创建一次**，之后的 `git push` 会自动更新

---

## 七、红线（触碰即不合格）

1. 修改、删除、重写 `main` 分支，或改写已有 commit 历史
2. `git push --force`、对已推送的 commit 使用 `--amend`
3. 修改测试文件、测试期望值，或用硬编码、删功能让测试通过
4. 把本地调试用的测试改动一起提交

---

## 八、计分

| 部分 | 权重 | 说明 |
|---|---|---|
| A 客观题 | 30% | 按答案对错 |
| B Debug_1 | 25% | 过程分：定位路径 > 修复结果 |
| C Debug_2 | 45% | 按 Bug 点计分，难度加权 |

**CI 通过不代表满分**：CI 只覆盖测试用例写到的点，最终以人工检查实际改动为准。
