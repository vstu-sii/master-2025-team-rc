## Лабораторная 3

Описание сервера, фронтенд (user flow), бэкенд.
+ описание мобильного приложения

Описание интерфейса серверной части.

Главная страница, на которой отображается текущее состояние экзоскелета (подключение к ESP32, углы приводов, заряд акб, статус записи). Также с этой страницы можно управлять записью данных.

![5Pv4MSeFwYBlYkSucjdD7qAO-ODCvs0tcy-g8CA4wYmcXcQgK07YRCaFUi6GXBeZs902N9MEB8BDXlUHUDoq8wpl](https://github.com/user-attachments/assets/56332f90-f553-492d-b758-a8f34fb356d4)


На данной странице можно просматривать данные в виде графиков (углы, момент, усилие).

![2sH0u6oWrY_NCcY6ECMjrsJwLAhNXbb_rMVkVWkEX4QIFYZUhFrWcghCkfSVXbwggYzwN2NpQD5mtWbciWfn1qL3](https://github.com/user-attachments/assets/3b997ccf-0a4d-46c4-9cc1-5fa135f78764)

Пример графика изменения углов приводов, скорости и усилия.

![0Zek3AYC7Z4oKddaQeXC_tTWh4OjiSE9XV23NydbSKNMbcS2b83_6lqxCGkIfVU8R7P8YFY3zoHjianVJIh6Z239](https://github.com/user-attachments/assets/6a48be30-6d7b-44ca-afc7-e2fcd6c49bff)


С помощью этой страницы можно взаимодействовать с файлами данных: просматривать, скачивать, удалять и преобразовывать в формат csv из binary.

![nS5AcYjPqxTUem5qoYEdaG8WmQdw08YlvFimgLL7yy4LFpQ1y-8Gk7dWwKN3CyASGE8KXNwICZ_mgOm2oUiHCc5G](https://github.com/user-attachments/assets/9d1d111d-bac9-4c03-b688-484fc64bc561)


На этой странице отображается статус работы сервера.

![EXMAKRjDr0qEhMaEzgfydMJIZkhbe3i6EVOKjyz08L3t5DjG_30gX2L6XOkq-V94alW6xUrF5aisPBJhHrpGicL1](https://github.com/user-attachments/assets/30ed5e93-7d13-421c-802c-e9a90c7b6273)


На странице “Энергосбережение” отображается текущий режим и кнопки выбора других режимов.

![Uxw1AjsKrC6jCZ995jTEZCOi6pjaFuVkLk4SLklyOm-aFUgZCpWGL6Hn5Ld1Mti-8lKYNiex7K3nyvdhrGEVOfBS](https://github.com/user-attachments/assets/404c852a-37c0-44ed-8353-238386d95e9d)


С помощью данной страницы можно управлять параметрами работы приводов экзоскелета. Есть возможность настраивать максимальные углы, выбирать уровень нагрузки и уровень усиления.

![k8bjcDopJBHpCJcrjzg-AqKCa3MUb0kZRgRz77yH21VKajcvDG6YYdkOanmKzuDGpmRMX_-iDydGMiomH87yHqig](https://github.com/user-attachments/assets/55335bc7-6148-4908-97de-acad02bb8815)


Описание блока с ESP32

Данный блок подключен к приводам экзоскелета с помощью CAN-шины. По ней происходит передача в реальном времени углов каждого привода. Полученные данные записываются на sd карту в csv формате. На дисплее отображаются текущие углы приводов, уровень заряда батареи (напряжение + процент заряда), статус записи, статус подключения по BLE, название текущего файла с данными и длительность записи.

![1764866484327](https://github.com/user-attachments/assets/08a22bc3-e72e-435b-88e7-c8070b946287)

