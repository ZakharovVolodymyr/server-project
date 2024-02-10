# server-project

This project consists of three sub-projects
- Server program (receiver)
- Client program (sender)
- Dynamic library (Project2)

Sender, using dynamic library, in different streams, reads files with text in Ukrainian and English,
counts the frequency of letters and sends to the server using the local network by the TCP / IP protocol.

Receiver, when you press win + s, builds two graphs with the frequency of letters in words in Ukrainian and English languages.
When double-clicking LMB, the three most frequently occurring letters are saved in the Windows registry

The dynamic library includes functions such as working with files, data transfer over the network, word analysis functions


Цей проект складається з трьох під-проектів
-	Програма-сервер (receiver)
-	Програма-клієнт (sender)
-	Динамічної бібліотеки (Project2)

Програма-клієнт, за допомогою динамічної бібліотеки, у різних потоках, читає файли з текстом українською та англійською мовами,
підраховує частоту використання букв та відправляє до серверу по локальній мережі по протоколу TCP / IP.

Програма-сервер, при натисканні win + s, будує два графіки з частотою використання букв в словах українською та англійською мовами.
При подвійному клікі ЛКМ до реєстру windows зберігається інформація три букви, які найбільш часто зустрічаються

У динамічній бібліотеці створені функції: роботи з файлами, передачі даних по мережі та функції аналізу слів 
