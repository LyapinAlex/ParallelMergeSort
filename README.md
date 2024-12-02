# Паралельный алгоритм сортировки слиянием
Данный проект представляет собой реализацию итеративной сортировки слиянием (merge sort) с использованием многопоточности на основе OpenMP. Сортировка выполняется без использования рекурсии, что делает её подходящей для сценариев, где рекурсивный стек может быть ограничен или неэффективен.

# Сборка проекта
1. Клонирование репозитория
    ```bash
   git clone https://github.com/LyapinAlex/ParallelMergeSort.git
     ```
2. Открываем Visual Studio
3. Настрйока OpenMP
    1. Перейдите в свойства проекта:
   - В меню **Visual Studio** выберите `Project → Properties`.

    2. Для всех конфигураций (**Debug**, **Release**) и платформ включите поддержку OpenMP:
   - Перейдите в `Project → Properties → C/C++ → Language → OpenMP Support` и установите значение `Yes`.
5. Сборка проекта
     Нажмите `Ctrl+Shift+B` или выберите `Build → Build Solution`.
6. Переходим в папку с скомпилированой программой
   ```bash
   cd x64\Realease
     ```
# Параметры запуска
--- 'NUM_THREADS=' - количесво задействованных потоков. Значение по умолчанию 2.
--- 'SIZE_LIST=' - размер списка. Значение по умолчанию 10000.
--- 'RANDOM_LIST=' - флаг будет ли вводиться список или получаться через рандомизацию. При значениях 0 и 1 соответственно.
# Численные результаты
![PMSTable](https://github.com/user-attachments/assets/23bb81a7-37df-41ad-8ef6-b78a9f9df359)
![PMSFigure](https://github.com/user-attachments/assets/4aa855f9-9c61-483c-87f1-19c7227dfccb)
