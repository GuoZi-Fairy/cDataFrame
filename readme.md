# cDataFrame

简单的2维数据处理库

## 目录
- [cDataFrame](#cdataframe)
  - [目录](#%e7%9b%ae%e5%bd%95)
  - [开发情况](#%e5%bc%80%e5%8f%91%e6%83%85%e5%86%b5)
    - [功能情况](#%e5%8a%9f%e8%83%bd%e6%83%85%e5%86%b5)
    - [开发规范](#%e5%bc%80%e5%8f%91%e8%a7%84%e8%8c%83)
    - [开发预期](#%e5%bc%80%e5%8f%91%e9%a2%84%e6%9c%9f)
  - [使用方法](#%e4%bd%bf%e7%94%a8%e6%96%b9%e6%b3%95)
    - [欢迎使用cRainbowconsole](#%e6%ac%a2%e8%bf%8e%e4%bd%bf%e7%94%a8crainbowconsole)
    - [构建](#%e6%9e%84%e5%bb%ba)
      - [将源码文件添加至您的项目中](#%e5%b0%86%e6%ba%90%e7%a0%81%e6%96%87%e4%bb%b6%e6%b7%bb%e5%8a%a0%e8%87%b3%e6%82%a8%e7%9a%84%e9%a1%b9%e7%9b%ae%e4%b8%ad)
    - [数据结构](#%e6%95%b0%e6%8d%ae%e7%bb%93%e6%9e%84)
      - [cell](#cell)
      - [columns](#columns)
      - [dataframe](#dataframe)
    - [API](#api)
      - [FILE level operate](#file-level-operate)
      - [dataframe level operate](#dataframe-level-operate)
      - [开始使用cDataFrame](#%e5%bc%80%e5%a7%8b%e4%bd%bf%e7%94%a8cdataframe)
  - [开源声明](#%e5%bc%80%e6%ba%90%e5%a3%b0%e6%98%8e)
  - [帮助](#%e5%b8%ae%e5%8a%a9)
  - [](#)
## 开发情况
### 功能情况
* **csv文件io支持** [✔]
* **dataframe构造(construct)** [✔]
* **dataframe修改** [待完成]
* 1. *删除* [待完成]
* 2. *修改* [待完成]
* 3. *增加* [待完成]
* 4. *合并* [待完成]
* 5. *去重* [待完成]
* 6. *基础查找* [完成]
* **dataframe↓查找拓展(AVL_p)** [待完成]
* 1. AVL树形构造 [待完成]
* 2. AVL操作 [待完成]
* **dataframe↓匹配拓展(match_p)** [完成]
* 1. [表达式计算](/docs/match_p_api.mdmatch_p_api.md) [完成]
* 2. [match接口](/docs/match_p_api.mdmatch_p_api.md) [完成]
* 3. [eval接口](/docs/expr_parse_api.mdexpr_parse_api.md) [完成]
* **dataframe↓HASH拓展(hash_p)** [待完成]
* 1. hash构造 [未完成]
* 2. 冲突调解 [未完成]
* **dataframe统计拓展(statu_p)** [待完成]
* * **数值型**
* * 1. 求和 [完成]
* * 2. mean [完成]
* * 3. median [待完成]
* * 4. sortby [待完成]
* * 5. bp [待完成]
* * 6. ...
* * **数学型**
* * 1. 高精度运算 [待完成]
* * 2. ...
* ...
### 开发规范
* 使用c11标准
* 希望使用下划线命名法/pascal命名法
### 开发预期
该项目希望通过开发来增加对算法、数据结构、优化、架构等能力的掌握。还有开发具有实用性的工具。还有可以交友(小声bb)
## 使用方法

### 欢迎使用cRainbowconsole
### 构建
There are serval ways to incorporate cDataFrame into your projects

#### 将源码文件添加至您的项目中
整个ccrainbow工具仅含有两个文件,您可以直接将 `cDataFrame.h` 和 `cDataFrame.c` 添加到您的项目中并开始使用.
可以使用如下方式来将cDataFrame添加至您的源代码中
``` c
#include "cDataFrame.h"
```

为了尽可能多的支持各种平台和编译器，cDataFrame是使用ANSI C (C89)标准编写。

### 数据结构
`cell` 数据单元结构，可以包含[int,float,string]等结构
`columns` 列结构,由一个cell表和一个columns链表组成
`dataframe` 数据表结构,由若干个列结构组成
#### cell
```c
/* The cell structrue: */
typedef struct _cell //一个单元
{   
    element data;
    type type;
}cell;
```
The element structure
```c
/* The element structrue: */
typedef union _element //cell的数据用此类型
{
    char char_ch[200]; //不能超过199个字节
    size_t integer_num;
    double float_num;
    char nan[4];
}element;
```
#### columns
The columns structrue
```c
/* The columns structrue: */
typedef struct _columns//该结构只允许访问链表的表头
{
    size_t index;
    char name[COL_NAME_LENGTH_MAX]; //列名称
    struct _columns* next_col;
    series series;
}column;
```
The series structrue
```c
/* The series structrue: */
typedef struct _series //一个cell表
{
    cell* cell_table;
    size_t size;
    size_t top;
}series;
```
#### dataframe
The dataframe structrue
```c
/* The dataframe structrue: */
typedef struct _dataframe //数据表
{
    size_t length;
    size_t width;//size info
    column* col;//columns 链表头
}dataframe;
```
### API
#### FILE level operate
`read_csv(const char* path)` 该接口返回一个`dataframe`对象，该`dataframe`包含从csv文件中解析到的数据\
`to_csv(dataframe* df,const char* path)` 允许将`dataframe`对象存储至csv文件中
#### dataframe level operate
`dfprint(dataframe* df)` 该接口将把`df`中的数据按格式输出至console中\
`cellfind(dataframe* df,size_t col,size_t index)` 该接口将返回在`df`的`[col,index]`处的`cell*`但是不建议直接更改该`cell`
#### 开始使用cDataFrame
## 开源声明

MIT License

>  Copyright (c) 2009-2017 GuoZi
>
>  Permission is hereby granted, free of charge, to any person obtaining a copy
>  of this software and associated documentation files (the "Software"), to deal
>  in the Software without restriction, including without limitation the rights
>  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
>  copies of the Software, and to permit persons to whom the Software is
>  furnished to do so, subject to the following conditions:
>
>  The above copyright notice and this permission notice shall be included in
>  all copies or substantial portions of the Software.
>
>  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
>  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
>  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
>  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
>  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
>  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
>  THE SOFTWARE.

## 帮助
如果在使用cDataFrame的过程中遇到了问题，您可以在此处或issue中寻找解决方法
## 
- 果子 (original author)
