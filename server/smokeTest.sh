#!/bin/bash
sudo dpkg -i http-server-dcsa.deb

deb_pakage/bin/http-server &
server_pid=$!

timeout 10 nc -z 127.0.0.1 8080 > /dev/null 2>&1
if [[ $? -eq 0 ]]; then
  echo "Сервер запущен успешно!"
  kill $server_pid 
  exit 0
else
  echo "Ошибка запуска сервера!"
  kill $server_pid
  exit 1
fi
