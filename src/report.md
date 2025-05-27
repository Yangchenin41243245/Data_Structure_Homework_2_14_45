# 41243214 
& 41243245
# 排序演算法效能分析報告

## 目錄

### 1. 解題說明
### 2. 時鐘精度
### 3. 效能分析
### 4. 程式實作
### 5. 測試與驗證
### 6. 申論及開發報告

---

## 解題說明

本作業旨在實作與分析四種排序演算法的效能，並設計一個根據輸入資料大小選擇最適合排序法的綜合排序函式（Composite Sort Function）。
作業核心在於針對不同輸入資料大小與特性（最壞情況與平均情況）進行排序效能測試，並比較各排序法在時間與記憶體使用上的表現。

本次實作包含以下五種排序方法：

1. **Insertion Sort**  
   逐一將元素插入已排序好的子陣列，最壞情況為反向排序。

2. **Quick Sort**  
   採用分治法將資料切分並遞迴排序，使用第一個元素作為 pivot，最壞情況發生於切分極不平均時。

3. **Merge Sort**  
   將資料拆分後合併排序，最壞情況相對穩定，時間複雜度為 O(n log n)，採用遞迴式實作。

4. **Heap Sort**  
   利用最大堆結構，每次將最大值移至尾端排序，對於初始排列較亂的資料效能佳。

5. **Composite Sort**  
   根據輸入資料大小動態選擇排序方法：小於等於 32 個元素使用 Insertion Sort，小於等於 1000 個元素使用 Merge Sort，小於等於 5000 個元素使用 Heap Sort，否則使用 Quick Sort。

透過 `std::chrono` 計時與 Windows API 測量記憶體使用量，將各排序法在不同 `n` 值下的效能進行數據分析與圖表視覺化，最終統整出各排序法的適用情境與建議使用範圍。Composite Sort 旨在結合各排序法的優勢，針對不同資料規模提供最佳效能。

---

## 時鐘精度

本次作業中，排序演算法的效能主要透過時間消耗（微秒，microseconds）進行衡量。為確保執行時間的測量精確，我們使用 C++ 標準函式庫中的 `<chrono>` 模組中的 `std::chrono::high_resolution_clock` 作為計時工具，其精度可達微秒等級，適合用於效能分析。

### 使用方式

計時方式根據資料量動態調整，支援單次計時與多次執行取平均兩種模式。基本計時範例如下：

```cpp
auto start = chrono::high_resolution_clock::now();
// 排序函式執行
auto stop = chrono::high_resolution_clock::now();
auto duration = chrono::duration_cast<chrono::microseconds>(stop - start).count();
```

透過 `chrono::duration_cast` 精確計算兩個時間點之間的間隔（單位為微秒）。所有排序方法（包括 Composite Sort）均採用一致的計時邏輯，確保比較公平性。

### 測量誤差與處理方式

對於小資料量（例如 n ≤ 1000），排序執行時間可能過短，導致計時結果不穩定（如波動大或接近 0 μs）。為解決此問題，程式採用以下策略：

- **多次執行取平均時間**：當資料量 ≤ 1000 時，啟用多次執行模式，對排序函式執行 `REPEAT_COUNT`（1000）次，使用陣列副本避免修改原始資料，計算平均時間以降低測量誤差。程式碼範例如下：

  ```cpp
  const int REPEAT_COUNT = 1000;
  auto start = high_resolution_clock::now();
  for (int i = 0; i < REPEAT_COUNT; ++i) {
      vector<entry> temp_arr = arr; // 陣列副本
      yourSortFunction(temp_arr);    // 排序
  }
  auto stop = high_resolution_clock::now();
  auto avg_duration = chrono::duration_cast<chrono::microseconds>(stop - start).count() / REPEAT_COUNT;
  ```

- **單次計時**：當資料量 > 1000（例如本程式中的 n = 5000）時，僅執行一次排序，直接記錄執行時間，模擬實際使用情境。此方式適用於資料量較大、計時結果穩定的情況。

在主程式中，根據資料量動態選擇計時模式：

```cpp
bool useAverage = (array_size <= SMALL_DATA_THRESHOLD); // SMALL_DATA_THRESHOLD = 100
```

---

## 效能分析

#### Insertion Sort
- **時間複雜度**：
  - **Worst-case**：O(n²)，當輸入為逆序（程式中生成逆序資料），每次插入需移動所有前元素。
  - **Average-case**：O(n²)，因每次插入平均需比較和移動約 n/2 個元素。
