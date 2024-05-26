import { Base64 } from 'js-base64';

const BASE_PATH = "http://localhost:8080"

export const AuthService = {
    async signin(username : string, password: string) {
        const baseData = username + ":" + password;
        const authHeader = Base64.encode(baseData);

        return await fetch(`${BASE_PATH}/signin`, {
            method: "post",
            "headers" : {
                "Authorization" : `Basic ${authHeader}`
            }
        })
        .then(response => {
            if(response.status == 200)
                return { "auth": authHeader, "user": response.json() };
            return null;
        })
    },
}