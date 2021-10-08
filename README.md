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

## Checklist

1. [x] Petit main() 1 thread client, 1 thread server
2. [x] Client nul
3. [~] Server un peu moins nul
4. [ ] Gérer Asynchrone
5. [ ] Opti

Connect, migrer la création du socket dans Connection()
ajouter connect() / ready() / send()

Listen -> terminal
accept = bloquant = KK

Conn = comportement de base du client / server

Thread reseau supplémentaire Update(). (qui prend probablemnt le timeout en paramètre 50ms par exemple)
Update fait les recv en boucle avec les select
On connect dans le Update AUSSI
Terminal tjrs utile car on pourrait faire plusieurs Terminaux,