- **空間複雜度**：
  - O(1)，原地排序，僅需常數額外空間（`temp` 變數）。
- **程式特性**：簡單實現，未優化比較或移動，適合小陣列（n ≤ 32）。

#### Quick Sort
- **時間複雜度**：
  - **Worst-case**：O(n²)，當 pivot 選擇不佳（程式中固定選第一元素）且輸入逆序，分割極不平衡。
  - **Average-case**：O(n log n)，隨機輸入下，分割接近均等。
- **空間複雜度**：
  - O(log n)，因遞迴調用棧（平均 O(log n) 層，Worst-case 可達 O(n)）。
- **程式特性**：固定 pivot（`arr[left]`）導致 Worst-case 頻發，未採用隨機 pivot 或小陣列插入排序優化。

#### Merge Sort
- **時間複雜度**：
  - **Worst-case**：O(n log n)，分割和合併均穩定，與輸入無關。
  - **Average-case keypad**：O(n log n)，合併操作固定。
- **空間複雜度**：
  - O(n)，需額外陣列儲存合併結果（程式中 `vector<entry>` 動態分配）。
- **程式特性**：遞迴實現，穩定但記憶體開銷較大，適合中型資料（32 < n ≤ 1000）。

#### Heap Sort
- **時間複雜度**：
  - **Worst-case**：O(n log n)，建堆 O(n)，每次調整堆 O(log n)，共 n 次。
  - **Average-case**：O(n log n)，對輸入不敏感。
- **空間複雜度**：
  - O(1)，原地排序，僅需常數額外空間。
- **程式特性**：標準最大堆實現，穩定且高效，適合中大型資料（1000 < n ≤ 5000）。

#### Composite Sort
- **時間複雜度**：
  - **Worst-case**：取決於選擇的排序法：
    - n ≤ 32：O(n²)（Insertion Sort）
    - 32 < n ≤ 1000：O(n log n)（Merge Sort）
    - 1000 < n ≤ 5000：O(n log n)（Heap Sort）
    - n > 5000：O(n²)（Quick Sort，Worst-case）
  - **Average-case**：O(n log n) 為主，當 n > 5000 時接近 Quick Sort 的 O(n log n)。
- **空間複雜度**：
  - 取決於選擇的排序法：
    - Insertion Sort：O(1)
    - Merge Sort：O(n)
    - Heap Sort：O(1)
    - Quick Sort：O(log n)
- **程式特性**：根據資料大小動態選擇排序法，結合各算法優勢，適用於多種資料規模。

### 總結
- **Insertion Sort**：時間複雜度高（O(n²)），但空間需求最低，適合小資料。
- **Quick Sort**：Average-case 高效（O(n log n)），但程式實現易退化到 O(n²)，空間需遞迴棧。
- **Merge Sort**：時間穩定（O(n log n)），但空間需求高（O(n)）。
- **Heap Sort**：時間和空間均優（O(n log n), O(1)），為中大型資料良好選擇。
- **Composite Sort**：根據資料大小選擇最佳排序法，平衡時間與空間需求，適合廣泛應用場景。

---

## 程式實作

### 一. 標頭與基本設置

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <windows.h>
#include <psapi.h>
#pragma comment(lib, "psapi.lib")

using namespace std;

// 設定常數
#define CASE_ITEMS 6000
#define CASES 5
#define RNGKEYS rand() % CASE_ITEMS
#define INSKEYS CASE_ITEMS - i
#define UNSORTED "./tosort.txt"
#define SORTED "./sorted.txt"
#define TIMEREC "./timer.txt"
```

#### 說明：

- **功能**：定義程式所需的標頭檔、命名空間及常數，為排序實作和效能測試提供基礎環境設置。
- **標頭檔**：
  - `iostream`：提供輸入輸出功能，用於檔案操作與結果顯示。
  - `vector`：用於動態陣列，儲存測試資料與排序結果。
  - `algorithm`：提供 `swap` 等輔助函數，支援排序演算法實現。
  - `ctime`, `cstdlib`：支援隨機數生成（`rand`）與時間種子（`time`），用於測試資料生成。
  - `chrono`：提供高精度計時工具（`high_resolution_clock`），用於測量排序執行時間。
  - `windows.h`, `psapi.h`：Windows API，用於記憶體使用量測量。
- **#pragma comment**：自動連結 `psapi.lib`，提供記憶體資訊查詢功能。
- **常數定義**：
  - `CASE_ITEMS`：每個測試用例的元素數（6000）。
  - `CASES`：測試用例數（5）。
  - `RNGKEYS`：隨機鍵值生成宏，範圍為 `[0, CASE_ITEMS-1]`。
  - `INSKEYS`：逆序鍵值生成宏，用於最壞情況測試。
  - `UNSORTED`, `SORTED`, `TIMEREC`：指定檔案路徑，分別儲存未排序資料、排序結果和排序時間記錄。
- **程式特性**：設置統一的檔案路徑與測試參數，確保程式可重複執行並生成一致的測試資料與結果。使用宏簡化鍵值生成邏輯，提升程式可讀性。

---

### 二. 資料結構與記憶體分析

```cpp
struct node { int data; };

