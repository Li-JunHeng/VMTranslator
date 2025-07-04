# VMTranslator

VMTranslator 是一个将 VM 虚拟机语言翻译为 Hack 汇编语言的命令行工具，适用于 Nand2Tetris 课程相关项目。

## 功能简介
- 支持将单个 `.vm` 文件或包含多个 `.vm` 文件的目录翻译为 `.asm` 汇编文件。
- 提供命令行参数帮助和版本信息。
- 错误处理友好，支持输入路径校验。

## 编译方法

本项目使用 CMake 构建。

```bash
mkdir build && cd build
cmake ..
make
```
编译完成后会生成 `VMTranslator` 可执行文件。

## 使用方法

```bash
VMTranslator [选项] <输入路径> [输出路径]
```

### 选项
- `-h`, `--help`      显示帮助信息并退出
- `-v`, `--version`   显示版本信息并退出

### 输入路径
- 可以是单个 `.vm` 文件或包含 `.vm` 文件的目录

### 输出路径
- 指定输出 `.asm` 文件的路径，默认为：
  - 输入为文件：与输入文件同目录，文件名相同但扩展名为 `.asm`
  - 输入为目录：在该目录下创建与目录同名的 `.asm` 文件

### 示例
```bash
# 翻译单个文件
target/VMTranslator test.vm

# 翻译目录下所有 .vm 文件
target/VMTranslator ./project_dir

# 指定输出文件
target/VMTranslator input.vm output.asm
```

## 许可证

本项目基于 MIT License。

## 致谢

本工具参考自 Nand2Tetris 课程项目。

