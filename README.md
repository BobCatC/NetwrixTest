# NetwrixTest

## Основные моменты:
- Для работы с файловой системой используется std::experimental::filesystem (Windows), 
  boost::filesystem (macOS);
- Для работы с маской используется std::regex;
- Проэкт кроссплатформенный (не было задачей, но реализовывалось достаточно просто);
- Основной алгоритм поиска - префикс функция;
- Приложение многопоточное.

## Алгоритм поиска подстроки:
1.  Префикс функция. 
    Найдём сначала все вхождения только первого фрагмента паттерна и тексте 
    (асимптотически линейная сложность).
2.  Функция просеивания. 
    Далее возьмём второй фрагмент паттерна и просмотрим все вхождения, найденные на предыдущем этапе.
    Будем проверять, может ли второй фрагмент продолжить данное вхождение первого.
    После этого во множестве вхождений окажутся только валидные как для первого, 
    так и для второго фрагмента паттерна.
    Далее идёт следующая итерация просеивания уже для третьего фрагмента. 
    И так далее.
    Время выполнения функции напрямую зависит от того, сколько вхождений было найдено на первом этапе

## О Си-коде в приложении
  Программа использует stdio.h для более быстрого и удобного в данной задачи ввода/вывода.
  (естественно, они обёрнуты проверками)
  
## О try-catch блоках
   Всего в коде находится три блока:
   1. В функции `main` инициализирующего потока. Данный блок ловит исключения из функций анализа аргументов и создания потоков. 
   Эти исключения фатальны для прогрмаммы, потому что связаны с парсингом аргументов, открытием output файла и созданием потоков.
   2. В функции `ThreadSearcher`. Также фатальны, потому что связаны с открытием необходимых файлов и выделением памяти.
   3. В методе `TaskExecutor::doTask`. Внутри этого блока исключения связаны только с открытием директорий и файлов, в которых идёт поиск.
    Здесь исключения интерпретируются как ошибки открытия директорий/файла из-за отсутствия доступа.
    
## О файлах, которые создаёт программа на время исполнения
  На время исполнения каждый поток создаёт в директории output файла по два файла
  1. output файл текущего потока, который в дальнейшем будет слит с остальными в один общий output файл.
  Каждый такой файл будет удалён в деструкторе `ThreadsController`. 
  2. `piForFirstPatternFragmentFile` содержит в себе значение префикс функции для первого фрагмента паттерна.
  Создан для того, чтобы при каждом поиске это значение не вычеслялось заново.
  Каждый такой файл будет удалён в деструкторе `TaskExecutor`
  
  **Все эти файлы будут удалены**
  
  ## Реальное время работы
  Во всех случаях поиск проводился с маской "*.*" в 4 потоках.
  1. Поиск "Copyright" в /usr (3.22 GB).  
     Время исполнения : 14с, размер выходного файла : 9,6 MB.  
     При выполнении команды `grep -rw /usr -e "Copyright"` однопоточная утилита справилась за 124с, при этом занимая до 220 MB.
  2. Поиск подстроки размером 1 MB в /usr (3.22 GB).  
     Время исполнения : 13с, размер выходного файла : 0 MB.
  3. Поиск 
  
  
  
  