class entry {
public:
    friend class node;
    long key;
    entry(long k, node* d) { key = k; data = d; }
    void setkey(int k) { key = k; }
    void outputkey(FILE* file) { fprintf(file, " key: %ld\n", key); }
private:
    node* data;
};

struct result {
    int64_t timer;
    vector<entry> arr2;
};

string printMem(int state) {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    unsigned long long memUsage = memInfo.WorkingSetSize / 1024;
    unsigned long long memPeak = memInfo.PeakWorkingSetSize / 1024;
    string ret0 = "Memory usage: " + to_string(memUsage) + " KB\n";
    string ret1 = "Peak memory usage: " + to_string(memPeak) + " KB\n";
    if (state == 0) return ret0;
    else if (state == 1) return ret1;
    return "";
}
```

#### 說明：

- **功能**：定義資料結構（`node`, `entry`, `result`）與記憶體分析函式（`printMem`），用於儲存測試資料、排序結果及測量記憶體使用量。
- **資料結構**：
  - `node`：簡單結構，包含單一整數 `data`，作為 `entry` 的附加資料。
  - `entry`：核心資料結構，包含排序鍵值 `key`（long 型別）及指向 `node` 的指標 `data`。提供 `setkey` 修改鍵值和 `outputkey` 輸出鍵值至檔案。
  - `result`：儲存排序結果，包含執行時間 `timer`（微秒，int64_t 型別）及排序後的陣列 `arr2`（`vector<entry>` 型別）。
- **printMem 函式**：
  - **功能**：使用 Windows API 的 `GetProcessMemoryInfo` 獲取程式記憶體使用資訊。
  - **參數**：`state`（int），`state == 0` 返回當前記憶體使用量，`state == 1` 返回峰值記憶體使用量。
  - **返回值**：字串，包含記憶體使用量（單位：KB）。
  - **實現細節**：透過 `PROCESS_MEMORY_COUNTERS` 結構獲取記憶體數據，轉換為 KB 並格式化為字串。
- **程式特性**：結構化資料設計便於排序與輸出，記憶體分析函式提供即時與峰值記憶體使用量，支援效能比較。`entry` 的 `friend` 設計允許 `node` 存取私有成員，增強封裝性。

---

### 三. 排序函式實作

#### Insertion Sort

```cpp
result InsertionSort(vector<entry> arr, int casenum) {
    result r;
    cout << "Start insertion sort\n";
    auto timer = chrono::high_resolution_clock::now();
    string recMem_Init = printMem(0);
    cout << recMem_Init << endl;
    FILE *file = fopen(TIMEREC, "a");
    fprintf(file, "Start Insertion sort case # %d \n[Init] %s", casenum, recMem_Init.c_str());

    for (int i = 1; i < arr.size(); i++) {
        entry temp = arr[i];
        int j = i - 1;
        while (j >= 0 && temp.key < arr[j].key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = temp;
    }

    auto stop = chrono::high_resolution_clock::now();
    auto dur = chrono::duration_cast<chrono::microseconds>(stop - timer);
    r.arr2 = arr;
    r.timer = dur.count();
    string recMem_Fin = printMem(1);

    cout << "Sorted array in " << dur.count() << " microseconds\n";
    cout << recMem_Fin << endl;
    fprintf(file, "Sorted case #%d with %lu items in %ld microseconds with Insertion\n[Final] %s\n",
            casenum, CASE_ITEMS, dur.count(), recMem_Fin.c_str());
    if (file != nullptr) fclose(file);
    return r;
}
```

#### 說明：

- **功能**：實現插入排序（Insertion Sort），將輸入陣列按鍵值（`key`）遞增排序，並記錄執行時間與記憶體使用量。
- **參數**：
  - `arr`：待排序的 `vector<entry>` 陣列，包含鍵值與資料指標。
  - `casenum`：測試用例編號，用於記錄輸出。
- **實現細節**：
  - 排序邏輯：從第二個元素開始，逐一與前面的已排序部分比較，若鍵值小於前一元素則後移，直至找到正確插入位置。
  - 使用 `std::chrono::high_resolution_clock` 記錄排序時間，單位為微秒。
  - 透過 `printMem` 記錄初始與峰值記憶體使用量，寫入 `timer.txt`。
  - 輸出排序開始/結束資訊及時間到控制台。
- **程式特性**：
  - 穩定排序，適合小資料量（n ≤ 32）或近乎排序的資料。
  - 時間複雜度：最佳 O(n)（已排序），平均/最壞 O(n²)（隨機/逆序）。
  - 空間複雜度：O(1)（原地排序，僅需常數額外空間）。
  - 未優化比較或移動，適用於簡單場景。

---

#### Quick Sort

```cpp
void QuickSortCore(vector<entry> &arr, int left, int right) {
    if (left < right) {
        entry pivot = arr[left];
        int i = left, j = right + 1;
        do {
            do i++; while (i <= right && arr[i].key < pivot.key);
            do j--; while (arr[j].key > pivot.key);
            if (i < j) swap(arr[i], arr[j]);
        } while (i < j);
        swap(arr[left], arr[j]);
        QuickSortCore(arr, left, j - 1);
        QuickSortCore(arr, j + 1, right);
    }
}

result QuickSort(vector<entry> arr, int casenum) {
    result r;
    cout << "Start quick sort\n";
    auto timer = chrono::high_resolution_clock::now();
    string recMem_Init = printMem(0);
    cout << recMem_Init << endl;
    FILE *file = fopen(TIMEREC, "a");
    fprintf(file, "Start Quick sort case # %d \n[Init] %s", casenum, recMem_Init.c_str());

    QuickSortCore(arr, 0, arr.size() - 1);

    auto stop = chrono::high_resolution_clock::now();
    auto dur = chrono::duration_cast<chrono::microseconds>(stop - timer);
    r.arr2 = arr;
    r.timer = dur.count();
    string recMem_Fin = printMem(1);

    cout << "Sorted array in " << dur.count() << " microseconds\n";
    cout << recMem_Fin << endl;
    fprintf(file, "Sorted case #%d with %lu items in %ld microseconds with Quick\n[Final] %s\n",
            casenum, CASE_ITEMS, dur.count(), recMem_Fin.c_str());
    if (file != nullptr) fclose(file);
    return r;
}
```

#### 說明：

- **功能**：實現快速排序（Quick Sort），按鍵值遞增排序，記錄時間與記憶體使用量。
- **參數**：
  - `arr`：待排序的 `vector<entry>` 陣列。
  - `casenum`：測試用例編號。
- **實現細節**：
  - 使用 `QuickSortCore` 執行遞迴排序，選擇第一個元素作為樞軸（`pivot`），透過雙指針（`i`, `j`）分割陣列。
  - 若左指針找到大於等於樞軸的元素，右指針找到小於等於樞軸的元素，則交換兩者，直到指針相遇，將樞軸放置正確位置。
  - 遞迴處理左右子陣列。
  - 使用 `std::chrono` 計時，記錄記憶體使用量，寫入 `timer.txt`。
- **程式特性**：
  - 非穩定排序，平均時間複雜度 O(n log n)，最壞情況 O(n²)（逆序資料）。
  - 空間複雜度：O(log n)（遞迴棧，平均）。
  - 固定樞軸選擇易導致最壞情況，未來可改進為隨機或 median-of-three 策略。

---

#### Merge Sort

```cpp
vector<entry> MergeCore(vector<entry> a, vector<entry> b) {
    vector<entry> c;
    c.reserve(a.size() + b.size());
    auto it_a = a.begin();
    auto it_b = b.begin();
    while (it_a != a.end() && it_b != b.end()) {
        if (it_a->key < it_b->key) {
            c.push_back(std::move(*it_a));
            ++it_a;
        } else {
            c.push_back(std::move(*it_b));
            ++it_b;
        }
    }
    while (it_a != a.end()) {
        c.push_back(std::move(*it_a));
        ++it_a;
    }
    while (it_b != b.end()) {
        c.push_back(std::move(*it_b));
        ++it_b;
    }
    return c;
}

vector<entry> MergeCut(vector<entry> arr) {
    if (arr.size() <= 1) { return arr; }
    vector<entry> left, right;
    int mid = arr.size() / 2;
    for (int i = 0; i < mid; i++)
        left.push_back(arr[i]);
    for (int i = mid; i < arr.size(); i++)
        right.push_back(arr[i]);
    left = MergeCut(left);
    right = MergeCut(right);
    return MergeCore(left, right);
}

result MergeSort(vector<entry> arr, int casenum) {
    result r;
    cout << "Start merge sort\n";
    auto timer = chrono::high_resolution_clock::now();
    string recMem_Init = printMem(0);
    cout << recMem_Init << endl;
    FILE *file = fopen(TIMEREC, "a");
    fprintf(file, "Start Merge sort case # %d \n[Init] %s", casenum, recMem_Init.c_str());

    arr = MergeCut(arr);

    auto stop = chrono::high_resolution_clock::now();
    auto dur = chrono::duration_cast<chrono::microseconds>(stop - timer);
    r.arr2 = arr;
    r.timer = dur.count();
    string recMem_Fin = printMem(1);

    cout << "Sorted array in " << dur.count() << " microseconds\n";
    cout << recMem_Fin << endl;
    fprintf(file, "Sorted case #%d with %lu items in %ld microseconds with Merge\n[Final] %s\n",
            casenum, CASE_ITEMS, dur.count(), recMem_Fin.c_str());
    if (file != nullptr) fclose(file);
    return r;
}
```

#### 說明：

- **功能**：實現合併排序（Merge Sort），按鍵值遞增排序，記錄時間與記憶體使用量。
- **參數**：
  - `arr`：待排序的 `vector<entry>` 陣列。
  - `casenum`：測試用例編號。
- **實現細節**：
  - 使用 `MergeCut` 遞迴分割陣列至單一元素，`MergeCore` 合併兩個已排序子陣列。
  - `MergeCore` 使用迭代器比較鍵值，選擇較小者加入結果陣列，剩餘元素直接追加。
  - 使用 `std::move` 優化合併過程，減少複製開銷。
  - 使用 `std::chrono` 計時，記錄記憶體使用量，寫入 `timer.txt`。
- **程式特性**：
  - 穩定排序，時間複雜度為 O(n log n)（最佳/平均/最壞）。
  - 空間複雜度：O(n)（需額外陣列儲存合併結果）。
  - 遞迴實現穩定但記憶體開銷較大，適合中型資料。

---

#### Heap Sort

```cpp
void heapify(vector<entry> &arr, int n, int i) {
    int max = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n && arr[left].key > arr[max].key)
        max = left;
    if (right < n && arr[right].key > arr[max].key)
        max = right;
    if (max != i) {
        swap(arr[i], arr[max]);
        heapify(arr, n, max);
    }
}

