# JackAssembler

JackAssembler 是一个用于将 Hack 汇编语言（.asm 文件）翻译为 Hack 机器码（.hack 文件）的命令行工具，适用于 Nand2Tetris 课程相关项目。

## 特性

- 支持标准 Hack 汇编到机器码的转换
- 命令行参数友好，支持输入输出文件自定义
- 代码结构清晰，易于扩展和维护

## 用法

```sh
./JackAssembler <input_file> <output_file>
```

或使用选项：

```sh
./JackAssembler --help
```

### 示例

```sh
./JackAssembler test/add/Add.asm test/add/Add.hack
```

## 命令行选项

- `<input_file>`：输入的 .asm 汇编文件路径
- `<output_file>`：输出的 .hack 机器码文件路径
- `--help`：显示详细的帮助信息

## 目录结构

```
Command/         # 汇编命令解析与翻译
Helper/          # 命令行参数与工具类
IO_Helper/       # 文件读写相关
Logger/          # 错误与日志处理
Memory/          # 符号表与内存分配
test/            # 测试用例
main.cpp         # 程序入口
CMakeLists.txt   # CMake 构建脚本
```

## 构建

本项目使用 CMake 构建：

```sh
mkdir build
cd build
cmake ..
make
```

生成的可执行文件位于 `build/` 目录下。

## 依赖

- C++23 或更高版本
- CMake 3.10+

## 参考

- [Nand2Tetris 官方网站](https://www.nand2tetris.org/)
- [Hack 汇编语言规范](https://www.nand2tetris.org/project06)

---

如需进一步完善或定制内容，请告知！

