# 排序



## 冒泡排序

```cpp
#include <iostream>
#include <vector>
using namespace std;
void bubbleSort(vector<int>& arr) {
    int n = arr.size();

    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}
int main() {
    vector<int> arr = {5, 3, 8, 4, 2};
    bubbleSort(arr);
    cout << "排序结果: ";
    for (auto num : arr) {
        cout << num << " ";
    }
    return 0;
}

```



## 选择排序

```cpp
#include <iostream>
#include <vector>
using namespace std;
void doubleSelectionSort(vector<int>& arr) {
    int left = 0;
    int right = arr.size() - 1;
    while (left < right) {
        int minIndex = left;
        int maxIndex = right;
        // 遍历当前区间
        for (int i = left; i <= right; i++) {
            if (arr[i] < arr[minIndex])
                minIndex = i;
            if (arr[i] > arr[maxIndex])
                maxIndex = i;
        }
        // 先交换最小值
        swap(arr[left], arr[minIndex]);
        // 如果最大值原本在 left 位置，需要更新
        if (maxIndex == left)
            maxIndex = minIndex;
        // 再交换最大值
        swap(arr[right], arr[maxIndex]);
        left++;
        right--;
    }
}
int main() {
    vector<int> arr = {5, 3, 8, 4, 2};
    doubleSelectionSort(arr);
    cout << "排序结果: ";
    for (int num : arr) {
        cout << num << " ";
    }
    return 0;
}
```



## 插入排序

```cpp
#include <iostream>
#include <vector>
using namespace std;
void insertionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];   // 当前要插入的元素
        int j = i - 1;
        // 向前查找插入位置
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];  // 元素后移
            j--;
        }
        arr[j + 1] = key;  // 插入
    }
}
int main() {
    vector<int> arr = {5, 3, 8, 4, 2};
    insertionSort(arr);
    cout << "排序结果: ";
    for (int num : arr) {
        cout << num << " ";
    }
    return 0;
}
```

### 二分插入排序

```cpp
#include <iostream>  // 1. 引入输入输出流头文件
                     // 作用：提供cout（输出）功能，没有它无法打印数组元素
#include <vector>    // 2. 引入vector容器头文件
                     // 作用：支持使用C++的动态数组vector<int>，否则无法定义arr变量
using namespace std; // 3. 使用std命名空间
                     // 作用：后续使用cout、vector时，无需写std::前缀（比如std::cout、std::vector），简化代码
// 4. 定义二分插入排序函数
// void：无返回值（排序是原地修改原数组，无需返回）
// binaryInsertionSort：函数名（语义化，表示“二分插入排序”）
// vector<int>& arr：参数是int类型vector的引用（&表示引用传递，能修改原数组，且避免拷贝）
void binaryInsertionSort(vector<int>& arr) {
    // 5. 获取数组长度并赋值给n
    // 原因：后续循环多次用到数组长度，提前获取并保存，比每次调用arr.size()更高效
    int n = arr.size();
    // 6. 外层for循环：遍历未排序区间
    // i从1开始：默认数组第0个元素（arr[0]）是已排序区间的初始值，i=1到n-1是未排序区间
    // i < n：遍历完所有未排序元素就停止
    for (int i = 1; i < n; i++) {
        // 7. 保存当前要插入的元素到key
        // 关键原因：后续元素后移时会覆盖arr[i]，必须先把这个值存起来
        int key = arr[i];
        // 8. 注释：提示接下来是二分查找插入位置的逻辑（仅说明作用，不执行）
        // 二分查找插入位置
        // 9. 初始化二分查找的左边界：已排序区间的起始位置（从0开始）
        int left = 0;
        // 10. 初始化二分查找的右边界：已排序区间的最后一个元素位置（i-1，因为i是未排序区间第一个）
        int right = i - 1;
        // 11. 二分查找循环：在[left, right]范围内找key的插入位置
        // 循环条件left <= right：只要左边界不超过右边界，就继续查找
        while (left <= right) {
            // 12. 计算二分查找的中间位置mid
            // 写法：left + (right - left)/2 而非 (left+right)/2
            // 原因：避免left和right都很大时，相加溢出int的取值范围（比如left=2e9，right=2e9，相加会超出int上限）
            int mid = left + (right - left) / 2;
            // 13. 判断中间元素和key的大小关系
            if (arr[mid] > key)
                // 14. 若中间元素>key：说明key应该插入到左半区间，调整右边界
                right = mid - 1;
            else
                // 15-16. 若中间元素<=key：说明key应该插入到右半区间，调整左边界
                // 注意：这里保留<=，是为了保证排序的“稳定性”（相同值元素相对位置不变）
                left = mid + 1;
        } // 17. 二分查找循环结束
          // 此时必然满足left > right，且left就是key的最终插入位置（核心结论）
        // 18. 注释：提示接下来是移动元素的逻辑
        // 移动元素
        // 19. 元素后移循环：从已排序区间末尾（i-1）倒序遍历到插入位置left
        // 倒序原因：如果正序遍历（从left到i-1），会先覆盖arr[left+1]，导致后续元素丢失
        for (int j = i - 1; j >= left; j--) {
            // 20. 将当前元素arr[j]后移一位到arr[j+1]，为key腾出位置
            arr[j + 1] = arr[j];
        } // 21. 元素后移循环结束
        // 22. 将key插入到找到的正确位置left，完成本轮插入
        arr[left] = key;
    } // 23. 外层for循环结束：所有未排序元素都完成插入，数组有序
} // 24. 二分插入排序函数体结束
// 25. 主函数：程序的入口（C++程序从main函数开始执行）
int main() {
    // 26. 定义并初始化一个int类型的vector数组
    // 初始值：{5,3,8,4,2}，这是待排序的原始数组
    vector<int> arr = {5, 3, 8, 4, 2};
    // 27. 调用二分插入排序函数
    // 因为arr是引用传递，函数内部的修改会直接作用于这个原数组
    binaryInsertionSort(arr);
    // 28. 范围for循环（C++11+）：遍历排序后的arr数组
    // 每次循环将arr中的一个元素赋值给临时变量num
    for (int num : arr)
        // 29. 输出当前元素num，并加一个空格（避免元素连在一起）
        cout << num << " ";
    // 30. 主函数返回0：表示程序正常执行结束（约定俗成的规范）
    return 0;
} // 31. 主函数体结束
```

