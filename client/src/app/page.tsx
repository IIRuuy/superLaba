'use client'

import { UserService } from "@/service/UserService";
import { useCookies } from "next-client-cookies";
import { useRouter } from "next/navigation";
import { useEffect, useState } from "react";

export default function Home() {

  const cookieStore = useCookies();
  const [msg, setMsg] = useState("");
  const [users, setUsers] = useState([]);
  const {push}= useRouter();

  useEffect(() => {
    const fetchData = async () => {
      const auth = cookieStore.get("auth")
      if(auth == undefined){
        push("/signin")
      } else {
        const response = await UserService.getAllUser(auth);
        if(response.status == 401){
          cookieStore.remove("auth");
          push("/signin");
        }
        if(response.status == 403)
          setMsg("У вас нет доступа к этой странице!")
        if(response.status == 200){
          response.json().then(data => {
            setUsers( Object.values(data));
          });
        }
      }
    };
    fetchData();

  }, []);


  return (
    <div>
      {msg != "" && <div>{msg}</div>}
      {users.map((user) => 
      <a key={user?.id} href={`/user/${user.id}`}>
        <div>
          Id: {user.id}, Username: {user.username}, Name: {user.name}, Age: {user.age}
        </div>
      </a>)
      }
    </div>
  );
}
