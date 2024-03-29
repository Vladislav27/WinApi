Задание:
Написать свой текстовый редактор с настройками (размер шрифта, прозрачность фона, цвет шрифта, цвет фона).

1. Разместите в окне многострочный дочерний Edit-контрол, занимающий всю клиентскую область.
2. Обработайте WM_SIZE для обновления размеров дочернего контрола.
3. Обработайте WM_CLOSE. Показать MessageBox, спрашивающий, закрыть ли приложение.
4. Следите за тем, осуществлял ли пользователь ввод в Edit-контрол (WM_COMMAND + EN_CHANGE). В обработке сообщения WM_CLOSE проверяйте, был ли ввод. Если ввода не было, то MessageBox не показывайте.
5. Получите текст Edit-контрола. Модифицируйте обработку WM_CLOSE так, чтобы MessageBox спрашивал, сохранить ли введенный текст. Возможные варианты: Да, Нет, Отмена. “Да” - сохранить и закрыть, “Нет” - не сохранять и закрыть, “Отмена” - не закрывать.  
Указание: Для получения текста Edit-контрола используйте сообщения WM_GETTEXTLENGTH и WM_GETTEXT.
Для сохранения текста в файл используйте функции CreateFile, WriteFile, CloseHandle.
Для выбора имени файла показывайте стандартный диалог выбора имени файла с помощью функции GetSaveFileName.

6. Добавьте в меню пункт View->Settings
7. При выборе этого пункта меню покажите модальный диалог (::DialogBox):
Все настройки диалога устанавливаются после нажатия кнопки “OK”, и не устанавливаются, если нажат “Cancel”. Если выбран check-box “Предпросмотр”, то текущие изменения сразу появляются в окне редактора.
8. При перемещении ползунка “Размер шрифта” меняйте размер шрифта в редакторе от 8 до 72. Так диапазон ползунка должен быть от 8 до 72 включительно.  
Указание. Шрифт в редактор можно выбрать с помощью сообщения WM_SETFONT. Для создания шрифта используйте функцию ::CreateFontIndirect. Следует получить текущий шрифт редактора с помощью сообщения WM_GETFONT. Затем извлечь из него структуру LOGFONT с помощью ::GetObject. У полученной структуры замените размер шрифта и передайте ее в ::CreateFontIndirect. Следите за хранением и удалением объектов FONT.
9. При перемещении ползунка “Прозрачность окна” меняйте прозрачность окна редактора от 0% до 100%. Диапазон ползунка должен быть от 0 до 255 включительно.  
Указание. Диапазон ползунка выставляется с помощью сообщений TBM_SETRANGE. Значение ползунка можно установить с помощью TBM_SETPOS, а получить с помощью TBM_GETPOS. Сообщение об изменении ползунка - WM_HSCROLL.
Чтобы окно сделать прозрачным, при создании окна укажите стиль WS_EX_LAYERED. Для изменения прозрачности окна используйте функцию ::SetLayeredWindowAttributes.
10. По нажатию кнопки выбора цвета открывайте стандартный диалог выбора цвета. После выбора цвета в окне редактора должен меняться соответствующий кнопке цвет.  
Указание. Стандартный диалог выбора создает цвета функция ::ChooseColor. Для отображения обработайте сообщение WM_CTLCOLOREDIT.
11. Настройте навигацию на диалоге с помощью клавиатуры.  
Указание. Порядок обхода регулируется в редакторе диалога с помощью Ctrl-D. Тестировать диалог из редактора можно, нажав Ctrl-T.
12. Сделайте так, чтобы диалог стал немодальным. Настройте навигацию на диалоге с помощью клавиатуры.  
Указание. Добавьте в цикл обработки сообщений вызов функции ::IsDialogMessage.