### 希尔排序

```cpp
#include <iostream>
#include <vector>
using namespace std;
// 4. 定义希尔排序函数
// void：无返回值（原地排序，直接修改传入的原数组）
// shellSort：函数名（语义化，对应“希尔排序”）
// vector<int>& arr：int类型vector的引用参数（&保证修改原数组，且避免拷贝开销）
void shellSort(vector<int>& arr) {
    // 5. 获取数组长度并赋值给n
    // 原因：后续多次用到数组长度，提前保存提升效率，避免重复调用arr.size()
    int n = arr.size();
    // 6. 外层循环：控制分组间隔（gap）的递减
    // 初始gap = n/2（将数组分为n/2组），每次gap减半，直到gap>0
    // 核心：gap是希尔排序的“增量”，逐步缩小增量让数组接近有序
    for (int gap = n / 2; gap > 0; gap /= 2) {
        // 7. 中层循环：遍历每个分组的“待插入元素”
        // i从gap开始：前gap个元素是每个分组的第一个元素（默认已排序），i从gap开始处理后续元素
        for (int i = gap; i < n; i++) {
            // 8. 保存当前待插入的元素到temp
            // 关键原因：后续分组内元素后移时会覆盖arr[i]，必须先缓存值
            int temp = arr[i];
            // 9. 初始化j为i，用于向前遍历当前分组的已排序部分
            int j = i;
            // 10. 注释：提示接下来是“分组内的插入排序”（仅说明作用，不执行）
            // 分组插入排序
            // 11. 内层while循环：在当前分组内向前查找插入位置
            // 条件1：j >= gap → 保证j-gap不越界（避免访问arr[-gap]等非法位置）
            // 条件2：arr[j - gap] > temp → 前一个分组元素比当前元素大，需要后移
            while (j >= gap && arr[j - gap] > temp) {
                // 12. 分组内元素后移：将前一个分组元素挪到当前位置
                arr[j] = arr[j - gap];
                // 13. j向前移动一个gap，继续比较前一个分组元素
                j -= gap;
            }
            // 14. 将temp插入到分组内的正确位置（j是最终插入位置）
            arr[j] = temp;
        }
    }
} // 15. 希尔排序函数体结束
int main() {
    vector<int> arr = {8, 5, 3, 7, 6, 2, 4, 1};
    shellSort(arr);
    for (int num : arr)
        cout << num << " ";
    return 0;
}
```

