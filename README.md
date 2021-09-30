# TP-Socket-Berkeley


## Client

```cpp

struct Callbacks 
{
    std::function<X> onConnect;
    std::function<X> onMessage;
    std::function<X> onDisconnect;
}

int main()
{
    SocketLib sl;
    Connection connection = sl.Connect("1.2.3.4",56789, callbacks);   

    // ...

    connection.Send("ping");
}
```