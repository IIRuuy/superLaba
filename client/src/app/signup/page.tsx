'use client'
import { useCookies } from 'next-client-cookies';

import { AuthService } from "@/service/AuthService";
import { useState } from "react";
import { useRouter } from 'next/navigation';
import { UserService } from '@/service/UserService';

export default function SignIn() {
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  const [name, setName] = useState("");
  const [age, setAge] = useState(0);

  const [error, setError] = useState("");
  const cookieStore = useCookies();
  const { push } = useRouter();
  
  const onclick = async () => {
    const res = await UserService.createUser(username, password, name, age);
    if(res.status == 201) {
        const resAuth = await AuthService.signin(username, password);
        cookieStore.set("auth", resAuth.auth);
        resAuth?.user.then(user => push(`/user/${user.id}`));
    } else {
        res.json().then(error => setError(error?.error))
    }
  }

  return (
    <div style={{display : "grid", width : "60vh", margin: "auto"}}>
      <div style={{display : "grid", width : "60vh", margin: "auto"}}>
              <h1>Регистрация</h1>
              <input
                      name="username"
                      placeholder="Username"
                      value={username}
                      onChange={e => setUsername(e.target.value)} 
                />
              <input
                      name="password"
                      placeholder="Password"
                      value={password}
                      onChange={e => setPassword(e.target.value)} 
                />
              <input
                      name="name"
                      placeholder="Name"
                      value={name}
                      onChange={e => setName(e.target.value)} 
                />
              <input
                      type='number'
                      name="age"
                      placeholder="Age"
                      value={age}
                      onChange={e => setAge(parseInt(e.target.value))} 
                />

                {error != "" && <div>{error}</div>}
              <button onClick={onclick}>Зарегистрироваться</button>
          </div>
          <div style={{margin: "1vh 0"}}>
            <a href="/signin">Войти</a>
          </div>
    </div>
  );
}