## 堆排序

```cpp
#include <iostream>
#include <vector>
using namespace std;
// 4. 注释：说明heapify函数的功能——维护最大堆的性质
// 堆化函数（维护最大堆）
// void：无返回值（原地修改数组）
// vector<int>& arr：待堆化的数组（引用传递，修改原数组）
// int n：当前堆的有效大小（不是整个数组长度，排序阶段会逐步缩小）
// int i：当前要进行堆化的节点索引（从该节点开始向下调整堆）
void heapify(vector<int>& arr, int n, int i) {
    // 5. 初始化largest为i：假设当前节点i是当前子树的最大值节点
    int largest = i;        // 当前最大节点的索引
    // 6. 计算当前节点i的左子节点索引：二叉堆的存储规则（左孩子=2*i+1）
    int left = 2 * i + 1;   // 左子节点索引
    // 7. 计算当前节点i的右子节点索引：二叉堆的存储规则（右孩子=2*i+2）
    int right = 2 * i + 2;  // 右子节点索引
    // 8. 注释：提示接下来是“找当前子树的最大值节点”
    // 找最大值
    // 9. 比较左子节点和当前最大值：若左子节点更大，更新largest
    // 条件1：left < n → 左子节点在当前堆的有效范围内（避免越界）
    // 条件2：arr[left] > arr[largest] → 左子节点值大于当前最大值
    if (left < n && arr[left] > arr[largest])
        largest = left;     // 更新最大值节点为左子节点
    // 10. 比较右子节点和当前最大值：若右子节点更大，更新largest
    // 条件1：right < n → 右子节点在当前堆的有效范围内
    // 条件2：arr[right] > arr[largest] → 右子节点值大于当前最大值
    if (right < n && arr[right] > arr[largest])
        largest = right;    // 更新最大值节点为右子节点
    // 11. 注释：提示接下来是“若最大值不是当前节点，交换并递归调整”
    // 如果最大值不是当前节点（说明堆结构被破坏）
    if (largest != i) {
        // 12. 交换当前节点i和最大值节点largest的值，恢复当前子树的最大堆性质
        swap(arr[i], arr[largest]);
        // 13. 递归堆化被交换的子节点：因为交换后，largest位置的子树可能不再满足最大堆性质，需要继续向下调整
        heapify(arr, n, largest);  // 递归调整受影响的子树
    }
}
// 14. 定义堆排序函数
// void：无返回值（原地排序）
// vector<int>& arr：待排序的数组（引用传递）
void heapSort(vector<int>& arr) {
    // 15. 获取数组总长度n
    int n = arr.size();
    // 16. 注释：标记第一步——构建最大堆
    // 1?? 建最大堆
    // 17. 循环：从最后一个非叶子节点向前遍历，逐个堆化
    // 起始点：n/2 - 1 → 二叉堆中最后一个非叶子节点的索引（叶子节点无需堆化）
    // 终止点：i >= 0 → 遍历到根节点（索引0）
    // 递减：i-- → 从下往上堆化，保证每个父节点都满足最大堆性质
    for (int i = n / 2 - 1; i >= 0; i--) {
        // 18. 对节点i进行堆化，构建最大堆
        heapify(arr, n, i);
    }
    // 19. 注释：标记第二步——依次取出最大元素（堆顶）
    // 2?? 依次取出最大元素
    // 20. 循环：从数组末尾向前遍历，逐个处理堆顶元素
    // 起始点：i = n - 1 → 数组最后一个元素的索引
    // 终止点：i > 0 → 只剩根节点时停止（此时数组已完全有序）
    // 递减：i-- → 每次处理后，堆的有效大小减1
    for (int i = n - 1; i > 0; i--) {
        // 21. 交换堆顶（arr[0]，当前最大值）和当前堆的最后一个元素（arr[i]）
        // 作用：将最大值“移出堆”，放到数组的最终有序位置
        swap(arr[0], arr[i]);      // 把最大值放到数组末尾
        // 22. 对剩余的i个元素重新堆化（堆的有效大小变为i），根节点为0
        // 作用：恢复剩余元素的最大堆性质，以便取下一个最大值
        heapify(arr, i, 0);        // 重新调整堆（仅处理前i个元素）
    }
}
int main() {
    vector<int> arr = {4, 10, 3, 5, 1};
    heapSort(arr);
    for (int num : arr)
        cout << num << " ";
    return 0;
}
```



