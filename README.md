Это репозиторий с распределенной системой логирования.

Состоит из двух программ:
1. rclogd -- демон, выполняет следующие функции: 
    Команда для запуска: rclogd <socket_path>
    a) Принимает логи от локальных процессов и сохраняет их в локальную базу. Локальные процессы передают логи через UNIX сокет.
    b) Выполняет обмен логами с другими узлами.
2. rclogger -- клиент, для отправки логов в rclogd. Является примером взаимодействия локального процесса с rclogd.

Установка:
1. Установить зависимости
2. Загрузить репозиторий и скомпилировать rclogd, пример для bash:
```
git clone https://github.com/blaecwen/rc_logging.git && cd rc_logging
mkdir -p build-rclogd && cd build-rclogd && qmake ../rclogd/rclogd.pro
make
cd .. && sudo ./install.sh install
```
3. Запустить скрипт install.sh

Зависимости:
1. boost
2. libmongoc
3. libmongocxx
