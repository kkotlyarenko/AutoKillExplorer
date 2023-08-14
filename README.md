# AutoKillExplorer
This application is designed to solve the problem of some games crashing due to the explorer.exe process, such as Forza Horizon 5 and Atomic Heart.

AutoKillExplorer automatically terminates the explorer.exe process if it detects a running program from the list, and automatically starts the explorer.exe process if all programs from the list have terminated.

By default, the names of the processes Forza Horizon 5 and Atomic Heart are added to the list.

To add an application to the list, enter the name of the application's process in the field and click the Add button.
In order to find out the name of the application process, you need to:
1. Launch this application.
2. Run cmd and enter the tasklist command.
3. Find a process name that is similar to the name of the application you want to add to the list.

To remove an application from the list, select it and click the Remove button.

You can also select the interval between checks. This affects how quickly the explorer.exe closes and opens when you open a game or application.

After clicking on the Start button when closing AutoKillExplorer will be minimized to tray.

For more convenient use, AutoKillExplorer can be added to autorun. Win + r - enter shell: common startup - throw the AutoKillExplorer shortcut into the folder. Now, at startup, if the Start button was pressed, AutoKillExplorer will start minimized and monitor the processes from the list.

You may see a Windows message on first launch stating that AutoKillExplorer is not safe. This is due to the fact that AutoKillExplorer is not digitally signed. In this case, you should add AutoKillExplorer to the list of exceptions in your antivirus.

AutoKillExplorer is written in C++ programming language using Qt framework and WinApi libraries

If you have any bugs or suggestions please use this: https://forms.gle/3uJDncafTZi76xsD8

Rus
Это приложение создано для решение проблемы, связанной с вылетом некоторых игр из-за процесса explorer.exe, например Forza Horizon 5 и Atomic Heart.

AutoKillExplorer автоматически завершает процесс explorer.exe, если обнаруживает запущенную программу из списка, и автоматически запускает процесс explorer.exe, если все программы из списка завершены.

По умолчанию в списке добавлены названия процессов Forza Horizon 5 и Atomic Heart.

Для добавления приложения в список необходимо ввести имя процесса приложения в поле и нажать кнопку Add. 
Для того чтобы узнать имя процесса приложения нужно: 
1. Запустить это приложение.
2. Запустить cmd и ввести команду tasklist.
3. Найти название процесса, похожее на название приложения, которое необоходимо добавить в список. 

Для удаления приложения из списка следует выбрать его и нажать кнопку Remove.

Также можно выбрать промежуток между проверками. Это влияет на то насколько быстро будет закрываться и открываться explorer.exe при открытии игры или приложения.

После нажатия на кнопку Start при закрытии AutoKillExplorer будет сворачиваться в трей.

Для более удобного использования AutoKillExplorer можно добавить в автозапуск.  Win+r - вводим shell:common startup - закидываем ярлык AutoKillExplorer в папку. Теперь при запуске, если была нажата кнопка Start, AutoKillExplorer будет запускаться свёрнуто и отслеживать процессы из списка.

Возможно при первом запуске Вы увидите сообщение Windows о том, что AutoKillExplorer не безопасен. Это происходит из-за того, что AutoKillExplorer не имеет цифровой подписи. В таком случае следует добавить AutoKillExplorer в список исключений в Вашем антивирусе.

AutoKillExplorer написан на языке программирования C++ с использованием фреймворка Qt и библиотек WinApi