## 归并排序

```cpp
#include <iostream>  // 1. 引入输入输出流头文件
                     // 作用：提供cout输出功能，用于打印排序后的数组
#include <vector>    // 2. 引入vector容器头文件
                     // 作用：支持动态数组vector（包括待排序的arr、合并用的temp）
using namespace std; // 3. 使用std命名空间
                     // 作用：简化代码，无需写std::cout、std::vector、std::push_back等前缀
// 4. 定义合并函数：将两个有序子数组合并为一个有序数组
// void：无返回值（原地修改原数组）
// vector<int>& arr：待合并的原数组（引用传递，修改原数组）
// left：左子数组的起始索引（[left, mid]是第一个有序子数组）
// mid：两个子数组的分界点
// right：右子数组的结束索引（[mid+1, right]是第二个有序子数组）
void merge(vector<int>& arr, int left, int mid, int right) {
    // 5. 创建临时数组temp：存储合并后的有序元素
    // 关键原因：直接修改原数组会覆盖未合并的元素，temp作为“中转站”保证合并正确
    vector<int> temp;
    // 6. i：指向左子数组的起始位置（[left, mid]）
    int i = left;
    // 7. j：指向右子数组的起始位置（[mid+1, right]）
    int j = mid + 1;
    // 8. 核心循环：两两比较两个有序子数组的元素，按升序加入temp
    // 循环条件：i没遍历完左子数组 且 j没遍历完右子数组
    while (i <= mid && j <= right) {
        // 9. 左子数组当前元素 ≤ 右子数组当前元素：取左元素
        // 注意：用<=保证排序的“稳定性”（相同值的元素相对位置不变）
        if (arr[i] <= arr[j])
            temp.push_back(arr[i++]);  // 左元素加入temp，i自增（指向下一个左元素）
        else
            // 10. 右子数组当前元素更小：取右元素
            temp.push_back(arr[j++]);  // 右元素加入temp，j自增（指向下一个右元素）
    }
    // 11. 处理左子数组的剩余元素（如果有）
    // 说明：左子数组还没遍历完，直接全部加入temp（本身有序）
    while (i <= mid)
        temp.push_back(arr[i++]);
    // 12. 处理右子数组的剩余元素（如果有）
    // 说明：右子数组还没遍历完，直接全部加入temp（本身有序）
    while (j <= right)
        temp.push_back(arr[j++]);
    // 13. 注释：提示接下来将temp的有序元素拷贝回原数组
    // 拷贝回原数组
    // 14. 遍历temp，将有序元素覆盖原数组的[left, right]区间
    for (int k = 0; k < temp.size(); k++) {
        // left + k：对应原数组的位置（从left开始，逐个填充）
        arr[left + k] = temp[k];
    }
}
// 15. 定义归并排序递归函数：拆分数组并调用合并函数
// void：无返回值（原地排序）
// vector<int>& arr：待排序的原数组（引用传递）
// left：当前处理区间的起始索引
// right：当前处理区间的结束索引
void mergeSort(vector<int>& arr, int left, int right) {
    // 16. 递归终止条件：当前区间只有1个元素（left=right）或无元素（left>right）
    // 说明：长度≤1的数组天然有序，无需排序，直接返回
    if (left >= right)
        return;
    // 17. 计算当前区间的中点mid：拆分左右子数组
    // 写法：left + (right - left)/2 而非 (left+right)/2
    // 原因：避免left和right都很大时，相加溢出int的取值范围
    int mid = left + (right - left) / 2;
    // 18. 递归拆分：处理左子区间[left, mid]
    mergeSort(arr, left, mid);
    // 19. 递归拆分：处理右子区间[mid+1, right]
    mergeSort(arr, mid + 1, right);
    // 20. 合并：将两个已排序的子区间[left, mid]和[mid+1, right]合并为有序区间
    merge(arr, left, mid, right);
}
// 21. 主函数：程序入口
int main() {
    // 22. 定义并初始化待排序的vector数组
    vector<int> arr = {8, 4, 5, 7, 1, 3, 6, 2};
    // 23. 调用归并排序函数：初始处理整个数组（左边界0，右边界arr.size()-1=7）
    mergeSort(arr, 0, arr.size() - 1);
    // 24. 范围for循环：遍历排序后的数组
    for (int num : arr)
        // 25. 输出当前元素，加空格分隔
        cout << num << " ";
    // 26. 返回0：表示程序正常执行结束（C++主函数规范）
    return 0;
} 	
```

