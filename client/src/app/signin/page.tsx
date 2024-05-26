'use client'
import { useCookies } from 'next-client-cookies';

import { AuthService } from "@/service/AuthService";
import { useState } from "react";
import { useRouter } from 'next/navigation';

export default function SignIn() {
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  const [error, setError] = useState("");
  const cookieStore = useCookies();
  const { push } = useRouter();
  
  const onclick = async () => {
    const res = await AuthService.signin(username, password);
    if(res != null){
      cookieStore.set("auth", res.auth);
      res.user.then(user => push(`/user/${user.id}`));
    } else {
      setError("Не правильный логин и/или пароль!");
    }
  }

  return (
    <div style={{display : "grid", width : "40vh", margin: "auto"}}>
      <div style={{display : "grid", width : "40vh", margin: "auto"}}>
          <h1>Аутентификация</h1>
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
            {error != "" && <div>{error}</div>}
          <button onClick={onclick}>Войти</button>
      </div>
      <div style={{margin: "1vh 0"}}>
            <a href="/signup">Зарегистрироваться</a>
      </div>  
    </div>
  );
}
