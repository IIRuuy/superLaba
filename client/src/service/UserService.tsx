
const BASE_PATH = "http://localhost:8080"

export const UserService = {
    async getUser(userId: number, auth: string) {
        return await fetch(`${BASE_PATH}/user/${userId}`, {
            method: "get",
            mode : "cors",
            "headers" : {
                "Authorization" : `Basic ${auth}`
            }
        })
    },

    async createUser(
        username:string, password:string, name:string, age:number
    ) {
        return await fetch(`${BASE_PATH}/user`, {
            method: "post",
            mode : "cors",
            headers : {
                "Content-Type": "application/json",
            },
            body : JSON.stringify({
                username : username,
                password : password,
                name : name,
                age: age
            })
        })
    },

    async deleteUser(userId:string, auth:string) {
        return await fetch(`${BASE_PATH}/user/${userId}`, {
            method: "delete",
            mode : "cors",
            "headers" : {
                "Authorization" : `Basic ${auth}`
            }
        })
    },

    async changeUser(userId:string, name:string, age:number, auth:string) {
        return await fetch(`${BASE_PATH}/user/${userId}`, {
            method: "PATCH",
            mode : "cors",
            headers : {
                "Authorization" : `Basic ${auth}`,
                "Content-Type": "application/json"
            },
            body : JSON.stringify({
                name : name,
                age: age
            })
        })
    },
    
    async getAllUser(auth: string) {
        return await fetch(`${BASE_PATH}/user`, {
            method: "get",
            mode : "cors",
            "headers" : {
                "Authorization" : `Basic ${auth}`
            }
        })
    },
}