## 快速排序

```cpp
#include <iostream>  // 1. 引入输入输出流头文件
                     // 作用：提供cout输出功能，用于打印排序后的数组
#include <vector>    // 2. 引入vector容器头文件
                     // 作用：支持动态数组vector<int>存储待排序数据
using namespace std; // 3. 使用std命名空间
                     // 作用：简化代码，无需写std::cout、std::vector、std::swap等前缀
// 4. 注释：说明partition函数的功能——划分数组并返回基准的正确索引
// 划分函数
// int：返回值是基准元素的最终索引（排序后的正确位置）
// vector<int>& arr：待划分的数组（引用传递，直接修改原数组）
// left：当前划分区间的起始索引
// right：当前划分区间的结束索引（基准元素的初始位置）
int partition(vector<int>& arr, int left, int right) {
    // 5. 选择基准值：选区间最后一个元素作为pivot（Lomuto划分方案）
    int pivot = arr[right];  
    // 6. 初始化i：表示“小于pivot的区域”的右边界（初始在区间外，left-1）
    // 核心作用：i左边的所有元素都 < pivot，i右边（到j-1）的元素都 ≥ pivot
    int i = left - 1;       
    // 7. 遍历区间[left, right-1]（除基准外的所有元素）
    for (int j = left; j < right; j++) {
        // 8. 如果当前元素 < pivot：归入“小于pivot的区域”
        if (arr[j] < pivot) {
            i++;  // 9. 扩大“小于pivot的区域”（右边界右移）
            // 10. 交换arr[i]和arr[j]：把当前元素放到“小于区域”的末尾
            swap(arr[i], arr[j]);
        }
        // 注意：如果arr[j] ≥ pivot，不做任何操作，j直接右移（归入“大于等于区域”）
    }
    // 11. 把基准元素放到“正确位置”：i+1是基准的最终索引（小于区域的下一个位置）
    swap(arr[i + 1], arr[right]);  
    // 12. 返回基准的索引，供递归排序左右子数组
    return i + 1;
}
// 13. 定义快速排序递归函数
// void：无返回值（原地排序，直接修改原数组）
// vector<int>& arr：待排序的数组（引用传递）
// left：当前排序区间的起始索引
// right：当前排序区间的结束索引
void quickSort(vector<int>& arr, int left, int right) {
    // 14. 递归终止条件：当前区间长度 ≤ 1（left >= right）
    // 说明：长度≤1的数组天然有序，无需排序，直接返回
    if (left < right) {
        // 15. 调用划分函数，得到基准的正确索引pivotIndex
        int pivotIndex = partition(arr, left, right);
        // 16. 递归排序：基准左边的子区间[left, pivotIndex-1]（都 < 基准）
        quickSort(arr, left, pivotIndex - 1);
        // 17. 递归排序：基准右边的子区间[pivotIndex+1, right]（都 > 基准）
        quickSort(arr, pivotIndex + 1, right);
    }
}
// 18. 主函数：程序入口
int main() {
    // 19. 定义并初始化待排序的vector数组
    vector<int> arr = {6, 3, 8, 5, 2, 7};
    // 20. 调用快速排序函数：初始处理整个数组（左边界0，右边界arr.size()-1=5）
    quickSort(arr, 0, arr.size() - 1);
    // 21. 范围for循环：遍历排序后的数组
    for (int num : arr)
        // 22. 输出当前元素，加空格分隔
        cout << num << " ";
    // 23. 返回0：表示程序正常执行结束（C++主函数规范）
    return 0;
}
```

### 三路快排

针对**大量重复元素优化的三路快速排序**

