'use client'

import UserComponent from '@/component/UserComponent'
import { UserService } from '@/service/UserService';
import { useCookies } from 'next-client-cookies';
import { useRouter } from 'next/navigation';
import { useEffect, useState } from 'react';

export default function User({params}:any) {
  const cookieStore = useCookies();
  const {push}= useRouter();

  const [msg, setMsg] = useState("");
  const [user, setUser] = useState(null);
  const [name, setName] = useState("");
  const [age, setAge] = useState(0);

  const onClick = async () => {
    const auth = cookieStore.get("auth")
    if(auth == undefined){
      push("/signin")
    } else {
      const response = await UserService.getUser(params.userId, auth);
      if(response.status == 401){
        cookieStore.remove("auth");
        push("/signin");
      }
      if(response.status == 404)
        setMsg("Пользователь не найден!")
      if(response.status == 403)
        setMsg("У вас нет прав на просмотр данных об этом пользователе!")
      if(response.status == 200){
        response.json().then(user => {
          setUser(user);
          setName(user?.name);
          setAge(user?.age);
        });
        
      }
    }
  }

  const onDelete = async () => {
    const auth = cookieStore.get("auth")
    if(auth == undefined){
      push("/signin")
    } else {
      const response = await UserService.deleteUser(params.userId, auth);
      if(response.status == 401){
        cookieStore.remove("auth");
        push("/signin");
      }
      if(response.status == 404)
        setMsg("Пользователь не найден!")
      if(response.status == 403)
        setMsg("У вас нет прав на удаление данного пользователя!")
      if(response.status == 204){
        setUser(null);
        setMsg("Пользователь успешно удалён!");
      }
    }
  }

  const onChange = async () => {
    const auth = cookieStore.get("auth")
    if(auth == undefined){
      push("/signin")
    } else {
      const response = await UserService.changeUser(params.userId,name, age ,auth);
      if(response.status == 401){
        cookieStore.remove("auth");
        push("/signin");
      }
      if(response.status == 404)
        setMsg("Пользователь не найден!")
      if(response.status == 403)
        setMsg("У вас нет прав на удаление данного пользователя!")
      if(response.status == 200){
        response.json().then(user => {
          setName(user?.name);
          setAge(user?.age);
          setMsg("Пользователь изменён!");
        });
      }
      if(response.status == 400)
        response.json().then(error => setMsg(error["error"]));
    }
  }

  useEffect(() => {
    const fetchData = async () => {
        await onClick(); 
        console.log(user)
    };

    fetchData();
  }, []);

  return (
    <div style={{display : "grid", width : "40vh", margin: "auto"}}>
      {user != null && <div>User ID : {user?.id}</div>}
      {user != null && <div>Username : {user?.username}</div>}
      {user != null && <input name="name" placeholder="Name" value={name} onChange={e => setName(e.target.value)}/>}
      {user != null && <input type='number' name="age" placeholder="Age" value={age} onChange={e => setAge(parseInt(e.target.value))}/>}
      
      {msg != "" && <div>{msg}</div>}
      {user != null && <button onClick={onChange}>Изменить</button>}
      {user != null && <button onClick={onDelete}>Удалить</button>}
      {user != null && <button onClick={() => {cookieStore.remove("auth"); window.location.reload(); }}>Выйти</button>}
    </div>
  );
}
