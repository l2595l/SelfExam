# Part A · 客观题（20 题 / 20 分钟 / 闭卷）

**答题方式**：在 `exam/answers.md` 中按题号写答案。多选题把所有正确选项连写（例如 `AB`）。
每题 5 分，满分 100。**不倒扣**，不确定也要作答。

---

## 一、构建系统与依赖（5 题）

**1.** 同一工作空间里包 `a` 依赖包 `b`。执行 `colcon build` 时的构建顺序由什么决定？

- A. 包名字母序
- B. 目录创建时间
- C. `package.xml` 中声明的依赖关系（拓扑排序）
- D. 必须手动先单独编译 `b`

**2.** 执行 `colcon build --packages-select a` 的实际行为是？

- A. 编译 `a` 及其全部依赖
- B. 只编译 `a`，不编译它的依赖
- C. 编译除 `a` 以外的所有包
- D. 编译 `a` 并跳过测试

**3.** `package.xml` 中写 `<depend>foo</depend>`，等价于同时声明了：

- A. `<build_depend>` + `<exec_depend>`（含 build_export）
- B. 仅 `<build_depend>`
- C. 仅 `<exec_depend>`
- D. `<test_depend>` + `<build_depend>`

**4.** 使用方包的 `package.xml` 已写 `<depend>my_interfaces</depend>`，但 `CMakeLists.txt` 的 `ament_target_dependencies` 里**漏**了 `my_interfaces`。最可能的结果是：

- A. 编译阶段报 `fatal error: my_interfaces/msg/xxx.hpp: No such file or directory`
- B. 编译通过，运行时崩溃
- C. 一切正常
- D. 只有 Python 节点受影响

**5.** 关于 `colcon build --symlink-install`，正确的是：

- A. 所有产物都用软链接，C++ 源码改动后也不需要重新编译
- B. Python 脚本、launch、yaml 等使用软链接，改动后无需重编；C++ 仍需重新编译
- C. 只影响编译速度，产物与普通 build 完全相同
- D. 只对 Python 包有效

---

## 二、安装规则（4 题）

**6.** 以下哪条 `install()` 能让 `ros2 run my_pkg my_node` 找到可执行文件？

- A. `install(TARGETS my_node DESTINATION bin)`
- B. `install(TARGETS my_node DESTINATION lib/${PROJECT_NAME})`
- C. `install(FILES my_node DESTINATION lib)`
- D. `install(PROGRAMS my_node DESTINATION share/${PROJECT_NAME})`

**7.** `install(DIRECTORY config DESTINATION share/${PROJECT_NAME})` 与 `install(DIRECTORY config/ DESTINATION share/${PROJECT_NAME})` 的区别是：

- A. 完全等价
- B. 前者安装为 `share/pkg/config/`，后者把 `config` 的**内容**直接铺到 `share/pkg/`
- C. 后者会报错
- D. 前者只创建空目录

**8.** `ros2 pkg prefix --share my_pkg` 通常输出：

- A. 该包的源码目录
- B. `.../install/my_pkg/share/my_pkg`
- C. `.../build/my_pkg`
- D. `/opt/ros/humble/share/my_pkg`（无论什么包都固定如此）

**9.** 某个包 `colcon build` 成功、`ros2 pkg list` 也能看到，但 `ros2 run my_pkg my_node` 报 `No executable found`。最可能的原因是：

- A. 忘记 `source install/setup.bash`
- B. `CMakeLists.txt` 缺少 `install(TARGETS ...)`，或 `DESTINATION` 写成了 `bin`
- C. 节点里有语法错误
- D. 消息类型定义错误

---

## 三、接口（3 题）

**10.** 一个包里有 `msg/Status.msg` 与 `action/Fibonacci.action`。关于编译产物，正确的是：