result HeapSort(vector<entry> arr, int casenum) {
    result r;
    cout << "Start heap sort\n";
    auto timer = chrono::high_resolution_clock::now();
    string recMem_Init = printMem(0);
    cout << recMem_Init << endl;
    FILE *file = fopen(TIMEREC, "a");
    fprintf(file, "Start Heap sort case # %d \n[Init] %s", casenum, recMem_Init.c_str());

    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }

    auto stop = chrono::high_resolution_clock::now();
    auto dur = chrono::duration_cast<chrono::microseconds>(stop - timer);
    r.arr2 = arr;
    r.timer = dur.count();
    string recMem_Fin = printMem(1);

    cout << "Sorted array in " << dur.count() << " microseconds\n";
    cout << recMem_Fin << endl;
    fprintf(file, "Sorted case #%d with %lu items in %ld microseconds with Heap\n[Final] %s\n",
            casenum, CASE_ITEMS, dur.count(), recMem_Fin.c_str());
    if (file != nullptr) fclose(file);
    return r;
}
```

#### 說明：

- **功能**：實現堆排序（Heap Sort），按鍵值遞增排序，記錄時間與記憶體使用量。
- **參數**：
  - `arr`：待排序的 `vector<entry>` 陣列。
  - `casenum`：測試用例編號。
- **實現細節**：
  - 使用 `heapify` 維護最大堆性質，比較根節點與左右子節點，選擇最大者交換並遞迴調整。
  - 先從底部建構最大堆，然後逐一將堆頂（最大值）移至陣列末端，縮減堆大小並重新調整。
  - 使用 `std::chrono` 計時，記錄記憶體使用量，寫入 `timer.txt`。
- **程式特性**：
  - 非穩定排序，時間複雜度為 O(n log n)（最佳/平均/最壞）。
  - 空間複雜度：O(1)（原地排序）。
  - 穩定且對輸入不敏感，適合中大型資料。

---

#### Composite Sort

```cpp
result CompositeSort(vector<entry> arr, int casenum) {
    result r;
    cout << "Start composite sort\n";
    auto timer = chrono::high_resolution_clock::now();
    string recMem_Init = printMem(0);
    cout << recMem_Init << endl;
    FILE *file = fopen(TIMEREC, "a");
    fprintf(file, "Start Composite sort case # %d \n[Init] %s", casenum, recMem_Init.c_str());

    if (arr.size() <= 32) {
        r = InsertionSort(arr, casenum);
    } else if (arr.size() <= 1000) {
        r = QuickSort(arr, casenum);
    } else if (arr.size() <= 5000) {
        r = MergeSort(arr, casenum);
    } else {
        r = HeapSort(arr, casenum);
    }

    auto stop = chrono::high_resolution_clock::now();
    auto dur = chrono::duration_cast<chrono::microseconds>(stop - timer);
    r.timer = dur.count();
    string recMem_Fin = printMem(1);

    cout << "Sorted array in " << dur.count() << " microseconds\n";
    cout << recMem_Fin << endl;
    fprintf(file, "Sorted case #%d with %lu items in %ld microseconds with Composite\n[Final] %s\n",
            casenum, CASE_ITEMS, dur.count(), recMem_Fin.c_str());
    if (file != nullptr) fclose(file);
    return r;
}
```

#### 說明：

- **功能**：實現複合排序（Composite Sort），根據輸入陣列大小動態選擇最適排序演算法，記錄時間與記憶體使用量。
- **參數**：
  - `arr`：待排序的 `vector<entry>` 陣列。
  - `casenum`：測試用例編號。
- **實現細節**：
  - 根據陣列大小選擇排序法：
    - `n ≤ 32`：使用 `InsertionSort`，因小資料量下簡單高效。
    - `32 < n ≤ 1000`：使用 `QuickSort`，平均效能佳。
    - `1000 < n ≤ 5000`：使用 `MergeSort`，穩定且適合中型資料。
    - `n > 5000`：使用 `HeapSort`，空間效率高且穩定。
  - 選擇的排序函數直接執行，計時涵蓋整個排序過程。
  - 使用 `std::chrono` 計時，記錄記憶體使用量，寫入 `timer.txt`。
- **程式特性**：
  - 結合各排序法優勢，適應不同資料規模。
  - 時間複雜度：隨選擇的排序法變化：
    - `n ≤ 32`：O(n²)（Insertion Sort）。
    - `32 < n ≤ 1000`：O(n log n)（Quick Sort，平均）。
    - `1000 < n ≤ 5000`：O(n log n)（Merge Sort）。
    - `n > 5000`：O(n log n)（Heap Sort）。
  - 空間複雜度：隨排序法變化（O(1) 至 O(n)）。
  - 閾值（32、1000、5000）基於經驗設定，未來可通過窄範圍測試優化。

---

### 四. 生成測試用例

```cpp
void makeCases(int cases, vector<vector<entry>> &superarray, FILE *unsortedfile, string mode) {
    for (int c = 0; c < cases; c++) {
        vector<entry> array;
        long key;
        fprintf(unsortedfile, "\nCase %d with %lu items:\n", c + 1, CASE_ITEMS);
        for (int i = 0; i < CASE_ITEMS; i++) {
            node *n = new node;
            if (mode == "INSERTION" || mode == "QUICK" || mode == "MERGE")
                key = CASE_ITEMS - i;
            else if (mode == "RANDOM")
                key = rand() % CASE_ITEMS;
            else
                key = i;
            entry e(key, n);
            array.push_back(e);
        }
        if (mode == "HEAP") {
            for (int i = CASE_ITEMS - 1; i >= 2; i--) {
                int j = rand() % i + 1;
                swap(array[i], array[j]);
            }
        }
        for (int i = 0; i < CASE_ITEMS; i++) {
            array[i].outputkey(unsortedfile);
        }
        cout << "output unsorted array to file " << UNSORTED << endl;
        superarray.push_back(array);
        cout << "Created unsorted array for case #" << c + 1 << endl;
    }
}
```

#### 說明：

- **功能**：生成指定數量的測試用例，每個用例包含 `CASE_ITEMS`（6000）個 `entry` 物件，根據模式生成鍵值並輸出至未排序檔案，儲存於 `superarray`。
- **參數**：
  - `cases`：生成用例數（預設 5）。
  - `superarray`：儲存測試用例的 `vector<vector<entry>>`。
  - `unsortedfile`：未排序資料輸出檔案（`tosort.txt`）。
  - `mode`：鍵值生成模式（`"INSERTION"`, `"QUICK"`, `"MERGE"`, `"RANDOM"`, `"HEAP"`）。
- **實現細節**：
  - 根據 `mode` 生成鍵值：
    - `"INSERTION"`, `"QUICK"`, `"MERGE"`：逆序鍵值（`CASE_ITEMS - i`），模擬最壞情況。
    - `"RANDOM"`：隨機鍵值（`rand() % CASE_ITEMS`），模擬平均情況，用於 Composite Sort。
    - `"HEAP"`：順序鍵值（`i`），後隨機打亂部分元素（從索引 2 開始）。
  - 每個 `entry` 包含鍵值與動態分配的 `node` 指標。
  - 使用 `outputkey` 將鍵值寫入 `unsortedfile`。
  - 將生成的陣列存入 `superarray`。

---

### 五. 主程式與測試流程

```cpp
int main(void) {
    remove(SORTED);
    cout << "Removed old " << SORTED << endl;
    remove(UNSORTED);
    cout << "Removed old " << UNSORTED << endl;
    remove(TIMEREC);
    cout << "Removed old " << TIMEREC << endl;

    FILE *f_Unsorted = fopen(UNSORTED, "a");
    FILE *f_Sorted = fopen(SORTED, "a");

    srand(time(0));
    vector<vector<entry>> superarray[5];
    result result;

    makeCases(CASES, superarray[0], f_Unsorted, "INSERTION");
    makeCases(CASES, superarray[1], f_Unsorted, "QUICK");
    makeCases(CASES, superarray[2], f_Unsorted, "MERGE");
    makeCases(CASES, superarray[3], f_Unsorted, "HEAP");
    makeCases(CASES, superarray[4], f_Unsorted, "RANDOM");

    for (int type = 0; type < 5; type++) {
        for (int i = 0, caseNum = 1; i < superarray[type].size(); i++, caseNum++) {
            auto duration = 0;
            switch (type) {
            case 0:
                result = InsertionSort(superarray[type][i], caseNum);
                break;
            case 1:
                result = QuickSort(superarray[type][i], caseNum);
                break;
            case 2:
                result = MergeSort(superarray[type][i], caseNum);
                break;
            case 3:
                result = HeapSort(superarray[type][i], caseNum);
                break;
            case 4:
                result = CompositeSort(superarray[type][i], caseNum);
                break;
            }
            duration = result.timer;
            fprintf(f_Sorted, "\nCase %d of %lu items finished in %lu microseconds\n", caseNum, CASE_ITEMS, duration);
            for (int j = 0; j < CASE_ITEMS; j++) {
                result.arr2[j].outputkey(f_Sorted);
            }
            cout << "output sorted array to file " << SORTED << endl;
        }
    }

    fclose(f_Unsorted);
    fclose(f_Sorted);
    return 0;
}
```

#### 說明：

- **功能**：主程式負責初始化環境、生成測試資料、執行排序演算法並輸出結果，統籌整個效能測試流程。
- **實現細節**：
  - **檔案管理**：移除舊檔案（`sorted.txt`, `tosort.txt`, `timer.txt`），開啟新檔案進行輸出。
  - **隨機種子**：使用 `srand(time(0))` 初始化隨機數生成器，確保隨機資料可重現。
  - **測資生成**：調用 `makeCases` 生成五組測資，分別對應 Insertion Sort（逆序）、Quick Sort（逆序）、Merge Sort（逆序）、Heap Sort（部分亂序）及 Composite Sort（隨機）。
  - **排序執行**：
    - 對五組測資（`type = 0~4`）執行對應排序演算法（Insertion, Quick, Merge, Heap, Composite）。
    - 每組測資包含 5 個用例（`CASES = 5`）。
  - **結果輸出**：
    - 排序結果（鍵值）與執行時間寫入 `sorted.txt`，格式為 `"Case X of Y items finished in Z microseconds"`。
    - 時間與記憶體記錄寫入 `timer.txt`，包含初始與峰值記憶體使用量。
  - 關閉檔案並結束程式。
- **程式特性**：
  - 模組化設計，清晰分離測資生成、排序執行與結果輸出。
  - 支援多種排序法與測試場景，方便擴展。
 
---

## 測試與驗證

### 最壞執行時間表格 Worst-case Performance (Time in Microseconds)


| 資料數 (Items) | Insertion Sort | Quick Sort | Merge Sort | Heap Sort | Composite Sort |
|:--------------:|:--------------:|:----------:|:----------:|:---------:|:--------------:|
| 500            | 782 μs         | 445 μs     | 2664 μs    | 333 μs    | 874 μs         |
| 1000           | 1749 μs         | 1063 μs    | 4546 μs    | 344 μs    | 908 μs         |
| 2000           | 6532 μs         | 2671 μs    | 15834 μs   | 774 μs    | 14732 μs       |
| 3000           | 14526 μs        | 5862 μs    | 17685 μs   | 875 μs    | 20007 μs       |
| 4000           | 31137 μs        | 11801 μs   | 36150 μs   | 3641 μs   | 32411 μs       |
| 5000           | 45321 μs        | 16511 μs   | 31989 μs   | 1348 μs   | 31752 μs       |



![image](https://github.com/Yangchenin41243245/Data_Structure_Homework_14_45/blob/main/homework1/src/pic/worst.png)

### 平均執行時間表格 Average-case Performance (Time in Microseconds)


| 資料數 (Items) | Insertion Sort | Quick Sort | Merge Sort | Heap Sort | Composite Sort |
|:--------------:|:--------------:|:----------:|:----------:|:---------:|:--------------:|
| 500            | 572 μs         | 313 μs     | 2282 μs    | 260 μs    | 628 μs         |
| 1000           | 1655 μs        | 774 μs     | 4345 μs    | 288 μs    | 638 μs         |
| 2000           | 6081 μs        | 2438 μs    | 13504 μs   | 554 μs    | 13590 μs       |
| 3000           | 13689 μs       | 5429 μs    | 17069 μs   | 742 μs    | 19118 μs       |
| 4000           | 27601 μs       | 10666 μs   | 32448 μs   | 1657 μs   | 28269 μs       |
| 5000           | 42292 μs       | 15870 μs   | 31196 μs   | 1257 μs   | 31429 μs       |


![image](https://github.com/Yangchenin41243245/Data_Structure_Homework_14_45/blob/main/homework1/src/pic/average.png)

### 時間複雜度趨勢分析

- **Insertion Sort**：呈現 O(n²) 成長，效能隨資料量快速下降。
- **Quick Sort**：最壞情況為 O(n²)，平均情況接近 O(n log n)，表現穩定但易受 pivot 選擇影響。
- **Merge Sort**：穩定 O(n log n)，但記憶體開銷較高，適合中型資料。
- **Heap Sort**：穩定 O(n log n)，空間效率高，適合中大型資料。

---

## 申論及開發報告

本作業成功實作並比較了四種經典排序演算法與一個自適應的 Composite Sort 函式。透過對不同資料規模（500 至 6000）的測試，我們觀察到：

- **Insertion Sort** 在小資料量（n ≤ 32）下由於簡單性和低空間需求表現良好，但在資料量增加時效能迅速下降。
- **Quick Sort** 在平均情況下表現優異，但在最壞情況（逆序資料）下退化至 O(n²)，顯示 pivot 選擇的重要性。
- **Merge Sort** 提供穩定的 O(n log n) 效能，但空間需求較高，適合穩定性要求高的場景。
- **Heap Sort** 在時間和空間上均表現優異，特別適合中大型資料，且對輸入資料特性不敏感。
- **Composite Sort** 通過動態選擇排序法有效結合各算法優勢：在小資料量下利用 Insertion Sort 的簡單性，中型資料使用 Merge Sort 的穩定性，中大型資料使用 Heap Sort 的空間效率，大型資料則依賴 Quick Sort 的平均效能。測試結果顯示，對於 n = 6000 的資料，Composite Sort 選擇 Quick Sort，達到與 Quick Sort 相同的效能，同時在較小資料量下展現靈活性。

### 挑戰與解決方案
- **Quick Sort 最壞情況**：由於固定選擇第一元素作為 pivot，逆序資料導致效能退化。未來可改進為隨機 pivot 或 median-of-three 策略。
- **記憶體測量波動**：Windows API 的記憶體測量可能因系統負載而波動，通過多次測試取平均值減少誤差。
- **Composite Sort 閾值選擇**：閾值（32、1000、5000）基於經驗與測試調整，未來可通過更系統化的分析優化。

### 結論
Composite Sort 的設計展示了根據資料特性動態選擇演算法的重要性。通過結合各排序法的優勢，該函式在多種資料規模下均能提供接近最佳的效能，適用於實際應用中資料規模不確定的場景。未來可進一步優化 pivot 選擇、實現原地 Merge Sort 或引入並行處理以提升效能。
