# STL概述



# string库

```cpp
#include <iostream>
#include <string> // 必须显式包含
using namespace std;
int main() {
    // 1. 空字符串（最基础）
    string s1; // s1 = ""，size=0，capacity由编译器默认分配（通常15/31）
    // 2. 拷贝初始化（=）
    string s2 = "hello"; // s2 = "hello"
    string s3 = s2;      // 拷贝s2，s3独立于s2（深拷贝）
    // 3. 直接初始化（括号）
    string s4("world");  // s4 = "world"
    string s5(5, 'a');   // 重复5个'a'，s5 = "aaaaa"（填充初始化）
    // 4. 子串初始化（迭代器/下标范围）
    string s6(s2, 1, 3); // 从s2的下标1开始，取3个字符 → "ell"
    string s7(s4.begin()+1, s4.end()-1); // 迭代器范围 → "orl"
    // 打印验证
    cout << "s1: " << s1 << "(size=" << s1.size() << ")\n"; // 空，size=0
    cout << "s2: " << s2 << "\n"; // hello
    cout << "s5: " << s5 << "\n"; // aaaaa
    cout << "s6: " << s6 << "\n"; // ell
    return 0;
}
```

## 核心成员函数（按功能分类，深入讲解）

### 元素访问

```cpp
#include <iostream>
#include <string>
using namespace std;
int main() {
    string s = "hello";
    // 高效访问（无检查）
    cout << s[0] << endl; // h
    // 安全访问（有检查）
    try {
        s.at(10); // 下标越界，抛出异常
    } catch (out_of_range& e) {
        cout << "异常：" << e.what() << endl; // 捕获并打印异常信息
    }
    // 快捷访问首尾
    s.front() = 'H'; // 首字符改为H → "Hello"
    s.back() = '!';  // 尾字符改为! → "Hell!"
    cout << s << endl; // Hell!
    return 0;
}
```



### 容量管理

| 函数                | 功能                                                         |
| :------------------ | :----------------------------------------------------------- |
| `size()`/`length()` | 返回字符数（两者完全等价，`size()`是 STL 统一命名）          |
| `empty()`           | 判断是否为空（等价于`size()==0`，效率更高）                  |
| `capacity()`        | 返回当前分配的内存能容纳的字符数（不含末尾的 '\0'）          |
| `reserve(n)`        | 预分配至少能存`n`个字符的内存（仅扩容，不初始化，优化性能）  |
| `resize(n)`         | 调整字符串长度为`n`：n<size 则截断，n>size 则补 '\0'（或指定字符） |
| `clear()`           | 清空字符串（size=0，capacity 不变）                          |

```cpp
#include <iostream>
#include <string>
using namespace std;
int main() {
    string s;
    cout << "初始：size=" << s.size() << ", capacity=" << s.capacity() << endl;
    // 初始：size=0, capacity=15（GCC默认，不同编译器可能不同）
    // 频繁拼接会触发扩容（每次扩容通常翻倍）
    for (int i = 0; i < 20; i++) {
        s += 'a';
        cout << "添加第" << i+1 << "个字符：capacity=" << s.capacity() << endl;
        // 加到16个字符时，capacity从15→31；加到32个时→63，以此类推
    }
    // 预分配内存，避免频繁扩容（优化关键）
    string s_opt;
    s_opt.reserve(20); // 预分配20个字符的内存
    cout << "预分配后：size=" << s_opt.size() << ", capacity=" << s_opt.capacity() << endl;
    // size=0, capacity=20（无扩容开销）
    return 0;
}
```

### 修改操作

#### （1）拼接（+= /append ()）

```cpp
string s = "hello";
s += " world"; // 简化拼接 → "hello world"
s.append(3, '!'); // 拼接3个! → "hello world!!!"
s.append("12345", 1, 3); // 拼接"12345"的下标1开始3个字符 → "234"，最终s="hello world!!!234"
```

#### （2）插入（insert ()）

```cpp
string s = "hello";
s.insert(2, "123"); // 在下标2插入"123" → "he123llo"
s.insert(0, 2, 'x'); // 在下标0插入2个x → "xxhe123llo"
```

#### （3）删除（erase ()）

```cpp
string s = "hello world";
s.erase(5); // 从下标5开始删除所有字符 → "hello"
s.erase(1, 2); // 从下标1开始删除2个字符 → "hlo"
```

#### （4）替换（replace ()）

```cpp
string s = "hello world";
s.replace(6, 5, "cpp"); // 从下标6开始，替换5个字符为"cpp" → "hello cpp"
```

