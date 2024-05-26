#!/bin/bash

# Запускаем Docker контейнер
docker run --name mycontainer -d client

# Проверяем статус контейнера
if [ $(docker inspect -f {{.State.Running}} mycontainer) = "true" ]; then
  echo "Контейнер успешно запущен."
  # Останавливаем контейнер
  docker stop mycontainer
  echo "Контейнер остановлен."
  return 0
else
  echo "Не удалось запустить контейнер."
  return 1
fi

# Удаляем контейнер после остановки
docker rm mycontainer