```cpp
#include <iostream>  // 1. 引入输入输出流头文件，用于cout打印结果
#include <vector>    // 2. 引入vector容器头文件，支持动态数组存储数据
#include <cstdlib>   // 3. 引入cstdlib头文件，用于rand()函数（生成随机数选pivot）
using namespace std; // 4. 使用std命名空间，简化cout/vector/rand/swap等调用
// 5. 定义三路快速排序递归函数
// void：无返回值（原地排序，直接修改原数组）
// vector<int>& arr：待排序的数组（引用传递，避免拷贝且修改原数组）
// left：当前排序区间的起始索引
// right：当前排序区间的结束索引
void quickSort3Way(vector<int>& arr, int left, int right) {
    // 6. 递归终止条件：当前区间长度≤1（left>=right），天然有序无需排序
    if (left >= right)
        return;
    // 7. 注释：随机选择pivot索引（防止有序/重复数组导致算法退化到O(n?)）
    // 随机选 pivot（防退化）
    // 8. 生成[left, right]范围内的随机索引：left + 0~(right-left)的随机数
    // rand()%(right-left+1)：生成0到(right-left)的整数，保证随机索引在区间内
    int randomIndex = left + rand() % (right - left + 1);
    // 9. 交换随机索引的元素和区间第一个元素：将pivot放到left位置，方便后续处理
    swap(arr[left], arr[randomIndex]);
    // 10. 确定基准值pivot：取交换后的left位置元素（随机选的pivot）
    int pivot = arr[left];
    // 11. 定义lt（less than）：「小于pivot区域」的右边界，初始为left（pivot位置）
    // 核心：[left, lt-1] 都是 < pivot 的元素，初始lt=left时该区域为空
    int lt = left;        
    // 12. 定义gt（greater than）：「大于pivot区域」的左边界，初始为right
    // 核心：[gt+1, right] 都是 > pivot 的元素，初始gt=right时该区域为空
    int gt = right;       
    // 13. 定义i：当前扫描指针，初始为left+1（跳过pivot，从下一个元素开始扫描）
    // 核心：[lt, i-1] 都是 = pivot 的元素，初始i=left+1时该区域为空
    int i = left + 1;     
    // 14. 核心循环：扫描[left+1, gt]区间，划分三个区域，直到扫描指针i超过gt
    // 循环条件i<=gt：gt是「大于区域」的左边界，i超过gt说明所有元素已划分完成
    while (i <= gt) {
        // 15. 情况1：当前元素 < pivot → 归入「小于区域」
        if (arr[i] < pivot) {
            // 16. 交换当前元素arr[i]和「小于区域」右边界的元素arr[lt]
            swap(arr[i], arr[lt]);
            // 17. 「小于区域」右边界右移（扩大小于区域）
            lt++;
            // 18. 扫描指针i右移（处理下一个元素）
            i++;
        }
        // 19. 情况2：当前元素 > pivot → 归入「大于区域」
        else if (arr[i] > pivot) {
            // 20. 交换当前元素arr[i]和「大于区域」左边界的元素arr[gt]
            swap(arr[i], arr[gt]);
            // 21. 「大于区域」左边界左移（扩大大于区域）
            // 注意：这里i不右移！因为交换过来的arr[gt]还未扫描，需要重新检查
            gt--;
        }
        // 22. 情况3：当前元素 = pivot → 归入「等于区域」，仅移动扫描指针i
        else {
            i++;
        }
    }
    // 23. 递归排序「小于pivot」的区间：[left, lt-1]
    quickSort3Way(arr, left, lt - 1);
    // 24. 递归排序「大于pivot」的区间：[gt+1, right]
    // 注意：「等于pivot」的区间[lt, gt]无需递归，跳过重复元素优化效率
    quickSort3Way(arr, gt + 1, right);
}
// 25. 主函数：程序入口
int main() {
    // 26. 定义并初始化含大量重复元素的待排序数组（多个5）
    vector<int> arr = {5, 3, 5, 5, 2, 5, 8, 5};
    // 27. 调用三路快排函数：处理整个数组（left=0，right=arr.size()-1=7）
    quickSort3Way(arr, 0, arr.size() - 1);
    // 28. 范围for循环：遍历排序后的数组
    for (int num : arr)
        // 29. 输出当前元素，加空格分隔
        cout << num << " ";
    // 30. 返回0：程序正常结束
    return 0;
}
```