### 查找与搜索

| 函数                 | 功能                               |
| :------------------- | :--------------------------------- |
| `find(str)`          | 从左到右找 str，返回首次出现的下标 |
| `rfind(str)`         | 从右到左找 str，返回首次出现的下标 |
| `find_first_of(str)` | 找 str 中任意字符首次出现的下标    |
| `find_last_of(str)`  | 找 str 中任意字符最后出现的下标    |

```cpp
#include <iostream>
#include <string>
using namespace std;
int main() {
    string s = "hello world, hello cpp";
    // 查找"hello"首次出现的位置
    size_t pos = s.find("hello");
    if (pos != string::npos) {
        cout << "hello首次出现：" << pos << endl; // 0
    }
    // 从下标5开始查找"hello"
    pos = s.find("hello", 5);
    if (pos != string::npos) {
        cout << "hello第二次出现：" << pos << endl; // 13
    }
    // 查找任意数字（无则返回npos）
    pos = s.find_first_of("0123456789");
    if (pos == string::npos) {
        cout << "无数字" << endl; // 输出此结果
    }
    return 0;
}
```

### 子串截取

```cpp
string s = "hello world";
string sub1 = s.substr(6); // 从下标6开始截取所有字符 → "world"
string sub2 = s.substr(0, 5); // 从下标0开始截取5个字符 → "hello"
```

## 字符串比较

### 运算符重载

按**ASCII 码逐字符比较**：

```cpp
string s1 = "apple";
string s2 = "banana";
cout << (s1 < s2) << endl; // 1（true，'a' < 'b'）
cout << (s1 == "apple") << endl; // 1（true）
```

### compare()成员函数（精细控制）

```cpp
string s = "hello";
int res = s.compare("hell"); // 正数（s更长）
res = s.compare(0, 3, "hel"); // 0（前3个字符相等）
```

## 迭代器与 STL 算法结合

```cpp
#include <iostream>
#include <string>
#include <algorithm> // 算法头文件
using namespace std;
int main() {
    string s = "hello";
    // 反转字符串
    reverse(s.begin(), s.end());
    cout << s << endl; // olleh
    // 排序字符（按ASCII码）
    sort(s.begin(), s.end());
    cout << s << endl; // ehllo
    // 遍历：反向迭代器
    for (string::reverse_iterator it = s.rbegin(); it != s.rend(); it++) {
        cout << *it << " "; // o l l h e
    }
    return 0;
}
```



# 迭代器

## `string` 迭代器的核心类型（深入版）

`string` 提供 4 种迭代器类型，适配 “读写 / 只读”“正向 / 反向” 的不同场景，深入学习需明确区分：

| 迭代器类型     | 语法                             | 核心特性                       | 适用场景                               |
| :------------- | :------------------------------- | :----------------------------- | :------------------------------------- |
| 普通迭代器     | `string::iterator`               | 可读可写，正向遍历             | 修改字符、插入 / 删除操作              |
| 常量迭代器     | `string::const_iterator`         | 只读不可写，正向遍历           | 仅读取字符（如函数参数、const string） |
| 反向迭代器     | `string::reverse_iterator`       | 可读可写，反向遍历（从尾到首） | 反向访问 / 修改字符                    |
| 常量反向迭代器 | `string::const_reverse_iterator` | 只读不可写，反向遍历           | 仅反向读取字符                         |

```cpp
#include <iostream>
#include <string>
using namespace std;
int main() {
    string s = "hello world";
    // 1. 普通迭代器（可读可写）
    string::iterator it;
    it = s.begin(); // 指向第一个字符（'h'）
    *it = 'H';      // 修改字符 → s变为"Hello world"
    cout << s << endl;
    // 2. 常量迭代器（只读，不可修改）
    string::const_iterator cit = s.cbegin(); // cbegin()返回const_iterator
    cout << *cit << endl; // 读取'H'（合法）
    // *cit = 'x'; // 错误：常量迭代器不能修改元素
    // 3. 反向迭代器（反向遍历，rbegin()指向最后一个字符）
    string::reverse_iterator rit = s.rbegin();
    *rit = '!'; // 修改最后一个字符 → s变为"Hello worl!"
    cout << s << endl;
    // 4. 常量反向迭代器（只读反向遍历）
    string::const_reverse_iterator crit = s.crbegin();
    cout << *crit << endl; // 读取'!'（合法）
    return 0;
}
```

## 迭代器的核心操作（随机访问能力）