- A. 会生成 `msg/status.hpp`、`msg/fibonacci_goal.hpp`、`msg/fibonacci_result.hpp`、`msg/fibonacci_feedback.hpp` 四个头文件
- B. 生成 `msg/status.hpp` 和 `action/fibonacci.hpp`；`Fibonacci` 的 `Goal`/`Result`/`Feedback` 是 `fibonacci.hpp` 内的**嵌套结构体**，不会单独生成 msg 头文件
- C. action 必须额外提供一个 msg 文件才能编译
- D. action 不能包含 feedback 段

**11.** 一个接口包只写了 `<buildtool_depend>rosidl_default_generators</buildtool_depend>`，**漏**了 `<member_of_group>rosidl_interface_packages</member_of_group>`。最典型的表现是：

- A. 该包自身编译失败，报错信息很明确
- B. 该包能编过，但它作为「接口包」的注册信息不完整，其它包在使用其接口时可能找不到 typesupport
- C. 只影响 Python 生成
- D. 会导致所有 C++ 节点无法启动

**12.** 节点代码 `create_publisher<std_msgs::msg::String>("chatter", 10)` 中，第二个参数 `10` 表示：

- A. 队列深度（history depth）
- B. 超时时间（秒）
- C. 最大订阅者数量
- D. 单条消息大小上限

---

## 四、通信与 QoS（4 题）

**13.** 发布端 `RELIABLE`、订阅端 `BEST_EFFORT`，结果是：

- A. 连不上
- B. 能连上，订阅端按 best_effort 实际收
- C. 系统自动把订阅端升级为 reliable
- D. 编译报错

**14.** 发布端 `BEST_EFFORT`、订阅端 `RELIABLE`，结果是：

- A. 连不上，且通常不报错
- B. 能连上，只是会丢包
- C. 系统自动降级为 best_effort
- D. 节点立即崩溃

**15.** `ros2 topic echo /chatter` 无任何输出，但 `ros2 topic hz /chatter` 显示稳定频率。最可能的原因：

- A. 发布端没在发布
- B. QoS 的 reliability/durability 两侧不兼容
- C. 话题名写错了
- D. 消息类型不匹配

**16.** 节点代码订阅 `"status"`（相对名），启动时加了 `--ros-args -r __ns:=/robot1`。实际订阅的话题是：

- A. `/status`
- B. `/robot1/status`
- C. `status`
- D. `/listener/status`

---

## 五、工具链与调试（2 题）

**17.** 只想知道某个包**一共定义了哪些** msg / srv / action，最直接的方式是：

- A. `ros2 pkg executables <包名>`
- B. `ros2 interface list | grep <包名>`
- C. `ros2 node list`
- D. `ros2 topic list -t`

**18.** 改了 `msg/Status.msg`（新增一个字段）后，只执行 `colcon build --packages-select <使用方包>`。最可能的现象是：

- A. 使用方立刻看到新字段
- B. 使用方仍链接到旧的生成代码，新字段报 `is not a member of`
- C. 编译直接失败，提示找不到 .msg
- D. 一切正常，rosidl 会自动增量生成

---

## 六、Git 与流程（1 题）

**19.** 执行了 `git branch ZhangSan`（只创建未切换），接着改代码并 `git commit`，再 `git push -u origin ZhangSan`。结果是：

- A. push 失败，提示分支不存在
- B. push 成功，但 PR 显示 0 files changed
- C. commit 被自动转移到 `ZhangSan` 分支
- D. 报错，提示必须先切换分支

---

## 七、多选（1 题）

**20.** （多选）以下哪些情况会导致 `ros2 run my_pkg talker` 报 `No executable found`？

- A. `CMakeLists.txt` 缺少 `install(TARGETS talker ...)`
- B. `install` 的 `DESTINATION` 写成了 `bin` 而不是 `lib/${PROJECT_NAME}`
- C. 当前终端没有 `source install/setup.bash`
- D. 编译时用了 `--symlink-install`

---

> 完成后把答案写入 `exam/answers.md`，再进入 Part B。
