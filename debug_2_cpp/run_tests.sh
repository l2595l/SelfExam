#!/usr/bin/env bash
#
# run_tests.sh —— 两轮验证
#   第一轮：普通构建，跑功能测试（列出所有 PASS/FAIL）
#   第二轮：AddressSanitizer 构建，捕获越界、悬垂指针等内存错误
#
set -u
cd "$(dirname "$0")" || exit 1

echo "========== 第一轮：功能测试 =========="
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug > /dev/null || exit 1
cmake --build build -j 4 || exit 1
./build/run_tests
functional=$?

echo
echo "========== 第二轮：AddressSanitizer =========="
cmake -S . -B build-asan -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="-fsanitize=address -fno-omit-frame-pointer -g" > /dev/null || exit 1
cmake --build build-asan -j 4 || exit 1
./build-asan/run_tests
asan=$?

echo
if [ "$functional" -eq 0 ] && [ "$asan" -eq 0 ]; then
  echo "RESULT: ALL PASS"
  exit 0
fi

echo "RESULT: FAILED  (functional exit=$functional, asan exit=$asan)"
exit 1