| 操作       | 语法示例                 | 功能说明                                      |
| :--------- | :----------------------- | :-------------------------------------------- |
| 解引用     | `*it`                    | 访问迭代器指向的字符（普通迭代器可修改）      |
| 成员访问   | `it->`（极少用）         | 等价于`(*it).`，string 无成员，仅适配通用语法 |
| 前置 ++/-- | `++it`/`--it`            | 移动到下一个 / 上一个字符（效率高于后置）     |
| 后置 ++/-- | `it++`/`it--`            | 移动并返回原迭代器（需临时拷贝，效率低）      |
| 算术运算   | `it += n`/`it -= n`      | 向前 / 向后移动 n 个字符（随机访问核心）      |
| 随机访问   | `it[n]`                  | 访问迭代器偏移 n 位的字符（等价`*(it + n)`）  |
| 比较运算   | `it1 == it2`/`it1 < it2` | 判断迭代器位置关系（指向同一字符 / 前后位置） |
| 差值运算   | `it1 - it2`              | 返回两个迭代器的距离（size_t 类型）           |

```cpp
#include <iostream>
#include <string>
using namespace std;
int main() {
    string s = "hello world";
    string::iterator it = s.begin();
    // 1. 移动迭代器（++/+=）
    ++it; // 指向'e'
    it += 3; // 再向后移动3位 → 指向'o'
    cout << *it << endl; // 输出'o'
    // 2. 随机访问（it[n]）
    cout << it[2] << endl; // 等价*(it+2) → 指向' '（空格）
    // 3. 差值运算（距离）
    string::iterator it_end = s.end();
    cout << "迭代器距离：" << it_end - it << endl; // 输出6（o到末尾的字符数）
    // 4. 比较运算
    if (it < s.end()) {
        cout << "it在字符串范围内" << endl;
    }
    // 5. 反向移动（--/-=）
    it -= 2; // 向后移动2位 → 指向'l'
    --it;    // 再向前移动1位 → 指向'l'
    cout << *it << endl; // 输出'l'
    return 0;
}
```

## 基础遍历（正向 / 反向）

### 正向遍历（从首到尾）

`begin()`：指向第一个字符；`end()`：指向 “尾后位置”（不是最后一个字符，是最后一个字符的下一位，作为遍历终止条件）。

```cpp
int main() {
    string s = "hello";
    // 方式1：普通迭代器（可修改）
    for (string::iterator it = s.begin(); it != s.end(); ++it) {
        *it = toupper(*it); // 转为大写 → "HELLO"
        cout << *it << " "; // 输出：H E L L O
    }
    cout << endl;
    // 方式2：常量迭代器（仅读取，const string必须用）
    const string s_const = "world";
    for (string::const_iterator it = s_const.cbegin(); it != s_const.cend(); ++it) {
        cout << *it << " "; // 输出：w o r l d
    }
    return 0;
}
```

### 反向遍历（从尾到首）

`rbegin()`：指向最后一个字符；`rend()`：指向 “首前位置”（第一个字符的前一位）；反向迭代器的`++`等价于普通迭代器的`--`。

```cpp
int main() {
    string s = "hello";
    // 方式1：普通反向迭代器（可修改）
    for (string::reverse_iterator it = s.rbegin(); it != s.rend(); ++it) {
        *it = toupper(*it); // 转为大写 → "HELLO"
        cout << *it << " "; // 输出：O L L E H（反向遍历）
    }
    cout << endl;
    // 方式2：常量反向迭代器（仅读取）
    const string s_const = "world";
    for (string::const_reverse_iterator it = s_const.crbegin(); it != s_const.crend(); ++it) {
        cout << *it << " "; // 输出：d l r o w
    }
    return 0;
}
```

## 迭代器的进阶场景：与 STL 算法结合

```cpp
#include <iostream>
#include <string>
#include <algorithm> // sort/reverse/find
using namespace std;
int main() {
    string s = "hello world";
    // 1. 反转字符串（reverse）
    reverse(s.begin(), s.end());
    cout << "反转后：" << s << endl; // dlrow olleh
    // 2. 排序字符（sort，按ASCII码）
    sort(s.begin(), s.end());
    cout << "排序后：" << s << endl; //  空格 d e h l l l o o r w
    // 3. 查找字符（find）
    auto it = find(s.begin(), s.end(), 'l');
    if (it != s.end()) {
        cout << "找到'l'，位置：" << it - s.begin() << endl; // 输出5
    }
    // 4. 替换指定范围字符（fill）
    fill(s.begin() + 2, s.begin() + 5, 'x');
    cout << "填充后：" << s << endl; //  空格 d x x x l l o o r w
    return 0;
}
```

